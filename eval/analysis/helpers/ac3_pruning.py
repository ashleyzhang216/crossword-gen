import numpy as np
from scipy import stats
import matplotlib.pyplot as plt

from helpers.utils import get_track_ac3, get_initialize_field, weighted_linear_regression

#################### data collection ####################

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

# returns map of constr len -> (map of # pairs pruned -> [list of all durations])
def gather_constr_len_prune_data(constr_prune_data, constr_lens):
    all_durations = {}

    for constr_id, data in constr_prune_data.items():
        constr_len = constr_lens.get(str(constr_id))
        all_durations.setdefault(constr_len, {})

        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            all_durations[constr_len].setdefault(pairs_pruned, []).extend(durations)

    return all_durations

# returns map of variable id -> (map of # of pairs pruned -> list of durations)
def gather_var_prune_data(data):
    constr_dependent_vars = get_initialize_field(data, "constr_dependent_vars")

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

            assert(name in constr_dependent_vars)
            vars = constr_dependent_vars[name]

            for var in vars:
                pairs_pruned = vars_pruned.get(str(var), 0)

                prune_data.setdefault(var, {})
                prune_data[var].setdefault(pairs_pruned, []).append(duration)

        for child in node.get('children', []):
            self(self, child, prune_data)

    prune_data = {}
    traverse(traverse, data, prune_data)
    return prune_data

# returns map of # of pairs pruned -> {"success": freq, "fail": freq}
def gather_ac3_prune_data(data):
    all_prunes = {}

    def traverse(self, node, prune_data):
        if node.get('type') == "AC3":
            total_pairs_pruned = 0
            for child in node.get('children', []):
                if child.get('type') == "AC3 Prune":
                    assert('result' in child and 'vars_pruned' in child.get('result'))

                    result = child['result']
                    vars_pruned = result['vars_pruned']
                    total_pairs_pruned += sum(vars_pruned.values())

            all_prunes.setdefault(total_pairs_pruned, {
                "success": 0,
                "fail": 0
            })

            assert('result' in node and 'success' in node.get('result'))
            success = node['result']['success']
            all_prunes[total_pairs_pruned]["success" if success else "fail"] += 1
        else:
            for child in node.get('children', []):
                self(self, child, prune_data)

    traverse(traverse, data, all_prunes)
    return all_prunes

# returns {"unique": map of # of unique constraints considered -> {"success": freq, "fail": freq}, (success is based on ac3 call)
#          "total":  map of total # of unique constraints considered -> {"success": freq, "fail": freq}}
def gather_ac3_constr_data(data):
    unique_constr_data = {}
    total_constr_data = {}

    def traverse(self, node, unique_constr_data, total_constr_data):
        if node.get('type') == "AC3":
            constr_ids_considered = []

            for child in node.get('children', []):
                if child.get('type') == "AC3 Prune":
                    assert('name' in child)
                    constr_id = child['name']
                    constr_ids_considered.append(constr_id)

            num_unique_prunes = len(set(constr_ids_considered))
            num_total_prunes = len(constr_ids_considered)

            unique_constr_data.setdefault(num_unique_prunes, {
                "success": 0,
                "fail": 0
            })
            total_constr_data.setdefault(num_total_prunes, {
                "success": 0,
                "fail": 0
            })

            assert('result' in node and 'success' in node.get('result'))
            success = node['result']['success']
            unique_constr_data[num_unique_prunes]["success" if success else "fail"] += 1
            total_constr_data[num_total_prunes]["success" if success else "fail"] += 1
        else:
            for child in node.get('children', []):
                self(self, child, unique_constr_data, total_constr_data)

    traverse(traverse, data, unique_constr_data, total_constr_data)
    return {
        "unique": unique_constr_data,
        "total": total_constr_data
    }

#################### data parsers ####################

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

# get average pairs pruned per ac3 call
def get_avg_pairs_pruned_per_ac3(ac3_prune_data):
    all_prunes = []

    for pairs_pruned, data in ac3_prune_data.items():
        all_prunes.extend([pairs_pruned] * (data['success'] + data['fail']))

    return sum(all_prunes) / len(all_prunes)

