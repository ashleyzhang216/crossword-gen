import os
import string
import matplotlib.pyplot as plt
from collections import Counter

# get absolute filepath to data by name, located relatively at /../raw/filename
def get_abs_path(filename, subdir):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', subdir, filename))

# returns valid, formatted_word
# removes punctuation, filters out words with otherwise non alphabetical characters
def filter_word(word):
    word = word.lower()
    word = word.translate(str.maketrans('', '', string.punctuation))

    if not word.isalpha():
        return False, word

    return True, word

# creates bar graph of score frequencies
def plot_number_frequency(numbers):
    number_counts = Counter(numbers)
    
    numbers = list(number_counts.keys())
    counts = list(number_counts.values())
    
    plt.figure(figsize=(12, 6))
    plt.bar(numbers, counts, color='skyblue')
    plt.xticks(rotation=45)  
    
    plt.tight_layout()
    plt.show()