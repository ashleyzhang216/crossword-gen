import numpy as np
import statistics

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

# returns num_words, {map of length -> frequency}, [list of intersection word length pairs]
def words(grid):
    x, y = grid.shape
    freq = {}
    num_words = 0

    intersection_grid = np.zeros((x, y, 2), dtype=int) # first -> down, second -> across
    word_lens = []
    next_len_idx = 0

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

                    word_lens.append(word_len)
                    next_len_idx += 1

                    num_words += 1
                    in_word = False
            else:
                if in_word:
                    word_len += 1
                else:
                    in_word = True
                    word_len = 1

                intersection_grid[i][j][0] = next_len_idx

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

                    word_lens.append(word_len)
                    next_len_idx += 1

                    num_words += 1
                    in_word = False
            else:
                if in_word:
                    word_len += 1
                else:
                    in_word = True
                    word_len = 1

                intersection_grid[i][j][1] = next_len_idx

    intersections = []
    for i in range(x):
        for j in range(y):
            if not intersection_grid[i][j][0] == 0 and not intersection_grid[i][j][1] == 0:
                intersections.append((intersection_grid[i][j][0], intersection_grid[i][j][1]))

    return num_words, freq, intersections

# returns avg_word_len, median_word_len
def get_word_stats(freq):
    total_len = 0
    num_words = 0
    all_word_lens = []

    for key, value in freq.items():
        total_len += key * value
        num_words += value
        for i in range(value):
            all_word_lens.append(key)
    
    return all_word_lens, total_len / num_words, statistics.median(all_word_lens)

def get_intersection_product(intersections):
    sum = 0
    for x, y in intersections:
        sum += x * y
    return sum

# returns true iff grid has isolated section not connected to rest of grid
def contains_island(grid):
    # returns number of blank tiles accessible from root tile
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
    
    num_words, freq, intersections = words(grid)
    if 1 in freq:
        return True, "1 letter word"
    if 2 in freq:
        return True, "2 letter word"
    
    return False, None

# real error score, lower the better
def get_err_score(grid):
    num_words, freq, intersections = words(grid)
    all_word_lens, avg_word_len, median_word_len = get_word_stats(freq)
    intersection_product = get_intersection_product(intersections)

    # number of black tiles
    bt_ratio = black_tile_ratio(grid)

    # number of words
    ideal_num_words = 76 # harcoded for 15x15 grids, TOOD: change this
    num_word_err = abs(num_words - ideal_num_words) / ideal_num_words

    # penalize short words
    ideal_word_len_sq = num_words * (avg_word_len ** 2)
    word_len_sq = sum(len ** 2 for len in all_word_lens)
    word_len_sq_err = abs(word_len_sq - ideal_word_len_sq) / ideal_word_len_sq

    # penalize large intersection products
    ideal_intersection_product = num_words * (avg_word_len ** 2)
    intersection_err = abs(intersection_product - ideal_intersection_product) / ideal_intersection_product

    return 1*bt_ratio + 5*num_word_err + 2*word_len_sq_err + 10*intersection_err
