def main():
    x = positive_int()
    for i in range(1, x + 1):
        print((" " * (x - i) + ("#" * i) + "  " +  ("#" * i)))
        
    
    
    
def positive_int():
    while True:
        x = int(input("Height: "))
        if 0 < x <= 8:
            return x

            
            
main()