# get average num of unique prunes per ac3 call for all ac3 calls, successful ac3 calls, failed ac3 calls
def get_avg_unique_prunes_per_ac3(ac3_constr_data):
    num_prunes_success = []
    num_prunes_fail = []

    for num_prunes, data in ac3_constr_data['unique'].items():
        num_prunes_success.extend([num_prunes] * data['success'])
        num_prunes_fail.extend([num_prunes] * data['fail'])

    num_prunes_all = num_prunes_success + num_prunes_fail
    avg_prunes_all = sum(num_prunes_all)/len(num_prunes_all)
    avg_prunes_success = sum(num_prunes_success)/len(num_prunes_success)
    avg_prunes_fail = sum(num_prunes_fail)/len(num_prunes_fail) if len(num_prunes_fail) > 0 else 0
    return avg_prunes_all, avg_prunes_success, avg_prunes_fail

# get average num of total prunes per ac3 call for all ac3 calls, successful ac3 calls, failed ac3 calls
def get_avg_total_prunes_per_ac3(ac3_constr_data):
    num_prunes_success = []
    num_prunes_fail = []

    for num_prunes, data in ac3_constr_data['total'].items():
        num_prunes_success.extend([num_prunes] * data['success'])
        num_prunes_fail.extend([num_prunes] * data['fail'])

    num_prunes_all = num_prunes_success + num_prunes_fail
    avg_prunes_all = sum(num_prunes_all)/len(num_prunes_all)
    avg_prunes_success = sum(num_prunes_success)/len(num_prunes_success)
    avg_prunes_fail = sum(num_prunes_fail)/len(num_prunes_fail) if len(num_prunes_fail) > 0 else 0
    return avg_prunes_all, avg_prunes_success, avg_prunes_fail

# get num successful AC-3 prune calls, num failed AC-3 prune calls
def get_num_prunes_by_success(constr_prune_data):
    num_success = 0
    num_fail = 0

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            if pairs_pruned > 0:
                num_success += len(durations)
            else:
                num_fail += len(durations)

    return num_success, num_fail

# get total seconds of successful AC-3 prune calls, total seconds of failed AC-3 prune calls
def get_total_prune_durations(constr_prune_data):
    total_success_us = 0
    total_fail_us = 0

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            if pairs_pruned > 0:
                total_success_us += sum(durations)
            else:
                total_fail_us += sum(durations)

    return total_success_us * 1e-6, total_fail_us * 1e-6

#################### plotting ####################

# plot average microseconds per prune vs # of pairs pruned in a single pass, and weighted least squares fit
def plot_avg_pair_prune_durations(output_dir, constr_len_prune_data, filter=True, max_error=1.0, confidence=0.95):
    # returns minimum number of samples given max acceptable uncertainty given a confidence level
    def get_min_samples(durations, max_error, confidence):
        if len(durations) < 2:
            return float('inf')

        mean = np.mean(durations)
        std = np.std(durations, ddof=1)
        coeff_var = std / mean
        t_crit = stats.t.ppf(1 - (1 - confidence)/2, df=len(durations) - 1)
        return int(np.ceil((t_crit * coeff_var / max_error)**2))

    # filter datapoints to those within a max uncertainty
    if filter:
        for constr_len, data in constr_len_prune_data.items():
            constr_len_prune_data[constr_len] = {
                pairs_pruned: durations for pairs_pruned, durations in constr_len_prune_data[constr_len].items()
                if len(durations) >= get_min_samples(durations, max_error, confidence)
            }

    plt.figure(figsize=(10, 6))

    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    for i, constr_len in enumerate(sorted(constr_len_prune_data.keys())):
        num_pairs = np.asarray(sorted(constr_len_prune_data[constr_len].keys()))
        avg_durations = np.asarray([np.mean(constr_len_prune_data[constr_len][p]) for p in num_pairs])

        sample_sizes = [len(d) for d in constr_len_prune_data[constr_len].values()]
        weights = np.sqrt(sample_sizes)
        results = weighted_linear_regression(num_pairs, avg_durations, weights=weights)

        plt.scatter(num_pairs, avg_durations, alpha=0.6, color=colors[i % len(colors)], label=f'Length {constr_len} Constraints')

        plt.plot(num_pairs, results['intercept'] + results['slope']*num_pairs, '--',
                label=f'Length {constr_len} Weighted fit: y={results["slope"]:.1f}x + {results["intercept"]:.1f}\n'
                    f'R²={results["r_squared"]:.2f}, p={results["p_value"]:.2e}', color=colors[i % len(colors)])

    if filter:
        plt.annotate(
            f"Excluded points with insufficient samples at {max_error*100:.0f}% error, {confidence*100:.0f}% CI)",
            xy=(0.98, 0.02),
            xycoords='axes fraction',
            ha='right',
            va='bottom',
            bbox=dict(boxstyle='round', pad=0.4, facecolor='white', alpha=0.8, edgecolor='0.8')
        )

    plt.title('Average AC-3 Pruning Durations vs Num Pairs Pruned')
    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Average Duration (μs)')
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.savefig(output_dir + 'avg_pair_prune_durations.png', bbox_inches='tight')
    plt.close()

