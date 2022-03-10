from sys import argv
from cs50 import SQL

# Ensure that command line argument is at least 2
if len(argv) != 2:
    print("Usage: python [file.py] 'house_name'")
    exit()

# Open db file for SQLite
db = SQL("sqlite:///students.db")

# Read database for house into memory
dictionary = db.execute(f"SELECT first, middle, last, birth FROM students WHERE house = '{argv[1]}' ORDER BY last, first")

# Iterate through every row of the dictionary
for row in dictionary:
    first = row['first']
    last = row['last']
    birth = row['birth']

    # Check if middle is null
    if (row['middle'] != None):
        middle = row['middle']
        print(f"{first} {middle} {last}, born {birth}")
        continue
    else:
        print(f"{first} {last}, born {birth}")