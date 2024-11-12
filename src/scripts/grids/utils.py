import matplotlib.pyplot as plt
import numpy as np
import os
import shutil

BLACK = 0
BLANK = 1
OUT_DIR = "out"
TICK_FONT_SIZE = 5

grid_size = (15, 15) # rows, cols

num_grids_made = 0

# clean new empty output directory 
def clean_output():
    if os.path.exists(OUT_DIR):
        shutil.rmtree(OUT_DIR)
    os.makedirs(OUT_DIR)

# save grid as png to output directory
def save_grid(grid, show=False):
    global num_grids_made

    fig, ax = plt.subplots()
    ax.imshow(grid, cmap="gray", origin="upper")
    
    x, y = grid.shape
    ax.set_xticks(np.arange(-0.5, y, 1), minor=True)
    ax.set_yticks(np.arange(-0.5, x, 1), minor=True)
    ax.grid(which="minor", color="black", linestyle='-', linewidth=0.5)

    ax.set_xticks(np.arange(0, y, 1))
    ax.set_yticks(np.arange(0, x, 1))
    ax.set_xticklabels(np.arange(0, y, 1), fontsize=TICK_FONT_SIZE)
    ax.set_yticklabels(np.arange(0, x, 1), fontsize=TICK_FONT_SIZE)

    ax.tick_params(which="major", bottom=False, left=False)
    ax.tick_params(which="minor", bottom=False, left=False)

    plt.title("Grid " + str(num_grids_made))
    plt.savefig(OUT_DIR + "/grid" + str(num_grids_made) + ".png", dpi=300)

    num_grids_made += 1

    plt.close()