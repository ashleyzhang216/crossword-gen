import json
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt

# getter for track_ac3 flag
def get_track_ac3(data) -> bool:
    assert('type' in data and data.get('type') == "Total")
    assert('result' in data)
    assert('track_ac3' in data.get('result'))
    return data.get('result').get('track_ac3')

# getter for constr_dependent_vars entries
def get_constr_dependent_vars(data) -> dict[str, list[int]]:
    assert('type' in data and data.get('type') == "Total")
    assert('children' in data)
    children = data.get('children', [])
    assert(len(children) > 0)
    assert('type' in children[0] and children[0].get('type') == "Initialize")
    assert('result' in children[0])
    assert('constr_dependent_vars' in children[0].get('result'))

    constr_dependent_vars = children[0].get('result').get('constr_dependent_vars')
    return constr_dependent_vars

# returns map of constraint id -> {"pairs_pruned": (map of # of pairs pruned -> list of durations), "num_vars_pruned": multimap of # of vars pruned at a time}
def gather_constr_prune_data(data):
    def traverse(self, node, prune_data):
        if node.get('type') == "AC3 Prune":
            assert('name' in node)
            assert('result' in node and 'vars_pruned' in node.get('result'))
            assert('duration_us' in node)
            assert('children' in node)

            name = node['name']
            result = node['result']
            duration = node['duration_us']
            vars_pruned = result['vars_pruned']

            constr_id = int(name)
            constr_data = prune_data.setdefault(constr_id, {
                "pairs_pruned": {},
                "num_vars_pruned": {}
            })

            # calculate and store pairs pruned
            total_pairs = sum(vars_pruned.values())
            constr_data["pairs_pruned"].setdefault(total_pairs, []).append(duration)

            # count variable pruning occurrences
            num_vars = len(vars_pruned)
            constr_data["num_vars_pruned"][num_vars] = constr_data["num_vars_pruned"].get(num_vars, 0) + 1

        for child in node.get('children', []):
            self(self, child, prune_data)

    prune_data = {}
    traverse(traverse, data, prune_data)
    return prune_data

# returns map of variable id -> (multimap of # of pairs pruned)
def gather_var_prune_data(data):
    constr_dependent_vars = get_constr_dependent_vars(data)

    def traverse(self, node, prune_data):
        if node.get('type') == "AC3 Prune":
            assert('name' in node)
            assert('result' in node and 'vars_pruned' in node.get('result'))
            assert('duration_us' in node)
            assert('children' in node)

            name = node['name']
            result = node['result']
            vars_pruned = result['vars_pruned']

            assert(name in constr_dependent_vars)
            vars = constr_dependent_vars[name]

            for var in vars:
                if not var in prune_data:
                    prune_data[var] = {}

                pairs_pruned = vars_pruned.get(str(var), 0)

                if not pairs_pruned in prune_data[var]:
                    prune_data[var][pairs_pruned] = 1
                else:
                    prune_data[var][pairs_pruned] += 1

        for child in node.get('children', []):
            self(self, child, prune_data)

    prune_data = {}
    traverse(traverse, data, prune_data)
    return prune_data

# get average microseconds per pair pruned
def get_avg_prune_duration(constr_prune_data):
    total_duration = 0
    total_pairs_pruned = 0

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            if pairs_pruned > 0:
                total_duration += sum(durations)
                total_pairs_pruned += pairs_pruned * len(durations)

    return total_duration / total_pairs_pruned

