import numpy as np
import matplotlib.pyplot as plt

from helpers.utils import get_initialize_field

#################### data collection ####################

# returns list of variable lengths
def get_var_len_data(data):
    all_var_lens = []

    var_lens = get_initialize_field(data, "var_lens")
    for _, length in var_lens.items():
        all_var_lens.append(length)

    return all_var_lens

#################### plotting ####################

# plot histogram of variable lengths
def plot_var_size_freq(output_dir, var_len_data):
    plt.figure(figsize=(10, 6))

    plt.hist(var_len_data, bins=len(set(var_len_data)), label='Variables', edgecolor='black')
    plt.xticks(sorted(list(set(var_len_data))))

    plt.title('Histogram of Variable Lengths')
    plt.xlabel('Variable Length')
    plt.ylabel('Frequency')
    plt.legend(title=f"{len(var_len_data)} total variables")

    plt.savefig(output_dir + 'var_size_freq.png', bbox_inches='tight')

#################### parent function ####################

# run all child functions, return true
def analyze_csp(data, output_dir) -> bool:
    var_len_data = get_var_len_data(data)

    plot_var_size_freq(output_dir, var_len_data)

    with open(output_dir + 'csp_metrics.md', 'w') as file:
        file.write("## CSP Metrics\n\n")

        file.write("### Variable Lengths\n")
        file.write("| Count | Min | Max | Average | Median |\n")
        file.write("|-------|-----|-----|---------|--------|\n")
        file.write(f"| {len(var_len_data)} | {min(var_len_data)} | {max(var_len_data)} | {sum(var_len_data)/len(var_len_data):.2f} | {np.median(var_len_data):.1f}|\n")

        file.write('\n')

    return True