# plot histogram of microseconds per pair pruned
def plot_duration_per_pair_prune_freq(output_dir, constr_len_prune_data):
    plt.figure(figsize=(10, 6))
    plt.yscale('log')

    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    for i, constr_len in enumerate(sorted(constr_len_prune_data.keys())):
        times_per_prune = []
        for num_pairs, durations in constr_len_prune_data[constr_len].items():
            if not num_pairs == 0:
                times_per_prune.extend([duration / num_pairs for duration in durations])

        binwidth = 100
        bins = range(int(min(times_per_prune)) - binwidth, int(max(times_per_prune)) + binwidth, binwidth)

        plt.hist(times_per_prune, bins=bins, alpha=0.5, label=f'Length {constr_len} Constraints', color=colors[i % len(colors)], edgecolor='black')

    plt.title('Histogram of Durations per Pair Pruned During Successful Prunes (Log Scale)')
    plt.xlabel('Duration per Pair Pruned (μs)')
    plt.ylabel('Frequency (log scale)')

    plt.legend()

    plt.savefig(output_dir + 'duration_per_pair_prune_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of number of pairs pruned per ac3 prune
def plot_prune_size_freqs(output_dir, constr_prune_data):
    num_pairs_pruned = []

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            num_pairs_pruned.extend([pairs_pruned] * len(durations))

    plt.figure(figsize=(10, 6))
    plt.yscale('log')

    plt.hist(num_pairs_pruned, bins=len(set(num_pairs_pruned)), edgecolor='black')

    plt.title('Histogram of Pairs Pruned per Pass (Log Scale)')
    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Frequency (log scale)')

    plt.savefig(output_dir + 'prune_size_freqs.png', bbox_inches='tight')
    plt.close()

# plot histogram of num pairs pruned, separated by constraint lengths
def plot_pairs_pruned_freq_by_constr_len(output_dir, constr_prune_data, constr_lens):
    # map of constraint length -> list of number of pairs pruned
    all_pairs_pruned = {}

    min_pairs_pruned = 0
    max_pairs_pruned = 0
    for constr_id, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]
        constr_len = constr_lens.get(str(constr_id))

        for pairs_pruned, durations in pairs_data.items():
            all_pairs_pruned.setdefault(constr_len, []).extend([pairs_pruned] * len(durations))
            min_pairs_pruned = min(min_pairs_pruned, pairs_pruned)
            max_pairs_pruned = max(max_pairs_pruned, pairs_pruned)

    lengths = np.asarray(sorted(all_pairs_pruned.keys()))

    plt.figure(figsize=(10, 6))
    plt.yscale('log')

    binwidth = int(min(10, max((max_pairs_pruned-min_pairs_pruned)/64, 1)))
    bins = range(min_pairs_pruned, max_pairs_pruned + binwidth, binwidth)

    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    for i, l in enumerate(lengths):
        label = f'Length {l} Constraints ({sum(all_pairs_pruned[l])/len(all_pairs_pruned[l]):.3f} avg)'
        plt.hist(all_pairs_pruned[l], bins=bins, alpha=0.5, label=label, color=colors[i % len(colors)], edgecolor='black')

    plt.title('Histogram of Pairs Pruned per Pass by Constraint Length (Log Scale)')
    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Frequency (log scale)')
    plt.legend()

    plt.savefig(output_dir + 'pairs_pruned_freq_by_constr_len.png', bbox_inches='tight')
    plt.close()

