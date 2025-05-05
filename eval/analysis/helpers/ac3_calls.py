import numpy as np
import matplotlib.pyplot as plt

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

#################### parent function ####################

# run all child functions, return true
def analyze_ac3_calls(data) -> bool:
    ac3_success_data = gather_ac3_success_data(data)

    num_success, num_fail = len(ac3_success_data['success']), len(ac3_success_data['fail'])
    print("Total number of AC-3 calls:", f'{num_success+num_fail} ({100*num_success/(num_success + num_fail):.2f}% successful)')

    return True