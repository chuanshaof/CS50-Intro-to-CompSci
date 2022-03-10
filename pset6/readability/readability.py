from cs50 import get_string

text = get_string("Text: ")

words = 1
sentences = 0
letters = 0
i = 0

while True:
    if i > len(text) - 1:
        break

    if " " in text[i]:
        words += 1
    elif text[i].isalpha():
        letters += 1
    elif text[i] == '!' or text[i] == '.'  or text[i] == '?':
        sentences += 1
    i += 1

L = 100 * letters / words
S = 100 * sentences / words
index = round(0.0588 * L - 0.296 * S - 15.8)

if index <= 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")