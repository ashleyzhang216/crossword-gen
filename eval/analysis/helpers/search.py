import numpy as np
import matplotlib.pyplot as plt
from enum import Enum
from graphviz import Digraph
from typing import Callable

#################### data collection ####################

class SearchReason(Enum):
    RECURSIVE = "recursive"
    AC3 = "ac3"
    SOLVED = "solved"
    DUPLICATE = "duplicate"

    @classmethod
    def from_string(cls, value):
        try:
            return cls(value)
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

        self.jump_height = None # num nodes backtracked/backjumped after evaluating all children
        self.var_of_children = None # variable that child nodes are trying to assign a word to (not this node)
        self.failed_subtree_size = None

    def add_child(self, success:bool, reason:SearchReason, word:str):
        child = SearchNode(success, reason, word=word, parent=self)
        self.children.append(child)
        return child

    def set_failed_subtree_size(self):
        assert(self.failed_subtree_size is None)

        self.failed_subtree_size = 0 if self.success or self.parent is None else 1
        for child in self.children:
            child.set_failed_subtree_size()
            assert(not child.failed_subtree_size is None)
            self.failed_subtree_size += child.failed_subtree_size

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

# returns tree of SearchNode
def gather_search_tree(data) -> SearchNode:
    assert('type' in data and data['type'] == "Total")
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
        else:
            if data_node['type'] == "Search Step":
                # temporarily enforce only backtracking since this is the only thing tested so far
                # TODO: remove this once other methods are supported
                assert(data_node['name'] == "Backtracking")

                if data_node['result']['success'] and data_node['result']['reason'] == "solved":
                    search_node.set_reason_solved()

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
            [Color.YELLOW, "Fail: Recursive"],
            [Color.ORANGE, "Fail: Duplicate"],
            [Color.RED, "Fail: AC-3"]
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

    dot.render(output_dir + 'search_tree.gv', view=False)

#################### parent function ####################

# run all child functions, returns true
def analyze_search(data, output_dir) -> bool:

    search_tree = gather_search_tree(data)

    plot_search_tree(output_dir, search_tree)

    with open(output_dir + 'search_metrics.md', 'w') as file:
        file.write("## Search Metrics\n\n")

        file.write('\n')

    return True