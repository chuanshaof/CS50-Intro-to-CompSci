#include<stdio.h>
#include<cs50.h>
#include<math.h>

int main(void)
{
    float cash;
    int cents, quarters, dimes, nickels, pennies, coins;
    do
    {
        cash = get_float("Change owed: ");// Get coins
    }
    while(cash < 0); // Ensure that the value entered is non-negative
    cents = round(cash*100); //Print cash into cents

    quarters = cents/25; // Get number of quarters
    dimes = (cents%25)/10; // Get number of dimes
    nickels = (cents%25%10)/5; // Get number of nickels
    pennies = (cents%25%10%5)/1; // Get number of pennies

    coins = quarters + dimes + nickels + pennies; // Sum total number of coins

    printf("%i\n", coins);
}