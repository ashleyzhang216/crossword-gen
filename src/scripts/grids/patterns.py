import numpy as np
from enum import Enum

class Pattern(Enum):
    SINGLE = 0
    BAR2_DOWN = 1
    BAR2_ACROSS = 2
    BAR3_DOWN = 3
    BAR3_ACROSS = 4
    DIAG2_RISING = 5
    DIAG2_FALLING = 6
    DIAG3_RISING = 7
    DIAG3_FALLING = 8

    # 2 quotes
    LQUOTE2_ROT0 = 9
    LQUOTE2_ROT90 = 10
    LQUOTE2_ROT180 = 11
    LQUOTE2_ROT270 = 12
    RQUOTE2_ROT0 = 13
    RQUOTE2_ROT90 = 14
    RQUOTE2_ROT180 = 15
    RQUOTE2_ROT270 = 16

    # 3 quotes
    LQUOTE3_ROT0 = 17
    LQUOTE3_ROT90 = 18
    LQUOTE3_ROT180 = 19
    LQUOTE3_ROT270 = 20
    RQUOTE3_ROT0 = 21
    RQUOTE3_ROT90 = 22
    RQUOTE3_ROT180 = 23
    RQUOTE3_ROT270 = 24

# returns 
def get_pattern_grid(pattern):
    match pattern:
        case Pattern.SINGLE:
            return np.zeros((1, 1))
        case Pattern.BAR2_DOWN:
            return np.zeros((2, 1))
        case Pattern.BAR2_ACROSS:
            return np.zeros((1, 2))
        case Pattern.BAR3_DOWN:
            return np.zeros((3, 1))
        case Pattern.BAR3_ACROSS:
            return np.zeros((1, 3))
        case Pattern.DIAG2_RISING:
            return np.array([
                [1, 0],
                [0, 1]
            ])
        case Pattern.DIAG2_FALLING:
            return np.array([
                [0, 1],
                [1, 0]
            ])
        case Pattern.DIAG3_RISING:
            return np.array([
                [1, 1, 0],
                [1, 0, 1],
                [0, 1, 1]
            ])
        case Pattern.DIAG3_FALLING:
            return np.array([
                [0, 1, 1],
                [1, 0, 1],
                [1, 1, 0]
            ])

        # 2 quotes
        case Pattern.LQUOTE2_ROT0:
            return np.array([
                [1, 0],
                [0, 1],
                [0, 1]
            ])
        case Pattern.LQUOTE2_ROT90:
            return np.array([
                [0, 0, 1],
                [1, 1, 0]
            ])
        case Pattern.LQUOTE2_ROT180:
            return np.array([
                [1, 0],
                [1, 0],
                [0, 1]
            ])
        case Pattern.LQUOTE2_ROT270:
            return np.array([
                [0, 1, 1],
                [1, 0, 0]
            ])
        case Pattern.RQUOTE2_ROT0:
            return np.array([
                [0, 1],
                [1, 0],
                [1, 0]
            ])
        case Pattern.RQUOTE2_ROT90:
            return np.array([
                [1, 1, 0],
                [0, 0, 1]
            ])
        case Pattern.RQUOTE2_ROT180:
            return np.array([
                [0, 1],
                [0, 1],
                [1, 0]
            ])
        case Pattern.RQUOTE2_ROT270:
            return np.array([
                [1, 0, 0],
                [0, 1, 1]
            ])
        
        # 3 quotes
        case Pattern.LQUOTE3_ROT0:
            return np.array([
                [1, 0],
                [0, 1],
                [0, 1],
                [0, 1]
            ])
        case Pattern.LQUOTE3_ROT90:
            return np.array([
                [0, 0, 0, 1],
                [1, 1, 1, 0]
            ])
        case Pattern.LQUOTE3_ROT180:
            return np.array([
                [1, 0],
                [1, 0],
                [1, 0],
                [0, 1]
            ])
        case Pattern.LQUOTE3_ROT270:
            return np.array([
                [0, 1, 1, 1],
                [1, 0, 0, 0]
            ])
        case Pattern.RQUOTE3_ROT0:
            return np.array([
                [0, 1],
                [1, 0],
                [1, 0],
                [1, 0]
            ])
        case Pattern.RQUOTE3_ROT90:
            return np.array([
                [1, 1, 1, 0],
                [0, 0, 0, 1]
            ])
        case Pattern.RQUOTE3_ROT180:
            return np.array([
                [0, 1],
                [0, 1],
                [0, 1],
                [1, 0]
            ])
        case Pattern.RQUOTE3_ROT270:
            return np.array([
                [1, 0, 0, 0],
                [0, 1, 1, 1]
            ])
        case _:
            raise Exception("unknown pattern: " + str(pattern))
            return None

