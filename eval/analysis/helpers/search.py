import numpy as np
import matplotlib.pyplot as plt
from enum import Enum
from graphviz import Digraph
from typing import Callable

from helpers.utils import get_track_ac3

#################### data collection ####################

class SearchReason(Enum):
    RECURSIVE = "recursive"
    AC3 = "ac3"
    SOLVED = "solved"
    DUPLICATE = "duplicate"

    @classmethod
    def from_string(cls, value):
        try:
            return cls(value.lower())
        except ValueError:
            raise ValueError(f"{value} is not a valid SearchReason")

class Color(Enum):
    GREEN = "#d5e8d4"
    YELLOW = "#fff2cc"
    ORANGE = "#f5d6b0"
    RED = "#f8cecc"

class SearchNode:
    def __init__(self, success:bool, reason:SearchReason, word=None, parent=None):
        self.success = success
        self.reason = reason
        self.word = word
        self.parent = parent
        self.children = []

        self.failed_subtree_size = None # number of nodes with success=False in this subtree, including itself
        self.num_constrs_checked = None # number of constraints checked during AC-3 for this specific node (not recursive)
        self.num_pairs_pruned = None # number of prunes during AC-3 for this specific node (not recursive)
        self.jump_height = None # num nodes backtracked/backjumped after evaluating all children
        self.var_of_children = None # variable that child nodes are trying to assign a word to (not this node)

    def add_child(self, success:bool, reason:SearchReason, word:str):
        child = SearchNode(success, reason, word=word, parent=self)
        self.children.append(child)
        return child

    def set_failed_subtree_size(self):
        assert(self.failed_subtree_size is None)

        self.failed_subtree_size = 0 if self.success else 1
        for child in self.children:
            child.set_failed_subtree_size()
            assert(not child.failed_subtree_size is None)
            self.failed_subtree_size += child.failed_subtree_size

    def set_num_constrs(self, num_constrs_checked, num_pairs_pruned):
        assert(self.num_constrs_checked is None)
        assert(self.num_pairs_pruned is None)
        self.num_constrs_checked = num_constrs_checked
        self.num_pairs_pruned = num_pairs_pruned

    def set_jump_height(self, jump_height):
        assert(self.jump_height is None)
        if self.success:
            assert(jump_height is None)
        self.jump_height = jump_height

    def set_var_of_children(self, var_of_children:int):
        assert(not var_of_children is None)
        self.var_of_children = var_of_children

    def set_reason_solved(self):
        assert(not self.reason is None)
        assert(not self.reason is SearchReason.SOLVED)
        self.reason = SearchReason.SOLVED

    def get_node_jumped_to(self):
        assert(not self.jump_height is None)
        assert(self.jump_height > 0)
        current = self
        for i in range(self.jump_height):
            current = current.parent
            if not i == self.jump_height - 1:
                assert(current.jump_height is None)
        return current

    def get_var(self):
        if self.parent is None:
            return None
        return self.parent.var_of_children

    def get_color(self) -> str:
        assert(not self.reason is None)
        if self.success:
            assert(self.reason == SearchReason.SOLVED or self.reason == SearchReason.RECURSIVE)
            return Color.GREEN.value
        else:
            match self.reason:
                case SearchReason.RECURSIVE:
                    return Color.YELLOW.value
                case SearchReason.AC3:
                    return Color.RED.value
                case SearchReason.DUPLICATE:
                    return Color.ORANGE.value
                case _:
                    raise ValueError(f"Node with success={self.success} and reason={self.reason.value} has no valid color")

    # returns map of success bool to (map of SearchReason to freq)
    def gather_reason_data(self, result:dict=None):
        if result is None:
            data = {
                True: {},
                False: {}
            }
            data[self.success].setdefault(self.reason, 0)
            data[self.success][self.reason] += 1

            for child in self.children:
                child.gather_reason_data(data)

            return data
        else:
            result[self.success].setdefault(self.reason, 0)
            result[self.success][self.reason] += 1

            for child in self.children:
                child.gather_reason_data(result)

    # returns map of jump height -> freq
    def gather_jump_height_data(self, result:dict=None):
        if result is None:
            data = {}
            if not self.jump_height is None:
                data.setdefault(self.jump_height, 0)
                data[self.jump_height] += 1

            for child in self.children:
                child.gather_jump_height_data(data)

            return data
        else:
            if not self.jump_height is None:
                result.setdefault(self.jump_height, 0)
                result[self.jump_height] += 1

            for child in self.children:
                child.gather_jump_height_data(result)

    # returns list of exclusive dead end subtree sizes (DESS), i.e. hierarchical counting, avoiding nested subtrees
    def gather_exclusive_dess_data(self, result:list[int]=None):
        if result is None:
            if self.success:
                data = []
                assert(self.failed_subtree_size == sum([c.failed_subtree_size for c in self.children]))
                for child in self.children:
                    child.gather_exclusive_dess_data(data)

                return data
            else:
                assert(self.failed_subtree_size > 0)
                return [self.failed_subtree_size]
        else:
            if self.success:
                assert(self.failed_subtree_size == sum([c.failed_subtree_size for c in self.children]))
                for child in self.children:
                    child.gather_exclusive_dess_data(result)
            else:
                assert(self.failed_subtree_size > 0)
                result.append(self.failed_subtree_size)

    # returns list of exclusive constraints checked per dead end (CCDE), i.e. hierarchical counting, avoiding nested subtrees
    def gather_exclusive_ccde_data(self, result:list[int]=None):
        def gather_recursive_ccde(node:SearchNode):
            assert(not node.success)
            assert(not node.num_constrs_checked is None)
            total_constrs_checked = node.num_constrs_checked
            for child in node.children:
                total_constrs_checked += gather_recursive_ccde(child)
            return total_constrs_checked

        if result is None:
            if self.success:
                data = []
                for child in self.children:
                    child.gather_exclusive_ccde_data(data)
                return data
            else:
                total_constrs_checked = gather_recursive_ccde(self)
                assert(total_constrs_checked > 0 or self.reason == SearchReason.DUPLICATE)
                return [total_constrs_checked]
        else:
            if self.success:
                for child in self.children:
                    child.gather_exclusive_ccde_data(result)
            else:
                total_constrs_checked = gather_recursive_ccde(self)
                assert(total_constrs_checked > 0 or self.reason == SearchReason.DUPLICATE)
                result.append(total_constrs_checked)

    # returns list of exclusive pairs pruned per dead end (PPDE), i.e. hierarchical counting, avoiding nested subtrees
    def gather_exclusive_ppde_data(self, result:list[int]=None):
        def gather_recursive_ppde(node:SearchNode):
            assert(not node.success)
            assert(not node.num_pairs_pruned is None)
            total_pairs_pruned = node.num_pairs_pruned
            for child in node.children:
                total_pairs_pruned += gather_recursive_ppde(child)
            return total_pairs_pruned

        if result is None:
            if self.success:
                data = []
                for child in self.children:
                    child.gather_exclusive_ppde_data(data)
                return data
            else:
                total_pairs_pruned = gather_recursive_ppde(self)
                assert(total_pairs_pruned > 0 or self.reason == SearchReason.DUPLICATE)
                return [total_pairs_pruned]
        else:
            if self.success:
                for child in self.children:
                    child.gather_exclusive_ppde_data(result)
            else:
                total_pairs_pruned = gather_recursive_ppde(self)
                assert(total_pairs_pruned > 0 or self.reason == SearchReason.DUPLICATE)
                result.append(total_pairs_pruned)

    # returns two list of number of nodes at each depth: {"no_leaves": without leaves, "leaves": with leaves}
    def gather_node_depth_data(self):
        # gets list of number of nodes at each depth
        def traverse(node:SearchNode, sizes:list[int], depth:int, exclude_leaves:bool=False):
            if len(sizes) <= depth:
                assert(len(sizes) == depth)
                sizes.append(0)

            if exclude_leaves and len(node.children) == 0:
                return

            sizes[depth] += 1
            for child in node.children:
                traverse(child, sizes, depth + 1, exclude_leaves)

        no_leaves_sizes = []
        traverse(self, no_leaves_sizes, 0, exclude_leaves=True)

        leaves_sizes = []
        traverse(self, leaves_sizes, 0, exclude_leaves=False)

        data = {
            "no_leaves": no_leaves_sizes,
            "leaves": leaves_sizes
        }
        assert(len(data['no_leaves']) == len(data['leaves']))
        return data

    # returns num solutions, depth of solutions or largest node depth if no solution exists
    def get_solution_depths(self):
        if self.reason == SearchReason.SOLVED:
            assert(self.success)
            assert(len(self.children) == 0)
            return 1, 0

        total_solutions = 0
        max_depth = 0
        for child in self.children:
            num_sols, depth = child.get_solution_depths()
            depth += 1

            if total_solutions > 0:
                if num_sols > 0:
                    assert(max_depth == depth)
                    total_solutions += num_sols
                else:
                    assert(max_depth >= depth)
            else:
                if num_sols > 0:
                    assert(depth >= max_depth)
                    total_solutions += num_sols
                    max_depth = depth
                else:
                    max_depth = max(max_depth, depth)

        return total_solutions, max_depth

