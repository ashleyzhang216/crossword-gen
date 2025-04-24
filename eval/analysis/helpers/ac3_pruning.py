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
    
