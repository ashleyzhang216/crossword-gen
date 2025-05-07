import numpy as np
from scipy import stats

#################### data collection ####################

# getter for track_ac3 flag
def get_track_ac3(data) -> bool:
    assert('type' in data and data.get('type') == "Total")
    assert('result' in data)
    assert('track_ac3' in data.get('result'))
    return data.get('result').get('track_ac3')

# getter for subfields in the result field of the initialize node
def get_initialize_field(data, field_name):
    assert('type' in data and data.get('type') == "Total")
    assert('children' in data)
    children = data.get('children', [])
    assert(len(children) > 0)
    assert('type' in children[0] and children[0].get('type') == "Initialize")
    assert('result' in children[0])
    assert(field_name in children[0].get('result'))

    field = children[0].get('result').get(field_name)
    return field

#################### data analysis ####################

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