# perform a weighted linear regression
def weighted_linear_regression(x, y, weights=None):
    """
    Perform weighted linear regression and return full statistics.

    Args:
        x: Independent variable
        y: Dependent variable
        weights: Weights

    Returns:
        Dictionary with keys: 'slope', 'intercept', 'r_squared', 'p_value',
        'slope_se', 'residuals', 'fitted_values'
    """
    # Convert to numpy arrays
    x = np.asarray(x)
    y = np.asarray(y)
    weights = np.ones_like(x) if weights is None else np.asarray(weights)

    # Weighted least squares solution
    X = np.vstack([x, np.ones(len(x))]).T
    theta = np.linalg.lstsq(X * weights[:, None], y * weights, rcond=None)[0]
    slope, intercept = theta[0], theta[1]

    # Calculate predictions and residuals
    y_pred = intercept + slope * x
    residuals = y - y_pred

    # Degrees of freedom
    dof = len(x) - 2

    # Standard errors (from weighted covariance matrix)
    X_weighted = X * weights[:, None]
    cov_matrix = np.linalg.inv(X_weighted.T @ X_weighted)
    slope_se = np.sqrt(cov_matrix[0, 0])
    intercept_se = np.sqrt(cov_matrix[1, 1])

    # t-statistics and p-values
    t_stat_slope = slope / slope_se
    p_value_slope = 2 * stats.t.sf(np.abs(t_stat_slope), df=dof)

    # Weighted R-squared
    weighted_mean = np.average(y, weights=weights)
    ss_res = np.sum(weights * residuals**2)
    ss_tot = np.sum(weights * (y - weighted_mean)**2)
    r_squared = 1 - (ss_res / ss_tot)

    return {
        'slope': slope,
        'intercept': intercept,
        'slope_se': slope_se,
        'intercept_se': intercept_se,
        'r_squared': r_squared,
        'p_value': p_value_slope,
        't_stat': t_stat_slope,
        'dof': dof,
        'residuals': residuals,
        'fitted_values': y_pred,
        'weights': weights
    }

# plot average microseconds per prune vs # of pairs pruned in a single pass, and weighted least squares fit
def plot_avg_prune_durations(constr_prune_data, max_error=1.0, confidence=0.95):
    # map of # pairs pruned -> [list of all durations]
    all_durations = {}

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            all_durations.setdefault(pairs_pruned, []).extend(durations)

    # returns minimum number of samples given max acceptable uncertainty given a confidence level
    def get_min_samples(durations, max_error, confidence):
        if len(durations) < 2:
            return float('inf')  # Undefined for single samples

        mean = np.mean(durations)
        std = np.std(durations, ddof=1)
        coeff_var = std / mean
        t_crit = stats.t.ppf(1 - (1 - confidence)/2, df=len(durations) - 1)
        return int(np.ceil((t_crit * coeff_var / max_error)**2))

    # filter datapoints to those within a max uncertainty
    all_durations = {
        pairs_pruned: durations for pairs_pruned, durations in all_durations.items()
        if len(durations) >= get_min_samples(durations, max_error, confidence)
    }

    num_pairs = np.asarray(sorted(all_durations.keys()))
    avg_durations = np.asarray([np.mean(all_durations[p]) for p in num_pairs])

    sample_sizes = [len(d) for d in all_durations.values()]
    weights = np.sqrt(sample_sizes)
    results = weighted_linear_regression(num_pairs, avg_durations, weights=weights)

    plt.figure(figsize=(10, 6))

    plt.scatter(num_pairs, avg_durations, alpha=0.6, label='Average duration per distinct number of pairs pruned')

    plt.plot(num_pairs, results['intercept'] + results['slope']*num_pairs, 'r--',
             label=f'Weighted fit: y={results["slope"]:.1f}x + {results["intercept"]:.1f}\n'
                   f'R²={results["r_squared"]:.2f}, p={results["p_value"]:.2e}')

    plt.annotate(
        f"Excluded points with insufficient samples at {max_error*100:.0f}% error, {confidence*100:.0f}% CI)",
        xy=(0.98, 0.02),
        xycoords='axes fraction',
        ha='right',
        va='bottom',
        bbox=dict(boxstyle='round', pad=0.4, facecolor='white', alpha=0.8, edgecolor='0.8')
    )

    plt.title('Average AC-3 Pruning Durations vs Pairs Pruned')
    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Average Duration (μs)')
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.show()

# plot histogram of number of pairs pruned per pass
def plot_prune_size_freqs(constr_prune_data):
    num_pairs_pruned = []

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            num_pairs_pruned.extend([pairs_pruned] * len(durations))

    plt.figure(figsize=(10, 6))
    plt.yscale('log')

    plt.hist(num_pairs_pruned, bins=len(set(num_pairs_pruned)), edgecolor='black')

    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Frequency (log scale)')
    plt.title('Histogram of Pairs Pruned per Pass (Log Scale)')

    plt.show()

# run all child functions, return true iff ac3 pruning was tracked
def analyze_ac3_pruning(data) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping")
        return False

    constr_data = gather_constr_prune_data(data)
    var_data = gather_var_prune_data(data)

    print("Average time per pair pruned:", get_avg_prune_duration(constr_data), "us")

    plot_avg_prune_durations(constr_data)
    plot_prune_size_freqs(constr_data)

    return True