# returns tree of SearchNode
def gather_search_tree(data) -> SearchNode:
    assert('type' in data and data['type'] == "CSP")
    assert('children' in data)
    children = data['children']
    assert(len(children) >= 2)
    assert('type' in children[0] and children[0]['type'] == "Initialize")
    assert('type' in children[1] and children[1]['type'] == "Solve")
    search_root = children[1]
    assert('result' in search_root and 'success' in search_root['result'])

    tree = SearchNode(search_root['result']['success'], SearchReason(search_root['result']['reason']))
    def traverse(search_node:SearchNode, data_node):
        if data_node['type'] == "Try Assign":
            node_child = search_node.add_child(data_node['result']['success'], SearchReason(data_node['result']['reason']), data_node['result']['word'])

            for child_data_node in data_node.get('children', []):
                traverse(node_child, child_data_node)

            # compensate for leaf nodes in a dead end currently not being annotated with jump_height due to no search step underneath
            if len(node_child.children) == 0 and not node_child.success:
                assert(node_child.reason == SearchReason.AC3 or node_child.reason == SearchReason.DUPLICATE)
                assert(node_child.jump_height is None)
                node_child.set_jump_height(1)

            # annotate duplicate nodes with prune data since no AC-3 was performed underneath
            if node_child.reason == SearchReason.DUPLICATE:
                assert(len(node_child.children) == 0)
                assert(not node_child.success)
                node_child.set_num_constrs(0, 0)

        elif data_node['type'] == "AC3":
            num_pairs_pruned = 0
            for child_data_node in data_node['children']:
                assert(child_data_node['type'] == "AC3 Prune")
                assert(len(child_data_node['children']) == 0)
                num_pairs_pruned += sum(child_data_node['result']['vars_pruned'].values())

            search_node.set_num_constrs(len(data_node.get('children', [])), num_pairs_pruned)
        else:
            if data_node['type'] == "Search Step":
                # temporarily enforce only backtracking since this is the only thing tested so far
                # TODO: remove this once other methods are supported
                assert(data_node['name'] == "Backtracking")

                if data_node['result']['success'] and data_node['result']['reason'] == "solved":
                    search_node.set_reason_solved()
                    assert(len(data_node.get('children', [])) == 0)

                search_node.set_jump_height(data_node['result']['jump_height'])
                if not data_node['result']['variable'] is None:
                    search_node.set_var_of_children(data_node['result']['variable']['id'])

            for child_data_node in data_node.get('children', []):
                traverse(search_node, child_data_node)

    def verify_jump_heights(root:SearchNode):
        def verify_node(node:SearchNode):
            # node that failed and isn't root
            if not node.parent is None and not node.success:
                assert(not node.jump_height is None)
                current = node
                for i in range(node.jump_height):
                    current = current.parent
                    assert(not current is None)
                    if not i == node.jump_height - 1:
                        assert(current.jump_height is None)
                verify_node(current)
            elif node.success:
                assert(node.jump_height is None)

        def call_func_from_leaves(node:SearchNode, func:Callable[[SearchNode], None]):
            if len(node.children) == 0:
                func(node)
            else:
                for child in node.children:
                    call_func_from_leaves(child, func)

        call_func_from_leaves(root, verify_node)

    traverse(tree, data)
    tree.set_failed_subtree_size()
    verify_jump_heights(tree)

    return tree

