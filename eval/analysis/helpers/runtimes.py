import numpy as np
import matplotlib.pyplot as plt

from helpers.utils import get_track_ac3

#################### data collection ####################

# returns {"init": initialization time in sec, "search": solving time in sec}
def gather_init_search_runtime_data(data):
    assert('type' in data and data.get('type') == "Total")
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

# returns map of timestep name -> list of durations
def gather_ts_runtime_data(data):
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
    assert(data['duration_us'] == sum([sum(v) for v in result.values()]))

    return result

#################### plotting ####################

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

#################### parent function ####################

# run all child functions, return true iff ac3 pruning was tracked
def analyze_runtimes(data, output_dir) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping some analysis of runtimes")

    init_search_runtime_data = gather_init_search_runtime_data(data)
    ts_runtime_data = gather_ts_runtime_data(data) if get_track_ac3(data) else None

    plot_init_search_runtimes(output_dir, init_search_runtime_data)

    with open(output_dir + 'runtimes_metrics.md', 'w') as file:
        file.write("## Runtimes Metrics\n\n")

        file.write("### Runtime Breakdown\n")
        file.write("| Total (s) | Initialization (s) | Search (s) |\n")
        file.write("|-----------|--------------------|------------|\n")
        file.write(f"| {init_search_runtime_data['init']+init_search_runtime_data['search']:.3f} | {init_search_runtime_data['init']:.2f} | {init_search_runtime_data['search']:.2f} |\n")

        file.write('\n')

    return get_track_ac3(data)
