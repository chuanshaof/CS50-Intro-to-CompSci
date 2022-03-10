from cs50 import get_float

# Get float that is not negative
while True:
    change = get_float("Change owed: ")
    if change >= 0:
        break

cents = round(change * 100)

quarters = int(cents / 25)
dimes = int((cents % 25) / 10)
nickels = int((cents % 25 % 10) / 5)
pennies = int((cents % 25 % 10 % 5) / 1)

coins = quarters + dimes + nickels + pennies

print(f"{coins}")