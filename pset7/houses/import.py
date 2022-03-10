from sys import argv
from csv import reader
from cs50 import SQL

# Ensure that command line argument is at least 2
if len(argv) != 2:
    print("Usage: python [file.py] [file.csv]")
    exit()

# Refresh file
open("students.db", "w").close()

# Open db file for SQLite
db = SQL("sqlite:///students.db")

# Create table in database
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# Open command line argument
with open(argv[1], "r") as CSV:
    # Read data into a list
    raw = list(reader(CSV))

    # Iterate through every row of the list
    for row in raw[1:]:
        # Set information in row
        house = row[1]
        birth = int(row[2])

        # Split name into various columns
        name = row[0].split()
        if (len(name) == 3):
            first = name[0]
            middle = name[1]
            last = name[2]
        else:
            first = name[0]
            middle = None
            last = name[1]

        # Insert information into selected table
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ? , ? ,?)", first, middle, last, house, birth)