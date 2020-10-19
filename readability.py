import cs50

# gets the text
text = cs50.get_string("Text: ")

# split where there is whitespace
x = text.split(" ")

# lenght = number of words
words = len(x)

# number of sentences
sentence = text.count("!") + text.count("?") + text.count(".") 

# number of letters
letter = 0
for i in text.lower():
    if i in "abcdefghijklmnopqrstuvwxyz":
        letter += 1


# avrg # of letter per 100 words
L = (100 * letter) / words

# avrg # of sentence per 100 words
S = (100 * sentence) / words

# Coleman-liau algo
index = (0.0588 * L) - (0.296 * S) - 15.8


if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")