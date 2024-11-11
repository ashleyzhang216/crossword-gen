import json
import matplotlib
import matplotlib.pyplot as plt
import argparse
import sys

def collect_durations(node, durations):
    # Check if the current node has the type 'AC3' and collect duration_us
    if node.get('type') == 'AC3' and 'duration_us' in node:
        durations.append(node['duration_us'])
        assert(len(node.get('children', [])) <= 1)
    # Recursively traverse the children if they exist
    for child in node.get('children', []):
        collect_durations(child, durations)

def main():
    # Set up command-line argument parsing
    parser = argparse.ArgumentParser(description='Analyze AC3 durations in JSON data.')
    parser.add_argument('data_path', help='Path to the JSON data file.')
    parser.add_argument('output_image', help='Filename for the output histogram image.')
    args = parser.parse_args()

    # Load the JSON data from the specified file
    try:
        with open(args.data_path, 'r') as file:
            data = json.load(file)
    except FileNotFoundError:
        print(f"Error: File '{args.data_path}' not found.")
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: Failed to parse JSON file '{args.data_path}'. {e}")
        sys.exit(1)

    # List to store duration_us values
    durations = []
    # Start the recursive traversal
    collect_durations(data, durations)

    # Compute the average duration
    if durations:
        average_duration = sum(durations) / len(durations)
        print(f"Average duration_us for type 'AC3': {average_duration}")
        print(f"Number of AC3 events: {len(durations)}")
    else:
        print("No durations found for type 'AC3'.")

    # Plot the histogram and save it to the specified output file
    plt.hist(durations, bins=20, edgecolor='black')
    plt.xlabel('duration_us')
    plt.ylabel('Frequency')
    plt.title('Histogram of duration_us values for type AC3')
    plt.tight_layout()  # Adjust layout to prevent clipping
    plt.savefig(args.output_image)
    print(f"Histogram saved as '{args.output_image}'")

if __name__ == "__main__":
    main()
