import numpy as np
import matplotlib.pyplot as plt

#################### plotting ####################

def plot_init_search_runtimes(output_dir, data):
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

    steps = np.asarray(['Init', 'Search'])
    durations = np.asarray([init_node.get('duration_us') * 1e-6, search_node.get('duration_us') * 1e-6])

    plt.figure(figsize=(10, 6))

    plt.bar(steps, durations, edgecolor='black')
    plt.xticks(steps)

    plt.title('Total Duration vs Execution Step')
    plt.xlabel('Execution Step')
    plt.ylabel('Total Duration (s)')

    plt.savefig(output_dir + 'init_search_runtimes.png', bbox_inches='tight')
    plt.show()

#################### parent function ####################

# run all child functions, returns true
def analyze_runtimes(data, output_dir) -> bool:
    plot_init_search_runtimes(output_dir, data)

    return True