#################### data parsers ####################

# returns two lists of tree size at each max depth: {"no_leaves": without leaves, "leaves": with leaves}
def get_tree_size_data(node_depth_data):
    no_leaves_sizes = node_depth_data['no_leaves']
    leaves_sizes = node_depth_data['leaves']

    data = {
        "no_leaves": [sum(no_leaves_sizes[0:i]) for i in range(len(no_leaves_sizes))],
        "leaves": [sum(leaves_sizes[0:i+1]) for i in range(len(leaves_sizes))]
    }
    assert(len(data['no_leaves']) == len(data['leaves']))

    return data

# returns map of max depth -> effective branching factor (EBF)
def get_ebf_data(search_tree:SearchNode, tree_size_data):
    def compute_ebf(N, d, epsilon=1e-6, max_iter=1000):
        if N == 1:
            return 0.0
        if d == 0:
            return float('inf')

        low, high = 1.0, float(N)

        for _ in range(max_iter):
            mid = (low + high) / 2
            if mid == 1.0:
                estimated = d + 1
            else:
                estimated = (mid**(d + 1) - 1) / (mid - 1)

            if abs(estimated - N) < epsilon:
                return mid
            elif estimated < N:
                low = mid
            else:
                high = mid

        return (low + high) / 2

    ebf_data = {}
    num_solutions, max_depth = search_tree.get_solution_depths()
    assert(len(tree_size_data['no_leaves']) == len(tree_size_data['leaves']))
    assert(max_depth == len(tree_size_data['no_leaves']) - 1)

    for i in range(1, len(tree_size_data['leaves'])):
        ebf_data[i] = compute_ebf(tree_size_data['leaves'][i] / (num_solutions if num_solutions > 0 else 1), i)

    return ebf_data

