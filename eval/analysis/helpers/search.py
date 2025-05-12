import numpy as np
import matplotlib.pyplot as plt
from enum import Enum
from graphviz import Digraph

#################### data collection ####################

class SearchReason(Enum):
    RECURSIVE = "recursive"
    AC3 = "ac3"
    SOLVED = "solved"
    DUPLICATE = "duplicate"

    @classmethod
    def from_string(cls, status_str):
        try:
            return cls(status_str)
        except ValueError:
            raise ValueError(f"{status_str} is not a valid SearchReason")

class SearchNode:
    def __init__(self, success:bool, reason:SearchReason, parent=None):
        self.parent = parent
        self.success = success
        self.children = []
        self.reason = reason

        self.jump_height = None
        self.failed_subtree_size = None

    def add_child(self, success:bool, reason:SearchReason):
        child = SearchNode(success, reason, parent=self)
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

    tree = SearchNode(search_root['result']['success'], SearchReason(search_root['result']['reason']))
    def traverse(self, search_node, data_node):
        if data_node['type'] == "Try Assign":
            node_child = search_node.add_child(data_node['result']['success'], SearchReason(data_node['result']['reason']))

            for child_data_node in data_node.get('children', []):
                self(self, node_child, child_data_node)
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
                self(self, search_node, child_data_node)

    traverse(traverse, tree, data)
    tree.set_failed_subtree_size()

    return tree

#################### parent function ####################

# run all child functions, returns true
def analyze_search(data, output_dir) -> bool:

    search_tree = gather_search_tree(data)

    with open(output_dir + 'search_metrics.md', 'w') as file:
        file.write("## Search Metrics\n\n")

        file.write('\n')

    return True