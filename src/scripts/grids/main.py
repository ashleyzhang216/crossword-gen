from utils import *
from grid import *

if __name__ == "__main__":
    clean_output()

    grid = blank_grid(grid_size)

    # placeholder
    black_squares = [(0, 0), (1, 2), (3, 5), (7, 7), (10, 8), (13, 10), (5, 3), (6, 6)]
    for r, c in black_squares:
        grid[r, c] = 0

    save_grid(grid)