# returns map of max depth -> average branching factor (ABF)
def get_abf_data(tree_size_data):
    abf_data = {}
    assert(len(tree_size_data['no_leaves']) == len(tree_size_data['leaves']))

    for i in range(1, len(tree_size_data['leaves'])):
        abf_data[i] = (tree_size_data['leaves'][i] - 1) / tree_size_data['no_leaves'][i]

    return abf_data

#################### plotting ####################

# draw search tree
def plot_search_tree(output_dir, search_tree:SearchNode):
    dot = Digraph(comment='Search Tree')
    dot.attr('node', style='filled')

    def add_nodes_and_record_edges(node:SearchNode, down_edges:set, up_edges:set):
        if node.get_var() is None:
            dot.node(str(id(node)), "root", fontname="bold", color=node.get_color())
        else:
            dot.node(str(id(node)), f"{node.get_var()}: {node.word}", color=node.get_color())

        for i, child in enumerate(node.children):
            # forward edge to child
            assert(not (str(id(node)), str(id(child))) in down_edges)
            down_edges.add((str(id(node)), str(id(child))))

            # backjump edge from child
            if not child.jump_height is None:
                if child.jump_height > 1:
                    assert(i == len(node.children) - 1)

                # upwards edge from child
                assert(not (str(id(child)), str(id(child.get_node_jumped_to()))) in up_edges)
                up_edges.add((str(id(child)), str(id(child.get_node_jumped_to()))))

            add_nodes_and_record_edges(child, down_edges, up_edges)

    down_edges = set()
    up_edges = set()
    add_nodes_and_record_edges(search_tree, down_edges, up_edges)

    # add downwards edges
    for (lhs, rhs) in down_edges:
        if (rhs, lhs) in up_edges:
            # bidirectional case
            dot.edge(lhs, rhs, dir='both')
            up_edges.remove((rhs, lhs))
        else:
            dot.edge(lhs, rhs)

    # leftover up edges must be backjumped edges
    for (rhs, lhs) in up_edges:
        dot.edge(lhs, rhs, color='red')

    # add legend
    with dot.subgraph(name='cluster_legend') as legend:
        legend.attr(label='Legend', style='rounded',
                   fontname='Helvetica', fontsize='12')

        legend.node('legend_label', 'Variable ID: Word', fillcolor='white', style='filled')

        colors = [
            [Color.GREEN, "Success"],
            [Color.YELLOW, "Fail (recursive)"],
            [Color.ORANGE, "Fail (duplicate)"],
            [Color.RED, "Fail (AC-3)"]
        ]
        for i, [color, label] in enumerate(colors):
            legend.node(
                f'legend_{i}',
                label,
                fillcolor=color.value,
                style='filled',
                penwidth='0'
            )
            legend.edge('legend_label', f'legend_{i}', style='invis', minlen='1')

    # place legend over root node
    dot.edge('legend_0', str(id(search_tree)), style='invis')

    dot.render(output_dir + 'search_tree.gv', view=False, format='pdf')
    dot.render(output_dir + 'search_tree.gv', view=False, format='svg')
    dot.render(output_dir + 'search_tree.gv', view=False, format='png')

