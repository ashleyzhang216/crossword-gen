import numpy as np
import matplotlib.pyplot as plt

from helpers.utils import get_track_ac3

#################### data collection ####################

# returns {"init": initialization time in sec, "search": solving time in sec}
def gather_init_search_runtime_data(data):
    assert('type' in data and data.get('type') == "CSP")
    assert('children' in data)
    children = data.get('children', [])
    assert(len(children) >= 2)
    assert('type' in children[0] and children[0].get('type') == "Initialize")
    assert('type' in children[1] and children[1].get('type') == "Solve")

    init_node = children[0]
    search_node = children[1]
    assert('duration_us' in init_node)
    assert('duration_us' in search_node)

    return {
        "init": init_node.get('duration_us') * 1e-6,
        "search": search_node.get('duration_us') * 1e-6
    }

# returns map of span name -> list of durations
def gather_span_runtime_data(data):
    result = {}

    def traverse(node, result:dict) -> int:
        assert('type' in data)
        assert('duration_us' in data)

        total_child_duration = sum([traverse(child, result) for child in node.get('children', [])])
        assert(total_child_duration <= node['duration_us'])

        result.setdefault(node['type'], [])
        result[node['type']].append(node['duration_us'] - total_child_duration)
        return node['duration_us']

    total_duration = traverse(data, result)

    assert('duration_us' in data)
    assert(data['duration_us'] == total_duration)
    assert(abs(data['duration_us'] - sum([sum(v) for v in result.values()])) < 1)

    return result

#################### plotting ####################

# plot runtime distribution between initialization and search
def plot_init_search_runtimes(output_dir, init_search_runtime_data):
    steps = np.asarray(['Init', 'Search'])
    durations = np.asarray([init_search_runtime_data.get('init'), init_search_runtime_data.get('search')])

    plt.figure(figsize=(10, 6))

    plt.bar(steps, durations, edgecolor='black')
    plt.xticks(steps)

    plt.title('Total Duration vs Execution Step')
    plt.xlabel('Execution Step')
    plt.ylabel('Total Duration (s)')

    plt.savefig(output_dir + 'init_search_runtimes.png', bbox_inches='tight')
    plt.close()

# plot total runtime distribution by span type
def plot_total_runtime_by_span_type(output_dir, span_runtime_data):
    if span_runtime_data is None:
        return

    span_types = np.asarray(sorted(span_runtime_data, key=lambda k: sum(span_runtime_data[k]), reverse=True))
    durations_us = np.asarray([sum(span_runtime_data[t]) for t in span_types])
    durations_s = np.asarray([durations_us[i] * 1e-6 for i in range(len(span_types))])
    percents = np.asarray([100 * sum(span_runtime_data[t]) / sum(durations_us) for t in span_types])

    plt.figure(figsize=(10, 6))

    bars = plt.bar(span_types, durations_s, edgecolor='black')
    plt.bar_label(bars,
              labels=[f'{p:.2f}%\n({s:.2f} sec)\n' for p, s in zip(percents, durations_s)],
              padding=3,
              fontsize=9)

    plt.ylim(0, 1.2 * max(durations_s))
    plt.xticks(span_types)

    plt.annotate(
        "Total time for each span type\nexcluding total duration of children",
        xy=(0.98, 0.98),
        xycoords='axes fraction',
        ha='right',
        va='top',
        bbox=dict(boxstyle='round', pad=0.4, facecolor='white', alpha=0.8, edgecolor='0.8')
    )

    plt.title('Total Duration vs Span Type')
    plt.xlabel('Span Type')
    plt.ylabel('Total Duration (s)')

    plt.savefig(output_dir + 'total_runtime_by_span_type.png', bbox_inches='tight')
    plt.close()

#################### parent function ####################

# run all child functions, return true iff ac3 pruning was tracked
def analyze_runtimes(data, output_dir) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping some analysis of runtimes")

    init_search_runtime_data = gather_init_search_runtime_data(data)
    span_runtime_data = gather_span_runtime_data(data) if get_track_ac3(data) else None

    plot_init_search_runtimes(output_dir, init_search_runtime_data)
    plot_total_runtime_by_span_type(output_dir, span_runtime_data)

    with open(output_dir + 'runtimes_metrics.md', 'w') as file:
        file.write("## Runtimes Metrics\n\n")

        file.write("### Runtime breakdown\n")
        file.write("| Total (s) | Initialization (s) | Search (s) |\n")
        file.write("|-----------|--------------------|------------|\n")
        file.write(f"| {init_search_runtime_data['init']+init_search_runtime_data['search']:.3f} | {init_search_runtime_data['init']:.2f} | {init_search_runtime_data['search']:.2f} |\n")

        if get_track_ac3(data):
            file.write("### Span type durations (excluding children)\n")

            span_types = sorted(span_runtime_data, key=lambda k: sum(span_runtime_data[k]), reverse=True)
            file.write("| Span | Count | Total (s) | Min (μs) | Max (μs) | Average (μs) | Median (μs) |\n")
            file.write("|------|-------|-----------|----------|----------|--------------|-------------|\n")
            for t in span_types:
                d = span_runtime_data[t]
                assert(len(d) > 0)
                file.write(f"| {t} | {len(d)} | {sum(d)*1e-6:.2f} | {min(d):.2f} | {max(d):.2f} | {sum(d)/len(d):.2f} | {np.median(d):.2f} |\n")

        file.write('\n')

    return get_track_ac3(data)
