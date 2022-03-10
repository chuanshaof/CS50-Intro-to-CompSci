from sys import argv
from csv import DictReader

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")

# Open CSV file and read into memory, and close after
with open(f'{argv[1]}', 'r') as csv:
    database = list(DictReader(csv))
    header = database[0]

# Open TXT file and read as a string, and close after
with open(f'{argv[2]}','r') as txt:
    sequence = txt.read()

# Set up a dictionary to save the highest consecutive sequences
saved = {}

# Loop through the different strings of STRs
for STRs in header:

    # Initialize integers
    i, j, repeats = 0, 0, 0
    L = len(STRs)
    SeqL = len(sequence)

    # Loop to check if STR, plussed one due to sequence and [:] function
    while i + L < SeqL + 1:
        if STRs in sequence[i:i + L]:
            j += 1
            i += L

            # Check if the unique STR is consecutive & save the highest repetition
            if STRs not in sequence[i:i + L]:
                if j > repeats:
                    repeats = j
                # Initialize j back to 0 for new back to back STRs
                j = 0

        # If sequence does not match, go to next
        else:
            i += 1

    # Save highest number of repeats into dictionary for a particular STR
    saved[STRs] = str(repeats)


# Loop through the various rows of the dictionary
for i in range(len(database)):
    count = 1
    # Set to compare each STR to the database
    for STRs in header:
        if STRs == 'name':
            continue
        elif saved[STRs] == database[i][STRs]:
            count += 1
        else:
            break

    # If reach END of header with all matches, print the name
    if count == len(header):
        print(database[i]['name'])
        break
    # Else if reach the rows print "No Match"
    elif i == len(database) - 1:
        print("No Match")