# plot histogram of jump heights
def plot_jump_height_freq(output_dir, jump_height_data):
    all_heights = []
    for height, freq in jump_height_data.items():
        all_heights.extend([height] * freq)

    plt.figure(figsize=(10, 6))

    plt.hist(all_heights, edgecolor='black')
    plt.xticks(sorted(jump_height_data.keys()))

    plt.title('Histogram of Jump Heights')
    plt.xlabel('Jump Height')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'jump_height_freq.png', bbox_inches='tight')
    plt.close()

# plot bar graph of reason frequencies
def plot_reason_freq(output_dir, reason_data):
    plt.figure(figsize=(10, 6))

    freq_sum = sum([sum(data.values()) for _, data in reason_data.items()])

    for success, data in reason_data.items():
        reasons = np.asarray(sorted([k.value for k in data.keys()]))
        labels = np.asarray([f"{r} ({'success' if success else 'fail'})" for r in reasons])
        freqs = np.asarray([data[SearchReason(r)] for r in reasons])
        percents = np.array([100 * freqs[i] / freq_sum for i in range(len(freqs))])

        bars = plt.bar(labels, freqs, color='green' if success else 'red', edgecolor='black')
        plt.bar_label(bars,
              labels=[f'{p:.2f}%\n({f} nodes)' for p, f in zip(percents, freqs)],
              padding=3,
              fontsize=9)

    max_freq = max([(max(data.values()) if len(data.values()) > 0 else 0) for _, data in reason_data.items()])
    plt.ylim(0, 1.1 * max_freq)

    plt.title('Search Node Reason Frequencies by Success Result')
    plt.xlabel('Reason')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'reason_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of exclusive dead end subtree sizes (DESS)
def plot_exclusive_dess_freq(output_dir, dess_data):
    plt.figure(figsize=(10, 6))

    plt.hist(dess_data, edgecolor='black')

    plt.title('Histogram of Exclusive Dead End Subtree Sizes (DESS)')
    plt.xlabel('Dead End Subtree Size')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'exclusive_dess_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of exclusive constraints checked per dead end (CCDE)
def plot_exclusive_ccde_freq(output_dir, ccde_data):
    plt.figure(figsize=(10, 6))

    plt.hist(ccde_data, edgecolor='black')

    plt.title('Histogram of Constraints Checked per Dead End (CCDE)')
    plt.xlabel('Constraints Checked')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'exclusive_ccde_freq.png', bbox_inches='tight')
    plt.close()

# plot histogram of exclusive pruned per dead end (PPDE)
def plot_exclusive_ppde_freq(output_dir, ppde_data):
    plt.figure(figsize=(10, 6))

    plt.hist(ppde_data, edgecolor='black')

    plt.title('Histogram of Pairs Pruned per Dead End (PPDE)')
    plt.xlabel('Pairs Pruned')
    plt.ylabel('Frequency')

    plt.savefig(output_dir + 'exclusive_ppde_freq.png', bbox_inches='tight')
    plt.close()

# plot number of nodes at each depth
def plot_num_nodes_by_depth(output_dir, node_depth_data):
    depths = np.asarray(range(len(node_depth_data['leaves'])))
    sizes = np.asarray([node_depth_data['leaves'][i] for i in depths])

    plt.figure(figsize=(10, 6))

    plt.plot(depths, sizes, marker='o', linestyle='-')

    plt.title('Number of Nodes vs Depth')
    plt.xlabel('Depth')
    plt.ylabel('Number of Nodes')
    plt.grid(True, alpha=0.3)

    plt.savefig(output_dir + 'num_nodes_by_depth.png', bbox_inches='tight')
    plt.close()

