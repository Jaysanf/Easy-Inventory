from cs50 import SQL
import sys
import csv

# Variable called db to run query on students.db
db = SQL("sqlite:///students.db")


# Checks if user put an argument
if len(sys.argv) != 2:
    print("USAGE: python import.py [FILE.csv]")
    exit()

# Opens CSV file
with open(f'{sys.argv[1]}', 'r') as csvfile:
    characters = csv.reader(csvfile)
    # Go through each line of csv
    for row in characters:
        # Skip for first line
        if row == ['name', 'house', 'birth']:
            continue
        
        # Split the name
        row[0] = row[0].split(" ")
        
        # Adds NULL if no middle Name
        if len(row[0]) != 3:
            row[0].insert(1, None)
            
        db.execute(
            f"INSERT INTO students (first, middle, last, house, birth) VALUES ('{row[0][0]}', '{row[0][1]}', '{row[0][2]}', '{row[1]}', '{row[2]}' )")
        print(row)