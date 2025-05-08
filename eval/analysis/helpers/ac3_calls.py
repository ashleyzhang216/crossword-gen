import numpy as np
import matplotlib.pyplot as plt

from helpers.utils import get_track_ac3

#################### data collection ####################

# returns map of {"success": list of durations, "fail": list of durations}
def gather_ac3_success_data(data):
    all_calls = {
        'success': [],
        'fail': []
    }

    def traverse(self, node, all_calls):
        if node.get('type') == "AC3":
            assert('result' in node and 'success' in node.get('result'))
            assert('duration_us' in node)
            success = node['result']['success']
            duration = node['duration_us']
            all_calls["success" if success else "fail"].append(duration)
        else:
            for child in node.get('children', []):
                self(self, child, all_calls)

    traverse(traverse, data, all_calls)
    return all_calls

#################### plotting ####################

# plot histogram of ac3 call durations, separated by success/fail
def plot_ac3_call_duration_freq(output_dir, ac3_success_data):
    plt.figure(figsize=(10, 6))

    binwidth = 2500
    global_min = min(min(ac3_success_data['success']), min(ac3_success_data['fail'])) if len(ac3_success_data['fail']) > 0 else min(ac3_success_data['success'])
    global_max = max(max(ac3_success_data['success']), max(ac3_success_data['fail'])) if len(ac3_success_data['fail']) > 0 else max(ac3_success_data['success'])
    bins = range(global_min - binwidth, global_max + binwidth, binwidth)

    plt.hist(ac3_success_data['success'], bins=bins, alpha=0.5, label='Successful AC-3 Calls', color='green', edgecolor='black')
    plt.hist(ac3_success_data['fail'], bins=bins, alpha=0.5, label='Failed AC-3 Calls', color='red', edgecolor='black')

    plt.title('Histogram of AC-3 Call Durations')
    plt.xlabel('AC-3 Call Duration (μs)')
    plt.ylabel('Frequency')
    plt.legend()

    plt.savefig(output_dir + 'ac3_call_duration_freq.png', bbox_inches='tight')
    plt.show()

#################### parent function ####################

# run all child functions, return true iff ac3 pruning was tracked
def analyze_ac3_calls(data, output_dir) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping analyze_ac3_calls()")
        return False

    ac3_success_data = gather_ac3_success_data(data)

    plot_ac3_call_duration_freq(output_dir, ac3_success_data)

    with open(output_dir + 'ac3_calls_metrics.md', 'w') as file:
        file.write("## AC-3 Calls Metrics\n\n")

        num_success, num_fail = len(ac3_success_data['success']), len(ac3_success_data['fail'])
        print("Total number of AC-3 calls:", f'{num_success+num_fail} ({100*num_success/(num_success + num_fail):.2f}% successful)')
        file.write("### Total number of AC-3 calls\n")
        file.write(f'{num_success+num_fail} ({100*num_success/(num_success + num_fail):.2f}% successful)\n')

        file.write('\n')

    return True