# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   12/23/2023
# Description: one-time script to parse raw data and output json format datasets
# ==================================================================

import pandas as pd
from helpers import get_abs_path, filter_word, target_ratio, plot_number_frequency

# data files
FREQ_FILE = "unigram_freq.csv" # https://www.kaggle.com/datasets/rtatman/english-word-frequency/data
WORD_FILES = [
    "Broda List 03.2020 trimmed by Diehl.txt", # https://www.facebook.com/groups/1515117638602016/permalink/2997721820341583
    "CLUED list to share ranked.txt", # https://www.facebook.com/groups/1515117638602016/permalink/1811192232327887
    "spreadthewordlist.txt", # https://www.spreadthewordlist.com/wordlist
    "xwordlist.txt", # https://raw.githubusercontent.com/Crossword-Nexus/collaborative-word-list/main/xwordlist.dict
    "crossfire_default.txt" # crossfire software
]
BAD_WORDS_FILE = "bad_words.txt" # https://data.world/wordlists/dirty-naughty-obscene-and-otherwise-bad-words-in-english
OUTPUT_FILE = "word_data.json"
TARGET_DATA_SIZES = [(1000, "small"), (5000, "medium"), (10000, "large"), (50000, "xlarge"), (100000, "giant"), (None, "all")]
DISPLAY_SCORE_DISTR = False
DISPLAY_LENGTH_DISTR = False

# read word frequency data, return as dict of word -> count
def get_word_freq():
    df = pd.read_csv(get_abs_path(FREQ_FILE, "raw"), sep=',')
    dict = df.set_index("word").to_dict()["count"]
    return dict

# reads word files with lines of format WORD;SCORE, adds to word_scores dict
# adds all valid scores to all_score_list to create score frequency graph
def read_word_file(filename, word_scores, bad_words, all_score_list, all_word_lengths):
    with open(get_abs_path(filename, "raw"), 'r') as file:
        lines = [line.strip() for line in file.readlines()]

        for line in lines:
            word, score = line.strip().split(';')
            score = int(score)

            valid, word = filter_word(word)

            # filters out valid words if any bad word is a substring in it
            # overly aggressive filtering but should ensure no slurs/bad words appear
            # ex: without this, 'faggots' would be allowed, which is clearly offensive
            if valid and not any(bad_word in word for bad_word in bad_words):
                if word in word_scores:
                    word_scores[word] = max(word_scores[word], score)
                else:
                    word_scores[word] = score
                    all_word_lengths.append(len(word))

                all_score_list.append(score)

# returns dict of all bad words to exclude
def read_bad_words_file():
    with open(get_abs_path(BAD_WORDS_FILE, "raw"), 'r') as file:
        lines = [filter_word(line.strip())[1] for line in file.readlines()]
        return set(lines)

# cap max # of words of each length as if all letters assigned to words of this length
# adds intended bias towards more words of shorter lengths in distribution
def cap_max_words_per_letter(df, target_ratio):
    total_letters = df['Word'].str.len().sum()

    filtered_dfs = []
    for length, group_df in df.groupby(df['Word'].str.len()):
        group_df = group_df.sort_values(by=['Score', 'Frequency'], ascending=[False, False])
        num_rows = int(total_letters * target_ratio / length) # allows more words for smaller lengths
        filtered_df = group_df.head(num_rows)
        filtered_dfs.append(filtered_df)
    return pd.concat(filtered_dfs)

# scales distribution to ensure size compliance by maintaining the ratio of words of each length
# rescale_letter_distribution
def rescale_letter_distribution(df, target_ratio):
    filtered_dfs = []
    for length, group_df in df.groupby(df['Word'].str.len()):
        group_df = group_df.sort_values(by=['Score', 'Frequency'], ascending=[False, False])
        num_rows = int(len(group_df) * target_ratio)
        filtered_df = group_df.head(num_rows)
        filtered_dfs.append(filtered_df)
    return pd.concat(filtered_dfs)

# returns dataframe containing word data, and list of all word scores
def parse_data(target_size):
    freq_dict = get_word_freq()
    bad_words = read_bad_words_file()

    all_score_list = []
    all_word_lengths = []
    word_scores = {}
    for file in WORD_FILES:
        read_word_file(file, word_scores, bad_words, all_score_list, all_word_lengths)
    
    data = [(word, score, freq_dict[word] if word in freq_dict else 0) for word, score in word_scores.items()]
    df = pd.DataFrame(data, columns=['Word', 'Score', 'Frequency'])

    df = cap_max_words_per_letter(df, target_ratio(target_size, df.shape[0]))
    df = rescale_letter_distribution(df, target_ratio(target_size, df.shape[0]))
    df = df.sort_values(by=['Score', 'Frequency'], ascending=[False, False])
    df = df.reset_index(drop=True)
 
    score_list = df['Score'].tolist()
    word_lengths = df['Word'].str.len().tolist()

    return df, score_list, word_lengths

# generate output json file
def generate_output(df, size_name):
    json_str = df.set_index('Word').to_json(orient='index', indent=4)
    with open(get_abs_path("data_" + size_name + ".json", "."), 'w') as file:
        file.write(json_str)

# create datasets
def main():
    for (target_size, size_name) in TARGET_DATA_SIZES:
        df, score_list, word_lengths = parse_data(target_size)
        generate_output(df, size_name)
        plot_number_frequency(DISPLAY_SCORE_DISTR, score_list, "scores for target " + str(target_size) + ", actual " + str(df.shape[0]))
        plot_number_frequency(DISPLAY_LENGTH_DISTR, word_lengths, "lengths for target " + str(target_size) + ", actual " + str(df.shape[0]))

if __name__ == "__main__":
    main()