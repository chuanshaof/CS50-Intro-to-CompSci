#include <stdio.h>
#include <cs50.h>
#include <math.h>

long getcard(void); //Prompt for card within range
int getdigit(long card, int X); //Get (X)th digit from the right

int main(void)
{
    long card = getcard(); //Prompt for card within range


    int j, doubleeven, even = 0; //Gather every other digit
    for (j = 1; j < 17; j++)
    {
        if (j % 2 == 0) //Make sure they are 'every other'
        {
            doubleeven = 2 * getdigit(card, j);
            even += (getdigit(doubleeven, 1) + getdigit(doubleeven, 2)); //Split them and sum
        }
    }

    int i, odd = 0; //Gather the odd digits
    for (i = 1; i < 17; i++)
    {
        if (i % 2 == 1) //Make sure they are odd
        {
            odd += getdigit(card, i); //Sum the odds
        }
    }

    int checksum = even + odd;
    if (checksum % 10 != 0) //Check for Luhn's Algorithm
    {
        printf("INVALID\n");
    }
    else if (getdigit(card, 15) == 3 && ((getdigit(card, 14) == 4) || (getdigit(card, 14) == 7))) //Check for AMEX
    {
        printf("AMEX\n");
    }
    else if (getdigit(card, 16) == 5 && (getdigit(card, 15) < 6)) //Check for MASTERCARD
    {
        printf("MASTERCARD\n");
    }
    else if (getdigit(card, 16) == 4 || getdigit(card, 13) == 4) //Check for VISA
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

long getcard(void)
{
    long card; // Prompt for a card number within 16 digits
    do
    {
        card = get_long("Card number: ");
    }
    while (card > 9999999999999999); // Card does not exceed 16 characters
    return card;
}

int getdigit(long card, int X) //Function to show (X)th number from the right
{
    long tenpow = 1;
    for (int i = 1; i < X; i++) //X is the (X)th number from the right
    {
        tenpow *= 10;
    }
    card /= tenpow; //Reduce card number to (X)
    card %= 10; //Get last digit
    return card;
}