import json
import argparse
import sys
import os

from helpers.ac3_pruning import analyze_ac3_pruning

def main():
    # parse cli args
    parser = argparse.ArgumentParser(description='Analyze profiling JSON output files')
    parser.add_argument('data_path', help='Path to the JSON data file')
    parser.add_argument('output_dir', help='Name of output directory')
    args = parser.parse_args()

    # create output dir
    if not args.output_dir:
        print(f"Error: Output directory name cannot be empty.")
        sys.exit(1)
    else:
        try:
            os.makedirs(args.output_dir)
        except FileExistsError:
            raise FileExistsError(f"Directory '{args.output_dir}' already exists!")

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

    # analyze_ac3_pruning_durations(data, args.output_dir + "/ac3_pruning")
    analyze_ac3_pruning(data)

if __name__ == "__main__":
    main()
