import cs50


def main():
    # prints out a pyramid
    x = positive_int()
    for i in range(1, x + 1):
        print((" " * (x - i) + ("#" * i) + "  " + ("#" * i)))
    
    
def positive_int():
    # gets an int between 1 and 8
    while True:
        x = cs50.get_int("Height: ")
        if 0 < x <= 8:
            return x


main()

