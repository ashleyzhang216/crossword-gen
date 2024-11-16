import matplotlib.pyplot as plt
import numpy as np

from utils import *
from patterns import *

# returns blank grid
def blank_grid(dims):
    return np.full(dims, 1)

# places pattern on grid
# returns success, new_grid
def place_pattern(grid, pattern_grid, loc):
    new_grid = np.copy(grid)
    x, y = new_grid.shape
    new_black_tiles = []

    for i in range(pattern_grid.shape[0]):
        for j in range(pattern_grid.shape[1]):
            if pattern_grid[i, j] == BLACK:
                if new_grid[i + loc[0], j + loc[1]] == BLACK:
                    return False, None
                else:
                    new_grid[i + loc[0], j + loc[1]] = BLACK
                    new_black_tiles.append((i + loc[0], j + loc[1]))

    # make symmetrical
    for i, j in new_black_tiles:
        if new_grid[x - i - 1, y - j - 1] == BLACK:
            return False, None
        new_grid[x - i - 1, y - j - 1] = BLACK
    
    return True, new_grid

# get all possible next placements of a pattern
def get_permutations(grid):
    X, Y = grid.shape

    permutations = []
    for pattern in Pattern:
        p_grid = get_pattern_grid(pattern)

        x, y = p_grid.shape
        for i in range(X - x):
            for j in range(min(Y - i, Y - y)):
                success, permutation = place_pattern(grid, p_grid, (i, j))
                if success:
                    permutations.append(permutation)
    
    return permutations
