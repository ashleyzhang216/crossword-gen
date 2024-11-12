import numpy as np
from scipy.ndimage import label

from utils import *

# returns [0, 1] fraction of black tiles
def black_tile_ratio(grid):
    x, y = grid.shape
    num_tiles = x * y

    num_black = 0
    for i in range(x):
        for j in range(y):
            if grid[i, j] == BLACK:
                num_black += 1
    
    return num_black / num_tiles

# returns num_words, {map of length -> frequency}
def words(grid):
    x, y = grid.shape
    freq = {}
    num_words = 0

    # down words
    for i in range(x):
        in_word = False
        word_len = 0

        for j in range(y + 1):
            if j == y or grid[i, j] == BLACK:
                # end word
                if in_word:
                    if word_len in freq:
                        freq[word_len] += 1
                    else:
                        freq[word_len] = 1

                    num_words += 1
                    in_word = False
            else:
                if in_word:
                    word_len += 1
                else:
                    in_word = True
                    word_len = 1

    # across words
    for j in range(y):
        in_word = False
        word_len = 0

        for i in range(x + 1):
            if i == x or grid[i, j] == BLACK:
                # end word
                if in_word:
                    if word_len in freq:
                        freq[word_len] += 1
                    else:
                        freq[word_len] = 1

                    num_words += 1
                    in_word = False
            else:
                if in_word:
                    word_len += 1
                else:
                    in_word = True
                    word_len = 1
    
    return num_words, freq

# helper for contains_island
def num_accessible_tiles(grid, loc):
    x, y = grid.shape
    visited = np.zeros_like(grid, dtype=bool)
    stack = [loc]

    while len(stack) > 0:
        curr = stack.pop()
        visited[curr] = True
        candidates = [(curr[0] - 1, curr[1]), (curr[0] + 1, curr[1]), (curr[0], curr[1] - 1), (curr[0], curr[1] + 1)]

        for c in candidates:
            if c[0] >= 0 and c[0] < x and c[1] >= 0 and c[1] < y and not visited[c] and grid[c] == BLANK:
                stack.append(c)
    
    return np.sum(visited)

# returns true iff grid has isolated section not connected to rest of grid
def contains_island(grid):
    num_blank_tiles = np.sum(grid)
    x, y = grid.shape
    
    for i in range(x):
        for j in range(y):
            if grid[i, j] == BLANK:
                if not num_accessible_tiles(grid, (i, j)) == num_blank_tiles:
                    return True

    return False

# return true iff disqualified due to egrigous features  
def disqualified(grid):
    if contains_island(grid):
        return True, "island"
    
    num_words, freq = words(grid)
    if 1 in freq:
        return True, "1 letter word"
    
    return False, None

# TODO: implement
def score(grid):
    return 0