# plot comparison of total pairs pruned per constraint length
def plot_total_pairs_pruned_by_constr_len(output_dir, constr_prune_data, constr_lens):
    # map of constraint length -> list of number of pairs pruned
    all_pairs_pruned = {}

    for constr_id, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]
        constr_len = constr_lens.get(str(constr_id))

        for pairs_pruned, durations in pairs_data.items():
            all_pairs_pruned.setdefault(constr_len, []).extend([pairs_pruned] * len(durations))

    lengths = np.asarray(sorted(all_pairs_pruned.keys()))
    total_pruned = np.asarray([sum(all_pairs_pruned[l]) for l in lengths])
    avg_pruned = np.asarray([sum(all_pairs_pruned[l])/len(all_pairs_pruned[l]) for l in lengths])
    percents_pruned = np.asarray([100 * sum(all_pairs_pruned[l]) / sum(total_pruned) for l in lengths])

    plt.figure(figsize=(10, 6))

    bars = plt.bar(lengths, total_pruned, edgecolor='black')
    plt.bar_label(bars,
              labels=[f'{p:.2f}%\n({t} pairs)\n(avg {a:.3f} pairs per prune)' for p, t, a in zip(percents_pruned, total_pruned, avg_pruned)],
              padding=3,
              fontsize=9)

    plt.ylim(0, 1.2 * max(total_pruned))
    plt.xticks(lengths)

    plt.title('Total Pairs Pruned vs Constraint Length')
    plt.xlabel('Constraint Lengths')
    plt.ylabel('Pairs Pruned')

    plt.savefig(output_dir + 'total_pairs_pruned_by_constr_len.png', bbox_inches='tight')
    plt.close()

# plot bar graph of total time spent by # of pairs pruned in a single pass
def plot_total_durations_by_num_pairs_pruned(output_dir, constr_prune_data):
    # map of {"success": list of durations, "fail": list of durations}
    all_durations = {}

    for _, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]

        for pairs_pruned, durations in pairs_data.items():
            all_durations.setdefault(pairs_pruned, []).extend(durations)

    num_pairs = np.asarray(sorted(all_durations.keys()))
    totals = np.asarray([sum(all_durations[n]) * 1e-6 for n in num_pairs])

    plt.figure(figsize=(10, 6))

    plt.bar(num_pairs, totals, edgecolor='black')

    plt.title('Total Duration of All Prunes vs Num Pairs Pruned')
    plt.xlabel('Number of Pairs Pruned in Pass')
    plt.ylabel('Total Duration of Prunes (s)')

    plt.savefig(output_dir + 'total_durations_by_num_pairs_pruned.png', bbox_inches='tight')
    plt.close()

# plot ratio of non-null AC-3 prunes vs var domain size
def plot_success_prune_ratio_vs_var_domain_size(output_dir, var_prune_data, var_domain_sizes):
    # map of var id -> {"empty": num prunes w/o effect, "nonempty": num prunes that removed 1+ pair}
    all_prunes = {}

    for var_id, data in var_prune_data.items():
        assert(str(var_id) in var_domain_sizes)
        all_prunes.setdefault(var_id, {
            "empty": 0,
            "nonempty": 0
        })

        for pairs_pruned, durations in data.items():
            if pairs_pruned == 0:
                all_prunes[var_id]["empty"] += len(durations)
            else:
                all_prunes[var_id]["nonempty"] += len(durations)

    var_ids = sorted(all_prunes.keys())
    dom_sizes = np.asarray([var_domain_sizes[str(var_id)] for var_id in var_ids])
    ratios = np.asarray([all_prunes[v]["nonempty"] / (all_prunes[v]["empty"] + all_prunes[v]["nonempty"]) for v in var_ids])

    plt.figure(figsize=(10, 6))

    plt.scatter(dom_sizes, ratios, alpha=0.6, label='Variable')

    plt.title('Ratio of Successful AC-3 Prunes vs Variable Domain Sizes')
    plt.xlabel('Domain Size')
    plt.ylabel('Ratio of Successful AC-3 Prunes')
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.savefig(output_dir + 'success_prune_ratio_vs_var_domain_size.png', bbox_inches='tight')
    plt.close()

