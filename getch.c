#include <stdio.h>

#ifdef _WIN32
#  include <conio.h>
#else // !_WIN32
#  include <termios.h>
#  include <unistd.h>
#endif // _WIN32

#ifdef _WIN32
// On Windows, getch() is defined in conio.h
#else // !_WIN32
int getch() {
    struct termios oldattr = { 0 };
    struct termios newattr = { 0 };
    int            ch      = 0;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#endif // _WIN32

// Example usage
int main() {
    printf("Press any key: ");
    int c = getch();
    printf("\nYou pressed: %c\n", c);
    return 0;
}