# plot effective branching factor (EBF) vs depth
def plot_ebf_by_depth(output_dir, ebf_data):
    depths = np.asarray(sorted(ebf_data.keys()))
    ebfs = np.asarray([ebf_data[i] for i in depths])

    plt.figure(figsize=(10, 6))

    plt.plot(depths, ebfs, marker='o', linestyle='-')

    plt.title('Effective Branching Factor (EBF) vs Max Depth')
    plt.xlabel('Depth')
    plt.ylabel('Effective Branching Factor (EBF)')
    plt.grid(True, alpha=0.3)

    max_depth = max(ebf_data, key=ebf_data.get)
    plt.annotate(
        f"Max EBF: {ebf_data[max_depth]:.4f} @ depth {max_depth}\nOverall EBF: {ebf_data[max(ebf_data.keys())]:.4f} @ depth {max(ebf_data.keys())}",
        xy=(0.98, 0.98),
        xycoords='axes fraction',
        ha='right',
        va='top',
        bbox=dict(boxstyle='round', pad=0.4, facecolor='white', alpha=0.8, edgecolor='0.8')
    )

    plt.savefig(output_dir + 'ebf_by_depth.png', bbox_inches='tight')
    plt.close()

# plot average branching factor (ABF) vs depth
def plot_abf_by_depth(output_dir, abf_data):
    depths = np.asarray(sorted(abf_data.keys()))
    abfs = np.asarray([abf_data[i] for i in depths])

    plt.figure(figsize=(10, 6))

    plt.plot(depths, abfs, marker='o', linestyle='-')

    plt.title('Average Branching Factor (ABF) vs Max Depth')
    plt.xlabel('Depth')
    plt.ylabel('Average Branching Factor (ABF)')
    plt.grid(True, alpha=0.3)

    max_depth = max(abf_data, key=abf_data.get)
    plt.annotate(
        f"Max ABF: {abf_data[max_depth]:.4f} @ depth {max_depth}\nOverall ABF: {abf_data[max(abf_data.keys())]:.4f} @ depth {max(abf_data.keys())}",
        xy=(0.98, 0.98),
        xycoords='axes fraction',
        ha='right',
        va='top',
        bbox=dict(boxstyle='round', pad=0.4, facecolor='white', alpha=0.8, edgecolor='0.8')
    )

    plt.savefig(output_dir + 'abf_by_depth.png', bbox_inches='tight')
    plt.close()

#################### parent function ####################

