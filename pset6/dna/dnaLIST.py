from sys import argv
from csv import reader

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")

# Open CSV file and read into memory, and close after
with open(f'{argv[1]}', 'r') as csv:
    database = list(reader(csv))
    header = database[0]

# Open TXT file and read as a string, and close after
with open(f'{argv[2]}','r') as txt:
    sequence = txt.read()

# Set up an array to save the highest consecutive STR count
array = []

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

    # Save highest number of repeats into array for a particular STR
    array.append(str(repeats))

# Check the saved array to the individual arrays
for i in range(len(database)): 
    if array[1:len(header)] == database[i][1:len(header)]:
        print(database[i][0])
        break
    elif i == len(database) - 1:
        print("No Match")