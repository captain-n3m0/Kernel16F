#include "display.h"
void print(char *message)
{
    int i = 0;
    for (i = 0;; i++)
    {
        if (message[i] == 0)
            break;

        putchar(message[i]);
    }
}

static void print_digit(int digit)
{
    // We expect digits to be below ten
    if (digit > 9)
    {
        digit = 0;
    }

    putchar((char)(digit + 48));
}

void print_number(int number)
{
    int ten_thousands = 0;
    int thousands = 0;
    int hundreds = 0;
    int tens = 0;
    int units = 0;
    if (number <= 99)
    {
        tens = number / 10;
        units = number % 10;
        print_digit(tens);
        print_digit(units);
    }
    else if (number <= 999)
    {
        hundreds = number / 100;
        tens = number / 10 % 10;
        units = number % 10 % 10;
        print_digit(hundreds);
        print_digit(tens);
        print_digit(units);
    }
    else if(number <= 9999)
    {
        thousands = number / 1000;
        hundreds = number / 100 % 10;
        tens = number / 10 % 10;
        units = number % 10 % 10;
        print_digit(thousands);
        print_digit(hundreds);
        print_digit(tens);
        print_digit(units);
    }
    else
    {
        ten_thousands = number / 10000;
        thousands = (number % 10000) / 1000;
        hundreds = (number % 1000) / 10;
        tens = number / 10 % 10;
        units = number % 10 % 10;
        print_digit(ten_thousands);
        print_digit(thousands);
        print_digit(hundreds);
        print_digit(tens);
        print_digit(units);
    }
}
