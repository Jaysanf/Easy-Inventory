import cs50


# prompt the user for a card id
card = cs50.get_int("Number: ")

# puts the card number in a list
card_list = [int(i) for i in str(card)]

# gets lenght
length = len(card_list)

total = 0

# loops through cards
for i in range(length):
    # check if number [odd  or even] 
    if ((i % 2) == 0):
        # check if lenght [odd or even]
        if ((length % 2) == 0):
            # luhn algorithm on every other number checking if over 10
            total += (card_list[i] * 2) // 10
            total += (card_list[i] * 2) % 10
        # if odd card just add up        
        else:

            total += card_list[i]
    else:
        # check if lenght [odd or even]
        if ((length % 2) == 0):
            total += card_list[i]

        # if odd card just add up        
        else:
            # luhn algorithm on every other number checking if over 10
            total += (card_list[i] * 2) // 10
            total += (card_list[i] * 2) % 10


# checks if luhn algoritm works
if (total % 10) == 0:
    # gets first 2 number
    first_two = card_list[0] * 10 + card_list[1] 
    
    # checks for AMEX
    if first_two in [34, 37]:
        print("AMEX")
        
    # checks for mastercard
    elif first_two in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
    
    elif card_list[0] == 4:
        print("VISA")
    
    else:
        print("INVALID")
else:
    print("INVALID")            

