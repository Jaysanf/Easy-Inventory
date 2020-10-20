import sys
import csv


def main():
    # gets argv and checks if usage was good
    argv = sys.argv
    
    if (len(argv) != 3):
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit()
    
    # open the csv files
    databases = open(f"{argv[1]}", "r")
    data = open(argv[2], "r")
    sequence = data.read()
    
    # get my keys for my dict
    keys = next(databases)
    keys = keys.split(",")
    # delete name     
    del keys[0]
    n = len(keys)
    
    # declare dictionarie of str
    str_count = {}
    
    # resets my pointer in the file to the beginning of the file
    databases.seek(0, 0)
    
    # remove the \n of last key
    keys[-1] = keys[-1].rstrip("\n")
    
    reader = csv.DictReader(databases) 
    
    # goes through the sequence of dna and appends to the dict the value for each STR
    for i in range(n):
        str_count[f"{keys[i]}"] = f"{str_repeated(sequence, keys[i])}"
    
    # loops through all the databases
    for i in reader:
        # loops through all the keys
        for key in keys:
            if i[f'{key}'] != str_count[f'{key}']:
                break
                
        # if loop goes without breaking, then its this guy
        else:
            
            print(i['name'])
            # closes the file and exit the programm
            databases.close()
            data.close()
            exit()
            
    # no one was found:
    print("No match")
    
    # closes the file and exit the programm
    databases.close()
    data.close()
    exit()
    
    
def str_repeated(dna, key):
    # func that finds the biggest STR
    
    # start of the search
    start = 0
    
    # Declare variable of highest found
    highest = 0
    
    # Lenght of the key and dna
    l = len(key)
    ld = len(dna)
    
    # declare pos
    pos = 0
    
    while True:
        # Finds next key
        pos = dna.find(key, start)
        
        # Breaks if no more key found
        if pos == - 1:
            return highest
            
        # Declare counter variable and initialize it at 0
        counter = 0
        
        # loops that goes from the pos through the end of string
        # and has a step of lenght of key
        for i in range(pos, ld, l):
            
            # Checks if we have another key following the last one
            if dna[(i): (i + l)] == key:
                counter += 1
                
            # if no: break
            else:
                # Checks if counter higher
                if counter > highest:
                    highest = counter
                    
                start = i
                break
            start = i + l + 1

        
main()