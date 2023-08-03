#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <conio.h> 
// Our custom math functions
#include "calc.h"

// Declaring the screen size, really its the buffer size.
const int screenWidth = 100;
const int screenHeight = 100;

// Frame buffer to store the char, position and color.
std::vector<std::vector<CHAR_INFO>> frameBuffer(screenHeight, std::vector<CHAR_INFO>(screenWidth, { ' ', 7 }));
// Might need a z-depth buffer so we only render what is close to the camera in depth

// TEMPORARY
// Points, Edges, Verticies


// Function to set the cursor position in the console
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to draw a character at a specific position with a specific color to the framebuffer
void drawToBuffer(int x, int y, char ch, int color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        frameBuffer[y][x].Char.AsciiChar = ch;
        frameBuffer[y][x].Attributes = color;
    }
}

// Function to render the entire frame from the framebuffer using the Windows Console API
void renderFrame() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Create a temporary buffer to store the ASCII characters and colors
    // Without this temp buffer, console goes sicko-mode
    std::vector<CHAR_INFO> tempBuffer;
    for (const auto& row : frameBuffer) {
        tempBuffer.insert(tempBuffer.end(), row.begin(), row.end());
    }

    COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };

    WriteConsoleOutput(hConsole, tempBuffer.data(), bufferSize, bufferCoord, &writeRegion);
}

// Dump the frame buffer for the next frame
void clearFramebuffer() {
    for (auto& row : frameBuffer) {
        for (auto& cell : row) {
            cell.Char.AsciiChar = ' ';
            cell.Attributes = 7; // Default color
        }
    }
}

// Simple function to make debugging values in real time easier.
void printValue(const std::string& name, double value, int x, int y) {
    // Break the string into Chars to place into buffer
    for (size_t i = 0; i < name.size(); i++) {
        drawToBuffer(x + i, y, name[i], 14); // Print each character of the name
    }

    // Convert the value to a string and print it at the specified x, y position
    std::string valueString = std::to_string(value);
    for (size_t i = 0; i < valueString.size(); i++) {
        drawToBuffer(x + name.size() + 1 + i, y, valueString[i], 15);
    }
}

int main() {
    system("cls"); // Clear the screen
    std::cout << "Dont forget to press a key dumbass... \n";
    _getch(); // Wait for key press to start so I can record :)

    std::array<std::array<double, 4>, 4> result1 = mat4x4(); // Identity matrix (by default)
    std::array<std::array<double, 4>, 4> result2 = mat4x4(false); // Identity matrix
    std::array<std::array<double, 4>, 4> result3 = mat4x4(true); // Non-identity matrix

    // Output the matrix elements for result1 (identity matrix by default)
    std::cout << "Resulting matrix 1: " << std::endl;
    for (int i = 0; i < result1.size(); ++i) {
        for (int j = 0; j < result1[i].size(); ++j) {
            std::cout << result1[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Output the matrix elements for result2 (identity matrix)
    std::cout << "Resulting matrix 2: " << std::endl;
    for (int i = 0; i < result2.size(); ++i) {
        for (int j = 0; j < result2[i].size(); ++j) {
            std::cout << result2[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Output the matrix elements for result3 (non-identity matrix)
    std::cout << "Resulting matrix 3: " << std::endl;
    for (int i = 0; i < result3.size(); ++i) {
        for (int j = 0; j < result3[i].size(); ++j) {
            std::cout << result3[i][j] << " ";
        }
        std::cout << std::endl;
    }


    /*
    // Initialing values for FPS calculation
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();


    // Begin main render loop
    for (int frame = 0; frame < 120; frame++) {
        for (int y = 0; y < 50; y++) {
            drawToBuffer(frame, y+1, 'X', 14); // Placeholder, draws an * each frame to the right.
        }
        
        // Calculate frame rate
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        double fps = static_cast<double>(frameCount) / (duration / 1000.0);
        frameCount = 0;
        startTime = currentTime;

        // Draw the FPS at position (1, 1) with white color (15)
        printValue("FPS: ", fps, 0, 0);

        // Render the frame directly to the console
        renderFrame();
        // Clear buffer
        clearFramebuffer();

        // Slow down MF I dont need 9 million FPS.
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    */


    return 0;
}
