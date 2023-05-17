#include <iostream>
#include <windows.h>

int main()
{
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the console screen buffer information
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Calculate the number of columns in the console
    int consoleColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Print a character to fill up the row
    char fillChar = '*';
    for (int i = 0; i < consoleColumns; i++)
    {
        std::cout << fillChar;
    }

    return 0;
}

