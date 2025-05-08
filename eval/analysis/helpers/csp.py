import numpy as np
import matplotlib.pyplot as plt

from helpers.utils import get_initialize_field

#################### data collection ####################

# returns map of variable len -> frequency
def gather_var_len_data(data):
    all_var_lens = {}

    var_lens = get_initialize_field(data, "var_lens")
    for _, length in var_lens.items():
        all_var_lens.setdefault(length, 0)
        all_var_lens[length] += 1

    return all_var_lens

# returns map of constr len -> frequency
def gather_constr_len_data(data):
    all_constr_lens = {}

    constr_lens = get_initialize_field(data, "constr_lens")
    for _, length in constr_lens.items():
        all_constr_lens.setdefault(length, 0)
        all_constr_lens[length] += 1

    return all_constr_lens

# returns list of variable domain sizes
def gather_dom_size_data(data):
    all_sizes = []

    var_domain_sizes = get_initialize_field(data, "var_domain_sizes")
    for _, size in var_domain_sizes.items():
        all_sizes.append(size)

    return all_sizes

#################### plotting ####################

# plot histogram of variable lengths
def plot_var_len_freq(output_dir, var_len_data):
    lengths = np.asarray(sorted(var_len_data.keys()))
    freqs = np.asarray([var_len_data[l] for l in lengths])
    percents = np.asarray([100 * var_len_data[l] / sum(var_len_data.values()) for l in lengths])

    plt.figure(figsize=(10, 6))

    bars = plt.bar(lengths, freqs, edgecolor='black')
    plt.bar_label(bars,
              labels=[f'{p:.2f}%\n({f} variables)' for p, f in zip(percents, freqs)],
              padding=3,
              fontsize=9)

    plt.ylim(0, 1.1 * max(freqs))
    plt.xticks(lengths)

    plt.title('Histogram of Variable Lengths')
    plt.xlabel('Variable Length')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'var_len_freq.png', bbox_inches='tight')

# plot histogram of constraint lengths
def plot_constr_len_freq(output_dir, constr_len_data):
    lengths = np.asarray(sorted(constr_len_data.keys()))
    freqs = np.asarray([constr_len_data[l] for l in lengths])
    percents = np.asarray([100 * constr_len_data[l] / sum(constr_len_data.values()) for l in lengths])

    plt.figure(figsize=(10, 6))

    bars = plt.bar(lengths, freqs, edgecolor='black')
    plt.bar_label(bars,
              labels=[f'{p:.2f}%\n({f} constraints)' for p, f in zip(percents, freqs)],
              padding=3,
              fontsize=9)

    plt.ylim(0, 1.1 * max(freqs))
    plt.xticks(lengths)

    plt.title('Histogram of Constraint Lengths')
    plt.xlabel('Constraint Length')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'constr_len_freq.png', bbox_inches='tight')

# plot histogram of variable domain sizes
def plot_dom_size_freq(output_dir, dom_size_data):
    plt.figure(figsize=(10, 6))

    plt.hist(dom_size_data, edgecolor='black')

    plt.title('Histogram of Variable Domain Sizes')
    plt.xlabel('Domain Size')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'dom_size_freq.png', bbox_inches='tight')

#################### parent function ####################

# run all child functions, return true
def analyze_csp(data, output_dir) -> bool:
    var_len_data = gather_var_len_data(data)
    constr_len_data = gather_constr_len_data(data)
    dom_size_data = gather_dom_size_data(data)

    plot_var_len_freq(output_dir, var_len_data)
    plot_constr_len_freq(output_dir, constr_len_data)
    plot_dom_size_freq(output_dir, dom_size_data)

    with open(output_dir + 'csp_metrics.md', 'w') as file:
        file.write("## CSP Metrics\n\n")

        file.write("### Variable lengths\n")
        file.write('| Length |')
        for length in sorted(var_len_data.keys()):
            file.write(' ' + str(length) + ' |')
        file.write('\n|' + ("---|" * (len(var_len_data.keys()) + 1)) + '\n')
        file.write('| Count |')
        for length in sorted(var_len_data.keys()):
            file.write(' ' + str(var_len_data[length]) + ' |')
        file.write('\n')

        file.write("### Constraint lengths\n")
        file.write('| Length |')
        for length in sorted(constr_len_data.keys()):
            file.write(' ' + str(length) + ' |')
        file.write('\n|' + ("---|" * (len(constr_len_data.keys()) + 1)) + '\n')
        file.write('| Count |')
        for length in sorted(constr_len_data.keys()):
            file.write(' ' + str(constr_len_data[length]) + ' |')
        file.write('\n')

        file.write("### Variable domain sizes\n")
        file.write("| Min | Max | Average | Median |\n")
        file.write("|-----|-----|---------|--------|\n")
        file.write(f"| {min(dom_size_data)} | {max(dom_size_data)} | {sum(dom_size_data)/len(dom_size_data):.2f} | {np.median(dom_size_data):.1f}|\n")

        file.write('\n')

    return True