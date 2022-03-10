from cs50 import get_string

# Get card number that is at most 16 characters long
while True:
    card = get_string("Number: ")
    if len(card) <= 16:
        break

# Initialize variables
length = len(card) - 1
total = 0

while True:
    if length < 1:
        break

    # Take every other number from the back and multiply by 2
    number = str(int(card[length - 1])* 2)

    # Manipulate strings into int
    if len(number) > 1:
        total += int(number[0]) + int(number[1]) + int(card[length])
    else:
        total += int(number[0]) + int(card[length])

    # Do in pairs
    length -= 2

# Add the first number of len() is odd
if len(card) % 2 == 1:
    total += int(card[0])

# Check for algorithm
if total % 10 != 0:
    print("INVALID")
elif len(card) == 15 and card[0] == "3" and (card[1] == "4" or "7"):
    print("AMEX")
elif len(card) == 16 and card[0] == "5" and int(card[1]) < 6:
    print("MASTERCARD")
elif (len(card) == 13 or 16) and card[0] == "4":
    print("VISA")
else:
    print("INVALID")