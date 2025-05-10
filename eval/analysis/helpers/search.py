import numpy as np
import matplotlib.pyplot as plt

#################### parent function ####################

# run all child functions, returns true
def analyze_search(data, output_dir) -> bool:

    with open(output_dir + 'search_metrics.md', 'w') as file:
        file.write("## Search Metrics\n\n")

        file.write('\n')

    return True