import csv
import sys


def main():
    # make a dict ot store the match numbers of each STR
    STR_match = dict()
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Incorrect number of command-line arguments")
        sys.exit(1)

    # TODO: Read database file into a variable
    # Read the STRs need to match
    with open(sys.argv[1], 'r') as STRfile:
        reader = csv.reader(STRfile)
        STRs = next(reader)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as DNAfile:
        DNA = DNAfile.read()

    # TODO: Find longest match of each STR in DNA sequence
    for STR in STRs[1:]:
        STR_match[STR] = longest_match(DNA, STR)

    # TODO: Check database for matching profiles
    with open(sys.argv[1], 'r') as STRfile:
        reader = csv.DictReader(STRfile)
        for row in reader:
            for STR in STRs[1:]:
                if row[STR] != str(STR_match[STR]):
                    break
                # Check if the whole STR numbers matched
                if (STR == STRs[-1]):
                    print(row["name"])
                    return
        print("No match")
        return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run

main()
