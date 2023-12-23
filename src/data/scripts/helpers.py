import os

# get absolute filepath to data by name, located relatively at /../raw/filename
def get_abs_path(filename):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'raw', filename))