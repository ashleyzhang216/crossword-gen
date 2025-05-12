import numpy as np
import matplotlib.pyplot as plt
from enum import Enum
from graphviz import Digraph

#################### data collection ####################

class SearchReason(Enum):
    RECURSIVE = ("recursive", "#fff2cc") # yellow
    AC3 = ("ac3", "#f8cecc") # red
    SOLVED = ("solved", "#d5e8d4") # green
    DUPLICATE = ("duplicate", "#f5d6b0") # orange

    def __init__(self, string_value, color):
        self.string_value = string_value
        self.color = color

    @classmethod
    def from_string(cls, value):
        for member in cls:
            if member.string_value == value:
                return member
        raise ValueError(f"{value} is not a valid SearchReason")

class SearchNode:
    def __init__(self, id:int, success:bool, reason:SearchReason, parent=None):
        self.id = id
        self.success = success
        self.reason = reason
        self.parent = parent
        self.children = []

        self.jump_height = None
        self.failed_subtree_size = None

    def add_child(self, id:int, success:bool, reason:SearchReason):
        child = SearchNode(id, success, reason, parent=self)
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
        self.jump_height = 0 if jump_height is None else jump_height

    def set_reason_solved(self):
        assert(not self.reason is None)
        assert(not self.reason is SearchReason.SOLVED)
        self.reason = SearchReason.SOLVED

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

    tree = SearchNode(0, search_root['result']['success'], SearchReason.from_string(search_root['result']['reason']))
    next_id = 1

    def traverse(search_node, data_node):
        nonlocal next_id
        if data_node['type'] == "Try Assign":
            node_child = search_node.add_child(next_id, data_node['result']['success'], SearchReason.from_string(data_node['result']['reason']))
            next_id += 1

            for child_data_node in data_node.get('children', []):
                traverse(node_child, child_data_node)
        else:
            if data_node['type'] == "Search Step":
                if data_node['result']['success'] and data_node['result']['reason'] == "solved":
                    # set reasons up to root to be "solved"
                    current = search_node
                    while not current is None:
                        current.set_reason_solved()
                        current = current.parent

                search_node.set_jump_height(data_node['result']['jump_height'])

            for child_data_node in data_node.get('children', []):
                traverse(search_node, child_data_node)

    traverse(tree, data)
    tree.set_failed_subtree_size()

    return tree

#################### plotting ####################

# draw search tree
def plot_search_tree(output_dir, search_tree:SearchNode):
    dot = Digraph(comment='Search Tree')
    dot.attr('node', style='filled')

    all_ids = set()
    def add_node_edges(node:SearchNode):
        assert(not node.id in all_ids)
        all_ids.add(node.id)

        dot.node(str(node.id), node.reason.string_value, color=node.reason.color)
        for child in node.children:
            dot.edge(str(node.id), str(child.id))
            add_node_edges(child)

    add_node_edges(search_tree)
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