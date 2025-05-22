import json
import argparse
import sys
import os
from datetime import datetime

from helpers.ac3_pruning import analyze_ac3_pruning
from helpers.ac3_calls import analyze_ac3_calls
from helpers.runtimes import analyze_runtimes
from helpers.csp import analyze_csp
from helpers.utils import combine_metrics_files
from helpers.search import analyze_search

AC3_PRUNING_DIR = "ac3_pruning"
AC3_CALLS_DIR = "ac3_calls"
RUNTIMES_DIR = "runtimes"
CSP_DIR = "csp"
SEARCH_DIR = "search"

def main():
    # parse cli args
    parser = argparse.ArgumentParser(description='Analyze profiling JSON output files')
    parser.add_argument('data_path', help='Path to the JSON data file')
    args = parser.parse_args()

    # create output dir
    data_name, _ = os.path.splitext(os.path.basename(args.data_path))
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S%f")
    output_dir = f"{data_name}_{timestamp}"
    attempts = 10
    for i in range(attempts):
        try:
            os.makedirs(output_dir)
        except FileExistsError as e:
            if i < attempts - 1:
                continue
            else:
                raise FileExistsError("Failed to create output directory")
        break

    # create subdirs
    ac3_pruning_path = output_dir + '/' + AC3_PRUNING_DIR + '/'
    ac3_calls_path = output_dir + '/' + AC3_CALLS_DIR + '/'
    runtimes_path = output_dir + '/' + RUNTIMES_DIR + '/'
    csp_path = output_dir + '/' + CSP_DIR + '/'
    search_path = output_dir + '/' + SEARCH_DIR + '/'
    os.makedirs(ac3_pruning_path)
    os.makedirs(ac3_calls_path)
    os.makedirs(runtimes_path)
    os.makedirs(csp_path)
    os.makedirs(search_path)

    # open file
    try:
        with open(args.data_path, 'r') as file:
            data = json.load(file)
    except FileNotFoundError:
        print(f"Error: File '{args.data_path}' not found.")
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: Failed to parse JSON file '{args.data_path}'. {e}")
        sys.exit(1)

    analyze_ac3_pruning(data, ac3_pruning_path)
    analyze_ac3_calls(data, ac3_calls_path)
    analyze_runtimes(data, runtimes_path)
    analyze_csp(data, csp_path)
    analyze_search(data, search_path)

    combine_metrics_files([ac3_pruning_path, ac3_calls_path, runtimes_path, csp_path, search_path], output_dir + '/' + "metrics.md")
    print(f"Results for {args.data_path} written to new directory {output_dir}")

if __name__ == "__main__":
    main()
