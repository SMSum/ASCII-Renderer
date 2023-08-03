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

// Funny character array for "shading"
const std::string charString = "@#W$9876543210?!abc;:+=-,._";
const int totalShades = charString.length();
// Declaring the screen size, really its the buffer size.
const int screenWidth = 100;
const int screenHeight = 100;
// Initilize the perspective, and tools for translation/rotation
const int FOV = 60;
const int rotationSpeed = 1;
const std::array<std::array<double, 4>, 4> p = perspective(screenWidth, screenHeight, 0.05, 100, FOV);
const std::array<std::array<double, 4>, 4> l = translate(0, 0, 10);
const std::array<std::array<double, 4>, 4> r = rotateYXZ(0, 0, 0);
// Points, Edges, Verticies, and Size
const double cubeSize = 2.5;
// Cube Vertexes
const std::array<std::array<double, 4>, 8> cubePoints = { {
    vec4(-cubeSize, -cubeSize, -cubeSize),
    vec4(cubeSize, -cubeSize, -cubeSize),
    vec4(cubeSize, -cubeSize, cubeSize),
    vec4(-cubeSize, -cubeSize, cubeSize),
    vec4(-cubeSize, cubeSize, -cubeSize),
    vec4(cubeSize, cubeSize, -cubeSize),
    vec4(cubeSize, cubeSize, cubeSize),
    vec4(-cubeSize, cubeSize, cubeSize)
} };
// Cube Edge Pairs
const std::vector<std::pair<int, int>> cubeEdges = {
    {1, 2}, {2, 3}, {3, 4}, {4, 1},
    {5, 6}, {6, 7}, {7, 8}, {8, 5},
    {1, 5}, {2, 6}, {3, 7}, {4, 8}
};
// Cube Faces
const std::vector<std::vector<int>> cubeFaces = {
    {1, 2, 3, 4}, // Bottom face
    {4, 3, 7, 8}, // Top face
    {2, 6, 7, 3}, // Front face
    {8, 7, 6, 5}, // Right face
    {4, 8, 5, 1}, // Back face
    {5, 6, 2, 1}  // Left face
};
// Light Source Location
const std::array<double, 3> lightSource = { 0.0, 0.0, -cubeSize * 205.0 };



// Frame buffer to store the char, position and color.
std::vector<std::vector<CHAR_INFO>> frameBuffer(screenHeight, std::vector<CHAR_INFO>(screenWidth, { ' ', 7 }));
// Might need a z-depth buffer so we only render what is close to the camera in depth

// TEMPORARY



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

// BEGIN FUNNY MATH

double calculateShading() {
    return 1.0;
}

void drawQuadrilateral() {

}

boolean isPointInQuadrilateral() {
    return true;
}

void connectPoints(int i1, int i2, int i3, int i4) {

    std::array<std::array<double, 4>, 4> point1 = mul(p, l);
    point1 = mul(point1, r);
    // Print the values of point1
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << point1[i][j] << " ";
        }
        std::cout << std::endl;
    }
    point1 = mul(point1, cubePoints[i1]);
    std::array<double, 4> result = divW(point1);
  

}


int main() {
    system("cls"); // Clear the screen
    std::cout << "Dont forget to press a key dumbass... \n";
    _getch(); // Wait for key press to start so I can record :)

    for (const auto& cubeFace : cubeFaces) {
        connectPoints(cubeFace[0], cubeFace[1], cubeFace[2], cubeFace[3]);
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
