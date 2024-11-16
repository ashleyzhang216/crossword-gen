from utils import *
from grid import *
from scoring import *
import random

MAX_JITTER = 16
MAX_DEPTH = 10
MAX_CANDIDATES = 10

def simulate_annealing(grid):
    def anneal(grid, depth, candidates, explored_grid_bytes):
        if depth < MAX_DEPTH:
            true_score = get_err_score(grid)
            temperature = 0.5 ** depth

            permutations = get_permutations(grid)
            permutations = random.sample(permutations, MAX_CANDIDATES)
            for i, p in enumerate(permutations):
                d, _ = disqualified(p)
                grid_bytes = p.tobytes()
                if not d and not grid_bytes in explored_grid_bytes:
                    explored_grid_bytes.add(grid_bytes)
                    true_candidate_score = get_err_score(p)
                    jitter = int(MAX_JITTER * temperature)
                    candidate_score = true_candidate_score + random.randint(-jitter, jitter)

                    if candidate_score <= true_score:
                        anneal(p, depth+1, candidates, explored_grid_bytes)

                if depth == 0:
                    completion = (i + 1) / len(permutations)
                    print(str(completion) + " complete")

            candidates.append((true_score, grid))

    all_candidates = []
    explored = set()
    anneal(grid, 0, all_candidates, explored)
    sorted_candidates = sorted(all_candidates, key=lambda x: x[0])

    for cscore, cgrid in sorted_candidates:
        save_grid(cgrid, cscore)


if __name__ == "__main__":
    clean_output()

    grid = blank_grid(grid_size)

    # black_squares = [(0, 0), (1, 2), (3, 5), (7, 7), (5, 3), (6, 6)]
    # # black_squares = [(0, 2), (1, 2), (2, 0), (2, 1)]
    # for r, c in black_squares:
    #     grid[r, c] = 0

    simulate_annealing(grid)

    # save_grid(grid, get_err_score(grid))
    # permutations = get_permutations(grid)
    # print("num permutations: ", len(permutations))
    
    # for i in range(0, len(permutations)):
    #     d, _ = disqualified(permutations[i])
    #     if not d:
    #         save_grid(permutations[i], get_err_score(permutations[i]))

