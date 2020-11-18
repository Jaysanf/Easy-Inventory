from cs50 import SQL
import sys


# Variable called db to run query on students.db
db = SQL("sqlite:///students.db")

# Checks if user put an argument and argument is an house
if len(sys.argv) != 2 or sys.argv[1] not in ["Gryffindor", "Hufflepuff", "Slytherin", "Ravenclaw"]:
    print("USAGE: python import.py [NAME OF GIVEN HOUSE]")
    exit()
    
# declare variable    
house = sys.argv[1]

# Create dict for  all house member
characters = db.execute(f"SELECT first, middle, last, birth FROM students WHERE house = '{house}' ORDER BY last, first")

for row in characters:
    if row['middle'] == 'None':
        row['middle'] = ""
    else: 
        row['middle'] = f"{row['middle']} " 
    print(f"{row['first']} {row['middle']}{row['last']}, born {row['birth']}")