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
OUTPUT_FILE = "word_data.json"

# read word frequency data, return as dict of word -> count
# https://www.kaggle.com/datasets/rtatman/english-word-frequency/data
def get_word_freq():
    df = pd.read_csv(get_abs_path(FREQ_FILE, "raw"), sep=',')
    dict = df.set_index("word").to_dict()["count"]
    return dict

# reads word files with lines of format WORD;SCORE, adds to word_scores dict
# adds all valid scores to all_score_list to create score frequency graph
def read_word_file(filename, word_scores, all_score_list):
    with open(get_abs_path(filename, "raw"), 'r') as file:
        lines = [line.strip() for line in file.readlines()]

        for line in lines:
            word, score = line.strip().split(';')
            score = int(score)

            valid, word = filter_word(word)

            if valid:
                if word in word_scores:
                    word_scores[word] = max(word_scores[word], score)
                else:
                    word_scores[word] = score

                all_score_list.append(score)

# returns dataframe containing word data, and list of all word scores
def parse_data():
    freq_dict = get_word_freq()

    all_score_list = []
    word_scores = {}
    for file in WORD_FILES:
        read_word_file(file, word_scores, all_score_list)
    
    data = [(word, score, freq_dict[word] if word in freq_dict else 0) for word, score in word_scores.items()]
    df = pd.DataFrame(data, columns=['Word', 'Score', 'Frequency'])
    df = df.sort_values(by=['Score', 'Frequency'], ascending=[False, False])
    df = df.reset_index(drop=True)

    return df, all_score_list

# generate output json files
def generate_outputs(df):
    json_str = df.set_index('Word').to_json(orient='index', indent=4)
    with open(get_abs_path(OUTPUT_FILE, "."), 'w') as file:
        file.write(json_str)

def main():
    df, all_score_list = parse_data()
    generate_outputs(df)

    print(df.head())
    print(df.shape)
    plot_number_frequency(all_score_list)

if __name__ == "__main__":
    main()