import json
import argparse
import sys
import os
from datetime import datetime

from helpers.ac3_pruning import analyze_ac3_pruning
from helpers.ac3_calls import analyze_ac3_calls
from helpers.runtimes import analyze_runtimes
from helpers.csp import analyze_csp
from helpers.utils import find_all_data_files, combine_metrics_files
from helpers.search import analyze_search

AC3_PRUNING_DIR = "ac3_pruning"
AC3_CALLS_DIR = "ac3_calls"
RUNTIMES_DIR = "runtimes"
CSP_DIR = "csp"
SEARCH_DIR = "search"

def main():
    # parse cli args
    parser = argparse.ArgumentParser(description='Analyze instrumentation output file(s)')
    parser.add_argument('data_path', help='Path to the data file or directory of files')
    args = parser.parse_args()

    # create output dir
    data_name, _ = os.path.splitext(os.path.basename(args.data_path))
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S%f")
    output_dir = f"analysis_report-{data_name}-{timestamp}"
    attempts = 10
    for i in range(attempts):
        try:
            os.makedirs(output_dir)
        except FileExistsError as e:
            if i < attempts - 1:
                continue
            else:
                raise FileExistsError("Failed to create output directory due to name collision")
        break

    # create subdirs
    ac3_pruning_path = os.path.join(output_dir, AC3_PRUNING_DIR)
    ac3_calls_path = os.path.join(output_dir, AC3_CALLS_DIR)
    runtimes_path = os.path.join(output_dir, RUNTIMES_DIR)
    csp_path = os.path.join(output_dir, CSP_DIR)
    search_path = os.path.join(output_dir, SEARCH_DIR)
    os.makedirs(ac3_pruning_path)
    os.makedirs(ac3_calls_path)
    os.makedirs(runtimes_path)
    os.makedirs(csp_path)
    os.makedirs(search_path)

    # find file(s)
    paths = find_all_data_files(args.data_path)

    # open file(s)
    datas = []
    for path in paths:
        try:
            with open(path, 'r') as file:
                datas.append(json.load(file))
                print(f"Parsed data from {path}") # TODO: DEBUG
        except FileNotFoundError:
            print(f"Error: File at '{path}' not found, skipping")
        except json.JSONDecodeError as e:
            print(f"Error: Skipping file at '{path}' due to parsing error: {e}")

    if(len(datas) == 0):
        print(f"Error: No valid JSON files found at '{args.data_path}'")
        sys.exit(1)

    data = datas[0] # TODO: DEBUG

    analyze_ac3_pruning(data, ac3_pruning_path)
    analyze_ac3_calls(data, ac3_calls_path)
    analyze_runtimes(data, runtimes_path)
    analyze_csp(data, csp_path)
    analyze_search(data, search_path)

    combine_metrics_files([ac3_pruning_path, ac3_calls_path, runtimes_path, csp_path, search_path], os.path.join(output_dir, "metrics.md"))
    print(f"Results for {args.data_path} written to new directory {output_dir}")

if __name__ == "__main__":
    main()
