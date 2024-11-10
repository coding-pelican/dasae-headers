// void Display_Render() {
//     write(STDOUT_FILENO, Display_bufferCurrent, Display_bufferCurrentSize);
// }

// // Define the display buffer using CHAR_INFO
// static CHAR_INFO Display_buffer[Display_Size];
// static HANDLE    hConsole = NULL;

// // Initialize the console handle and buffer
// void Display_Init() {
// #ifdef _WIN32
//     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     if (hConsole == INVALID_HANDLE_VALUE) {
//         perror("GetStdHandle failed");
//         exit(EXIT_FAILURE);
//     }

//     // Set up the console screen buffer
//     COORD      bufferSize = { .X = Display_Width, .Y = Display_Height };
//     SMALL_RECT windowSize = { .Left = 0, .Top = 0, .Right = Display_Width - 1, .Bottom = Display_Height - 1 };
//     SetConsoleScreenBufferSize(hConsole, bufferSize);
//     SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

//     // Disable the cursor for better performance
//     CONSOLE_CURSOR_INFO cursorInfo;
//     GetConsoleCursorInfo(hConsole, &cursorInfo);
//     cursorInfo.bVisible = FALSE;
//     SetConsoleCursorInfo(hConsole, &cursorInfo);
// #endif
// }

// void Display_SetBufferFromColors(const Color colors[Display_Size]) {
//     for (i32 y = 0; y < Display_Height; y++) {
//         for (i32 x = 0; x < Display_Width; ++x) {
//             i32   index = y * Display_Width + x;
//             Color color = colors[x + y * Display_Width];

//             // Set the character to a space with the desired foreground and background colors
//             Display_buffer[index].Char.AsciiChar = ' ';
//             Display_buffer[index].Attributes     = (color.r << 16) | (color.g << 8) | (color.b);
//         }
//     }
// }

// void Display_Render() {
// #ifdef _WIN32
//     COORD      bufferSize  = { .X = Display_Width, .Y = Display_Height };
//     COORD      bufferCoord = { .X = 0, .Y = 0 };
//     SMALL_RECT writeRegion = { .Left = 0, .Top = 0, .Right = Display_Width - 1, .Bottom = Display_Height - 1 };

//     // Write the buffer to the console
//     if (!WriteConsoleOutputA(
//             hConsole,
//             Display_buffer,
//             bufferSize,
//             bufferCoord,
//             &writeRegion
//         )) {
//         perror("WriteConsoleOutputA failed");
//         exit(EXIT_FAILURE);
//     }
// #else
//     // Existing non-Windows rendering code
//     if (!fwrite(Display_buffer, sizeof(Display_bufferCurrent[0]), Display_bufferCurrentSize, stdout)) {
//         assert(false);
//     }
// #endif
// }

// void Display_Clear() {
//     memset(Display_buffer, 0, sizeof(Display_buffer));
// }