# plot ratio of non-null AC-3 prunes vs var lengths
def plot_success_prune_ratio_vs_var_lens(output_dir, var_prune_data, var_lens):
    # map of var id -> {"empty": num prunes w/o effect, "nonempty": num prunes that removed 1+ pair}
    all_prunes = {}

    for var_id, data in var_prune_data.items():
        assert(str(var_id) in var_lens)
        all_prunes.setdefault(var_id, {
            "empty": 0,
            "nonempty": 0
        })

        for pairs_pruned, durations in data.items():
            if pairs_pruned == 0:
                all_prunes[var_id]["empty"] += len(durations)
            else:
                all_prunes[var_id]["nonempty"] += len(durations)

    var_ids = sorted(all_prunes.keys())
    var_lens = np.asarray([var_lens[str(var_id)] for var_id in var_ids])
    ratios = np.asarray([all_prunes[v]["nonempty"] / (all_prunes[v]["empty"] + all_prunes[v]["nonempty"]) for v in var_ids])

    plt.figure(figsize=(10, 6))

    plt.scatter(var_lens, ratios, alpha=0.6, label='Variable')

    plt.title('Ratio of Successful AC-3 Prunes vs Variable Lengths')
    plt.xlabel('Variable Length')
    plt.ylabel('Ratio of Successful AC-3 Prunes')
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.savefig(output_dir + 'success_prune_ratio_vs_var_lens.png', bbox_inches='tight')
    plt.close()

# plot ratio of non-null AC-3 prunes vs constraint lengths
def plot_success_prune_ratio_vs_constr_lens(output_dir, constr_prune_data, constr_lens):
    # map of constraint length -> {"success": freq, "fail": freq}
    all_calls = {}

    for constr_id, data in constr_prune_data.items():
        assert("pairs_pruned" in data)
        pairs_data = data["pairs_pruned"]
        constr_len = constr_lens.get(str(constr_id))
        all_calls.setdefault(constr_len, {
            'success': 0,
            'fail': 0
        })

        for pairs_pruned, durations in pairs_data.items():
            if pairs_pruned > 0:
                all_calls[constr_len]['success'] += len(durations)
            else:
                all_calls[constr_len]['fail'] += len(durations)

    lengths = np.asarray(sorted(all_calls.keys()))
    ratios = np.asarray([all_calls[l]['success']/(all_calls[l]['success']+all_calls[l]['fail']) for l in lengths])
    total_calls = np.asarray([all_calls[l]['success']+all_calls[l]['fail'] for l in lengths])

    plt.figure(figsize=(10, 6))

    bars = plt.bar(lengths, ratios, edgecolor='black')
    plt.bar_label(bars,
              labels=[f'{r:.4f}\n(out of {c} calls)' for r, c in zip(ratios, total_calls)],
              padding=3,
              fontsize=9)

    plt.xticks(lengths)
    plt.ylim(0, 1.1 * max(ratios))

    plt.title('Ratio of Successful AC-3 Prunes vs Constraint Length')
    plt.xlabel('Constraint Lengths')
    plt.ylabel('Ratio of Successful AC-3 Prunes')

    plt.savefig(output_dir + 'success_prune_ratio_vs_constr_lens.png', bbox_inches='tight')
    plt.close()

