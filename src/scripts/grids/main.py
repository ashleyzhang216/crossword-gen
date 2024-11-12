from utils import *
from grid import *
from scoring import *

if __name__ == "__main__":
    clean_output()

    grid = blank_grid(grid_size)

    # black_squares = [(0, 0), (1, 2), (3, 5), (7, 7), (5, 3), (6, 6)]
    # # black_squares = [(0, 2), (1, 2), (2, 0), (2, 1)]
    # for r, c in black_squares:
    #     grid[r, c] = 0

    save_grid(grid)
    permutations = get_permutations(grid)
    print("num permutations: ", len(permutations))
    
    for i in range(0, len(permutations), 10):
        d, _ = disqualified(permutations[i])
        if not d:
            save_grid(permutations[i])