# run all child functions, return true iff ac3 pruning was tracked
def analyze_search(data, output_dir) -> bool:
    if not get_track_ac3(data):
        print("Warning: AC-3 not tracked in provided file, skipping analysis of search")
        return False

    search_tree = gather_search_tree(data)
    reason_data = search_tree.gather_reason_data()
    jump_height_data = search_tree.gather_jump_height_data()
    dess_data = search_tree.gather_exclusive_dess_data()
    ccde_data = search_tree.gather_exclusive_ccde_data()
    ppde_data = search_tree.gather_exclusive_ppde_data()
    node_depth_data = search_tree.gather_node_depth_data()
    tree_size_data = get_tree_size_data(node_depth_data)
    ebf_data = get_ebf_data(search_tree, tree_size_data)
    abf_data = get_abf_data(tree_size_data)

    # sum of all exclusive failed subtrees equal to total # of failed nodes
    assert(sum(dess_data) == sum(reason_data[False].values()))

    plot_search_tree(output_dir, search_tree)
    plot_reason_freq(output_dir, reason_data)
    plot_jump_height_freq(output_dir, jump_height_data)
    plot_exclusive_dess_freq(output_dir, dess_data)
    plot_exclusive_ccde_freq(output_dir, ccde_data)
    plot_exclusive_ppde_freq(output_dir, ppde_data)
    plot_num_nodes_by_depth(output_dir, node_depth_data)
    plot_ebf_by_depth(output_dir, ebf_data)
    plot_abf_by_depth(output_dir, abf_data)

    with open(output_dir + 'search_metrics.md', 'w') as file:
        file.write("## Search Metrics\n\n")

        file.write("### Search node reasons\n")
        file.write("| Result | Total | Solved | Recursive | AC3 | Duplicate |\n")
        file.write("|--------|-------|--------|-----------|-----|-----------|\n")
        success_reasons = [
            sum(reason_data[True].values()),
            reason_data[True].get(SearchReason.SOLVED, 0),
            reason_data[True].get(SearchReason.RECURSIVE, 0),
            reason_data[True].get(SearchReason.AC3, 0),
            reason_data[True].get(SearchReason.DUPLICATE, 0)
        ]
        fail_reasons = [
            sum(reason_data[False].values()),
            reason_data[False].get(SearchReason.SOLVED, 0),
            reason_data[False].get(SearchReason.RECURSIVE, 0),
            reason_data[False].get(SearchReason.AC3, 0),
            reason_data[False].get(SearchReason.DUPLICATE, 0)
        ]
        all_reasons = [sum(x) for x in zip(success_reasons, fail_reasons)]
        file.write(f"| All | {all_reasons[0]} | {all_reasons[1]} | {all_reasons[2]} | {all_reasons[3]} | {all_reasons[4]} |\n")
        file.write(f"| Success | {success_reasons[0]} | {success_reasons[1]} | {success_reasons[2]} | {success_reasons[3]} | {success_reasons[4]} |\n")
        file.write(f"| Fail | {fail_reasons[0]} | {fail_reasons[1]} | {fail_reasons[2]} | {fail_reasons[3]} | {fail_reasons[4]} |\n")

        file.write("### Jump heights\n")
        file.write("| Min | Max | Average | Median |\n")
        file.write("|-----|-----|---------|--------|\n")
        all_heights = []
        for height, freq in jump_height_data.items():
            all_heights.extend([height] * freq)
        if len(all_heights) > 0:
            file.write(f"| {min(all_heights)} | {max(all_heights)} | {sum(all_heights)/len(all_heights):.2f} | {np.median(all_heights):.1f} |\n")
        else:
            file.write("| N/A | N/A | N/A | N/A |\n")

        file.write("### All jump heights\n")
        file.write('| Height |')
        for length in sorted(jump_height_data.keys()):
            file.write(' ' + str(length) + ' |')
        file.write('\n|' + ("---|" * (len(jump_height_data.keys()) + 1)) + '\n')
        file.write('| Count |')
        for length in sorted(jump_height_data.keys()):
            file.write(' ' + str(jump_height_data[length]) + ' |')
        file.write('\n')

        file.write("### Dead end subtree size (DESS)\n")
        file.write("| Count | Min | Max | Average | Median |\n")
        file.write("|-------|-----|-----|---------|--------|\n")
        if len(dess_data) > 0:
            file.write(f"| {len(dess_data)} | {min(dess_data)} | {max(dess_data)} | {sum(dess_data)/len(dess_data):.2f} | {np.median(dess_data):.1f}|\n")
        else:
            file.write(f"| N/A | N/A | N/A | N/A | N/A |\n")

        file.write("### Constraints checked per dead end (CCDE)\n")
        file.write("| Count | Min | Max | Average | Median |\n")
        file.write("|-------|-----|-----|---------|--------|\n")
        if len(ccde_data) > 0:
            file.write(f"| {len(ccde_data)} | {min(ccde_data)} | {max(ccde_data)} | {sum(ccde_data)/len(ccde_data):.2f} | {np.median(ccde_data):.1f}|\n")
        else:
            file.write(f"| N/A | N/A | N/A | N/A | N/A |\n")

        file.write("### Pairs pruned per dead end (PPDE)\n")
        file.write("| Count | Min | Max | Average | Median |\n")
        file.write("|-------|-----|-----|---------|--------|\n")
        if len(ppde_data) > 0:
            file.write(f"| {len(ppde_data)} | {min(ppde_data)} | {max(ppde_data)} | {sum(ppde_data)/len(ppde_data):.2f} | {np.median(ppde_data):.1f}|\n")
        else:
            file.write(f"| N/A | N/A | N/A | N/A | N/A |\n")

        file.write("### Overall effective branching factor (EBF)\n")
        file.write(f'{ebf_data[max(ebf_data.keys())]:.4f}\n')

        file.write("### Max effective branching factor (EBF)\n")
        file.write(f'{ebf_data[max(ebf_data, key=ebf_data.get)]:.4f} @ depth {max(ebf_data, key=ebf_data.get)}\n')

        file.write("### Overall average branching factor (ABF)\n")
        file.write(f'{abf_data[max(abf_data.keys())]:.4f}\n')

        file.write("### Max average branching factor (ABF)\n")
        file.write(f'{abf_data[max(abf_data, key=abf_data.get)]:.4f} @ depth {max(abf_data, key=abf_data.get)}\n')

        file.write('\n')

    return True