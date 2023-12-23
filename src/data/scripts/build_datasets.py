import pandas as pd
from helpers import get_abs_path

DIR_PATH = "/../raw/"

FREQ_FILE = "unigram_freq.csv"

WORD_FILES = [
    "Broda List 03.2020 trimmed by Diehl.txt",
    "CLUED list to share ranked.txt",
    "spreadthewordlist.txt"
]

# read word frequency data, return as dict of word -> count
# https://www.kaggle.com/datasets/rtatman/english-word-frequency/data
def get_word_freq():
    df = pd.read_csv(get_abs_path(FREQ_FILE), sep=',')
    dict = df.set_index("word").to_dict()["count"]
    return dict

def main():
    freq_dict = get_word_freq()

if __name__ == "__main__":
    main()