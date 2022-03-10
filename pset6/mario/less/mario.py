from cs50 import get_int

while True:
    block = get_int("Height: ")
    if block > 0 and block < 9:
        break

increase = 1
decrease = block - 1

for i in range(block):
    for j in range(decrease):
        print(" ", end = "")
    for j in range(increase):
        print("#", end = "")
    increase += 1
    decrease -= 1
    print()