# plot histogram of number of pairs pruned per ac3 call
def plot_pairs_pruned_per_ac3_freq(output_dir, ac3_prune_data):
    all_prunes = []
    success_prunes = []
    fail_prunes = []

    for pairs_pruned, data in ac3_prune_data.items():
        all_prunes.extend([pairs_pruned] * (data['success'] + data['fail']))
        success_prunes.extend([pairs_pruned] * data['success'])
        fail_prunes.extend([pairs_pruned] * data['fail'])

    plt.figure(figsize=(10, 6))

    success_bins = min(len(set(success_prunes)), 40)
    fail_bins = min(len(set(success_prunes)), 40)
    plt.hist(success_prunes, bins=success_bins, alpha=0.5, label='Successful AC-3 Calls', color='green', edgecolor='black')
    plt.hist(fail_prunes, bins=fail_bins, alpha=0.5, label='Failed AC-3 Calls', color='red', edgecolor='black')

    plt.title('Histogram of Pairs Pruned per AC-3 Call')
    plt.xlabel('Number of Pairs Pruned in AC-3 Call')
    plt.ylabel('Frequency')
    plt.legend()

    plt.savefig(output_dir + 'pairs_pruned_per_ac3_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of number of unique prunes/constraints considered per ac3 call
def plot_num_unique_prunes_per_ac3_freq(output_dir, ac3_constr_data, num_constraints):
    num_prunes_success = []
    num_prunes_fail = []

    for num_prunes, data in ac3_constr_data['unique'].items():
        num_prunes_success.extend([num_prunes] * data['success'])
        num_prunes_fail.extend([num_prunes] * data['fail'])

    num_prunes_success = np.asarray(num_prunes_success)
    num_prunes_fail = np.asarray(num_prunes_fail)

    plt.figure(figsize=(10, 6))

    binwidth = int(num_constraints / 16)
    global_min = min(min(num_prunes_success), min(num_prunes_fail)) if len(num_prunes_fail) > 0 else min(num_prunes_success)
    global_max = max(max(num_prunes_success), max(num_prunes_fail)) if len(num_prunes_fail) > 0 else max(num_prunes_success)
    bins = range(global_min - binwidth, global_max + binwidth, binwidth)

    plt.hist(num_prunes_success, bins=bins, alpha=0.5, label='Successful AC-3 Calls', color='green', edgecolor='black')
    plt.hist(num_prunes_fail, bins=bins, alpha=0.5, label='Failed AC-3 Calls', color='red', edgecolor='black')

    plt.title('Histogram of Unique Prunes/Constraints Considered per AC-3 Call')
    plt.xlabel('Number of Unique Prunes in AC-3 Call')
    plt.ylabel('Frequency')
    plt.legend(title=f"Out of {num_constraints} constraints available")

    plt.savefig(output_dir + 'num_unique_prunes_per_ac3_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of total number of prunes/constraints considered per ac3 call
def plot_num_total_prunes_per_ac3_freq(output_dir, ac3_constr_data, num_constraints):
    num_prunes_success = []
    num_prunes_fail = []

    for num_prunes, data in ac3_constr_data['total'].items():
        num_prunes_success.extend([num_prunes] * data['success'])
        num_prunes_fail.extend([num_prunes] * data['fail'])

    num_prunes_success = np.asarray(num_prunes_success)
    num_prunes_fail = np.asarray(num_prunes_fail)

    plt.figure(figsize=(10, 6))

    binwidth = int(num_constraints / 16)
    global_min = min(min(num_prunes_success), min(num_prunes_fail)) if len(num_prunes_fail) > 0 else min(num_prunes_success)
    global_max = max(max(num_prunes_success), max(num_prunes_fail)) if len(num_prunes_fail) > 0 else max(num_prunes_success)
    bins = range(global_min, global_max + binwidth, binwidth)

    plt.hist(num_prunes_success, bins=bins, alpha=0.5, label='Successful AC-3 Calls', color='green', edgecolor='black')
    plt.hist(num_prunes_fail, bins=bins, alpha=0.5, label='Failed AC-3 Calls', color='red', edgecolor='black')

    plt.title('Histogram of Total Prunes/Constraints Considered per AC-3 Call')
    plt.xlabel('Number of Total Prunes in AC-3 Call')
    plt.ylabel('Frequency')
    plt.legend(title=f"Out of {num_constraints} constraints available")

    plt.savefig(output_dir + 'num_total_prunes_per_ac3_freq.png', bbox_inches='tight')
    plt.close()

#################### parent function ####################

# run all child functions, return true iff ac3 pruning was tracked
def analyze_ac3_pruning(data, output_dir) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping analysis of AC-3 pruning")
        return False

    constr_data = gather_constr_prune_data(data)
    var_data = gather_var_prune_data(data)
    constr_len_prune_data = gather_constr_len_prune_data(constr_data, get_initialize_field(data, "constr_lens"))
    ac3_prune_data = gather_ac3_prune_data(data)
    ac3_constr_data = gather_ac3_constr_data(data)

    plot_avg_pair_prune_durations(output_dir, constr_len_prune_data)
    plot_duration_per_pair_prune_freq(output_dir, constr_len_prune_data)
    plot_prune_size_freqs(output_dir, constr_data)
    plot_pairs_pruned_freq_by_constr_len(output_dir, constr_data, get_initialize_field(data, "constr_lens"))
    plot_total_pairs_pruned_by_constr_len(output_dir, constr_data, get_initialize_field(data, "constr_lens"))
    plot_total_durations_by_num_pairs_pruned(output_dir, constr_data)

    plot_success_prune_ratio_vs_var_domain_size(output_dir, var_data, get_initialize_field(data, "var_domain_sizes"))
    plot_success_prune_ratio_vs_var_lens(output_dir, var_data, get_initialize_field(data, "var_lens"))

    plot_success_prune_ratio_vs_constr_lens(output_dir, constr_data, get_initialize_field(data, "constr_lens"))

    plot_pairs_pruned_per_ac3_freq(output_dir, ac3_prune_data)

    plot_num_unique_prunes_per_ac3_freq(output_dir, ac3_constr_data, len(get_initialize_field(data, "constr_lens").keys()))
    plot_num_total_prunes_per_ac3_freq(output_dir, ac3_constr_data, len(get_initialize_field(data, "constr_lens").keys()))

    with open(output_dir + 'ac3_pruning_metrics.md', 'w') as file:
        file.write("## AC-3 Pruning Metrics\n\n")

        file.write("### Average time per pair pruned\n")
        file.write(f'{get_avg_prune_duration(constr_data):.2f} us\n')

        num_success, num_fail = get_num_prunes_by_success(constr_data)
        file.write("### Total number of prune calls\n")
        file.write(f'{num_success+num_fail} ({100*num_success/(num_success + num_fail):.2f}% successful)\n')

        time_success, time_fail = get_total_prune_durations(constr_data)
        file.write("### Total time spent on prune calls\n")
        file.write(f'{time_success+time_fail:.2f} sec ({100*time_success/(time_success + time_fail):.2f}% successful)\n')

        file.write("### Average number of pairs pruned per AC-3 call\n")
        file.write(f'{get_avg_pairs_pruned_per_ac3(ac3_prune_data):.2f} pairs\n')

        avg_unique_prunes_all, avg_unique_prunes_success, avg_unique_prunes_fail = get_avg_unique_prunes_per_ac3(ac3_constr_data)
        avg_total_prunes_all, avg_total_prunes_success, avg_total_prunes_fail = get_avg_total_prunes_per_ac3(ac3_constr_data)

        file.write("### Average number of prune calls per AC-3 call\n")
        file.write("| Prune Calls | All | Successful AC-3 | Failed AC-3|\n")
        file.write("|-------------|-----|-----------------|------------|\n")
        file.write(f"| Unique | {avg_unique_prunes_all:.2f} | {avg_unique_prunes_success:.2f} | {avg_unique_prunes_fail:.2f} |\n")
        file.write(f"| Total | {avg_total_prunes_all:.2f} | {avg_total_prunes_success:.2f} | {avg_total_prunes_fail:.2f} |\n")

        file.write('\n')

    return True
