import pandas as pd
from helpers import get_abs_path, filter_word, plot_number_frequency

# data files
FREQ_FILE = "unigram_freq.csv"
WORD_FILES = [
    "Broda List 03.2020 trimmed by Diehl.txt",
    "CLUED list to share ranked.txt",
    "spreadthewordlist.txt",
    "xwordlist.txt",
    "crossfire_default.txt"
]
BAD_WORDS_FILE = "bad_words.txt"
OUTPUT_FILE = "word_data.json"

# read word frequency data, return as dict of word -> count
# https://www.kaggle.com/datasets/rtatman/english-word-frequency/data
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
def read_bad_words_file(filename):
    with open(get_abs_path(filename, "raw"), 'r') as file:
        lines = [filter_word(line.strip())[1] for line in file.readlines()]
        return set(lines)

# returns dataframe containing word data, and list of all word scores
def parse_data():
    freq_dict = get_word_freq()
    bad_words = read_bad_words_file(BAD_WORDS_FILE)

    all_score_list = []
    all_word_lengths = []
    word_scores = {}
    for file in WORD_FILES:
        read_word_file(file, word_scores, bad_words, all_score_list, all_word_lengths)
    
    data = [(word, score, freq_dict[word] if word in freq_dict else 0) for word, score in word_scores.items()]
    df = pd.DataFrame(data, columns=['Word', 'Score', 'Frequency'])
    df = df.sort_values(by=['Score', 'Frequency'], ascending=[False, False])
    df = df.reset_index(drop=True)

    return df, all_score_list, all_word_lengths

# generate output json files
def generate_outputs(df):
    json_str = df.set_index('Word').to_json(orient='index', indent=4)
    with open(get_abs_path(OUTPUT_FILE, "."), 'w') as file:
        file.write(json_str)

def main():
    df, all_score_list, all_word_lengths = parse_data()
    generate_outputs(df)

    print(df.head())
    print(df.shape)
    plot_number_frequency(all_word_lengths)

if __name__ == "__main__":
    main()