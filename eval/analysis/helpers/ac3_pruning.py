import json
import matplotlib.pyplot as plt

def analyze_ac3_pruning_durations(data, img_filepath):
    def traverse(self, node, durations):
        if node.get('type') == "AC3 Prune":
            assert('duration_us' in node)
            assert('result' in node)

            if node['result']['constr_len'] == 4:
                num_vars_pruned = len(node['result']['vars_pruned'])
                if num_vars_pruned in durations:
                    durations[num_vars_pruned].append(node['duration_us'])
                else:
                    durations[num_vars_pruned] = [node['duration_us']]

        for child in node.get('children', []):
            self(self, child, durations)

    durations = {}
    traverse(traverse, data, durations)

    plt.figure(figsize=(10, 6))

    x = [int(key) for key in durations]
    y = [sum(durations[key]) / len(durations[key]) for key in durations]
    plt.scatter(x, y)

    # colors = ["purple", "red", "blue", "yellow", "green"]
    # for key in durations:
    #     if int(key) > 0:
    #         plt.hist(durations[key], alpha=0.5, label=str(key) + " Vars Pruned", color=colors[int(key)])
    

    plt.title('Avg Pruning Duration vs # Vars Pruned')
    # plt.xlabel('Values')
    # plt.ylabel('Frequency')
    # plt.legend()
    plt.show()

# getter for track_ac3 flag
def get_track_ac3(data) -> bool:
    assert('type' in data and data.get('type') == "Total")
    assert('result' in data)
    assert('track_ac3' in data.get('result'))
    return data.get('result').get('track_ac3')

# getter for constr_dependent_vars entry
def get_dependent_vars(data, constr_id) -> list[int]:
    assert('type' in data and data.get('type') == "Total")
    assert('children' in data)
    children = data.get('children', [])
    assert(len(children) > 0)
    assert('type' in children[0] and children[0].get('type') == "Initialize")
    assert('result' in children[0])
    assert('constr_dependent_vars' in children[0].get('result'))

    constr_dependent_vars = children[0].get('result').get('constr_dependent_vars')
    assert(str(constr_id) in constr_dependent_vars)
    return constr_dependent_vars.get(str(constr_id))

# run all child functions, return true iff ac3 pruning was tracked
def analyze_ac3_pruning(data) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping")
        return False

    return True