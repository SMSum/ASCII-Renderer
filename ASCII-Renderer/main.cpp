#define NOMINMAX 
//Windows.h causes problems trying to use min/max. 
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <conio.h> 
#include <cmath>


// Our custom math functions
#include "calc.h"

// Funny character array for "shading"
//@#W$9876543210?!abc;:+=-,._
const std::string charString = "@#W$9876543210?!abc;:+=-,._";
const int totalShades = charString.length();
// Declaring the screen size, really its the buffer size.
const int screenWidth = 300;
const int screenHeight = 180;
// Initilize the perspective, and tools for translation/rotation
const int FOV = 90;
const double rotationSpeed = 0.1;
const std::array<std::array<double, 4>, 4> p = perspective(screenWidth, screenHeight, 0.05, 100, FOV);
std::array<std::array<double, 4>, 4> l = translate(0, 0, 10);
std::array<std::array<double, 4>, 4> r = rotateYXZ(0, 0, 0);
// Points, Edges, Verticies, and Size
const double cubeSize = 3;
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
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};
// Cube Faces
const std::vector<std::vector<int>> cubeFaces = {
    {0, 1, 2, 3}, // Bottom face
    {3, 2, 6, 7}, // Top face
    {1, 5, 6, 2}, // Front face
    {7, 6, 5, 4}, // Right face
    {3, 7, 4, 0}, // Back face
    {4, 5, 1, 0}  // Left face
};
// Light Source Location
std::array<double, 3> lightSource = { 0.0, 0.0, -cubeSize * 4 };
// Frame buffer to store the char, position and color.
std::vector<std::vector<CHAR_INFO>> frameBuffer(screenHeight, std::vector<CHAR_INFO>(screenWidth, { ' ', 7 }));

// NOTES
// Might need a z-depth buffer so we only render what is close to the camera in depth
// Revamp lighting system to be "per pixel"
// Might need to convert from double to float for accuracy reasons, we're missing 8 decimal places lol

// TESTING

// Function to set the cursor position in the console
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// Function to draw a character at a specific position with a specific color to the framebuffer
void drawToBuffer(int x, int y, char ch, double color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        frameBuffer[y][x].Char.AsciiChar = ch;
        frameBuffer[y][x].Attributes = color;
    }
}
// Simple function to make debugging values in real time easier.
void printValue(const std::string& name, double value, int x, int y) {
    // Break the string into Chars to place into buffer
    for (size_t i = 0; i < name.size(); i++) {
        drawToBuffer(x + i, y, name[i], 7); // Print each character of the name
    }

    // Convert the value to a string and print it at the specified x, y position
    std::string valueString = std::to_string(value);
    for (size_t i = 0; i < valueString.size(); i++) {
        drawToBuffer(x + name.size() + 1 + i, y, valueString[i], 7);
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
// Clear the frame buffer for the next frame
void clearFramebuffer() {
    for (auto& row : frameBuffer) {
        for (auto& cell : row) {
            cell.Char.AsciiChar = ' ';
            cell.Attributes = 7; // Default color
        }
    }
}
// Takes shading and outputs the correct character for the value
char calculateCharFromShading(double shading) {

    char resultChar = charString[static_cast<int>(shading * totalShades)];

    return resultChar;
}

// Rough calculation of how "shaded" a normalized face is
double calculateShading(const std::array<double, 4>& normal, const std::array<double, 4>& lightVector) {
    double dotProduct = dot(normal, lightVector);
    double normalLength = length(normal);
    double lightVectorLength = length(lightVector);
    double angle = std::acos(dotProduct / (normalLength * lightVectorLength));
    double shading = std::cos(angle);
    printValue("Shading", shading, 0, 4);
    return shading;
}
// Check if we are in or out
bool isPointInQuadrilateral(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    // Get signed area of square (2 triangles) to check if x,y is inside or outside
    auto sign = [](double p1x, double p1y, double p2x, double p2y, double p3x, double p3y) {
        return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
    };

    bool b1 = sign(x, y, x1, y1, x2, y2) < 0.0;
    bool b2 = sign(x, y, x2, y2, x3, y3) < 0.0;
    bool b3 = sign(x, y, x3, y3, x4, y4) < 0.0;
    bool b4 = sign(x, y, x4, y4, x1, y1) < 0.0;

    return b1 == b2 && b2 == b3 && b3 == b4;
}
// Scanline fill our faces
void drawQuadrilateral(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double shading) {
    // Find our largest and smallest X, Y
    double minX = std::min({ x1, x2, x3, x4 });
    double maxX = std::max({ x1, x2, x3, x4 });
    double minY = std::min({ y1, y2, y3, y4 });
    double maxY = std::max({ y1, y2, y3, y4 });

    for (double y = minY; y <= maxY; ++y) {
        // Calculate largest and smallest double possible (math.huge/-math.huge)
        constexpr double maxDouble = std::numeric_limits<double>::max();
        constexpr double minDouble = std::numeric_limits<double>::lowest();
        // We cant directly set x_left and x_right to min/max, better to handle those vars at compile instead since they're fixed.
        double x_left = maxDouble;
        double x_right = minDouble;
        
        for (double x = minX; x <= maxX; ++x) {
            //Check if inside the quadrilateral so we dont draw outside the shape
            if (isPointInQuadrilateral(x, y, x1, y1, x2, y2, x3, y3, x4, y4)) { 
                x_left = std::min(x_left, x);
                x_right = std::max(x_right, x);
            }
        }
        for (double x = x_left; x <= x_right; ++x) {
            drawToBuffer(x, y, calculateCharFromShading(shading), 7);
        }
    }
}
// Connect our points into edges to scanline fill
void connectPoints(int i1, int i2, int i3, int i4) {
    // vertex normalization from homogenous to cartesian
    std::array<std::array<double, 4>, 4> point1 = mul(p, l);
    point1 = mul(point1, r);
    std::array<double, 4> point1_vec4 = mul(point1, cubePoints[i1]);
    std::array<double, 4> point1_normalized = divW(point1_vec4);

    std::array<std::array<double, 4>, 4> point2 = mul(p, l);
    point2 = mul(point2, r);
    std::array<double, 4> point2_vec4 = mul(point2, cubePoints[i2]);
    std::array<double, 4> point2_normalized = divW(point2_vec4);

    std::array<std::array<double, 4>, 4> point3 = mul(p, l);
    point3 = mul(point3, r);
    std::array<double, 4> point3_vec4 = mul(point3, cubePoints[i3]);
    std::array<double, 4> point3_normalized = divW(point3_vec4);

    std::array<std::array<double, 4>, 4> point4 = mul(p, l);
    point4 = mul(point4, r);
    std::array<double, 4> point4_vec4 = mul(point4, cubePoints[i4]);
    std::array<double, 4> point4_normalized = divW(point4_vec4);

    // Create our 4 x, y, z cordinates for upcoming funny math pt.2
    double x1 = point1_normalized[0];
    double y1 = point1_normalized[1];
    double z1 = 1 - point1_normalized[2];

    double x2 = point2_normalized[0];
    double y2 = point2_normalized[1];
    double z2 = 1 - point2_normalized[2];

    double x3 = point3_normalized[0];
    double y3 = point3_normalized[1];
    double z3 = 1 - point3_normalized[2];

    double x4 = point4_normalized[0];
    double y4 = point4_normalized[1];
    double z4 = 1 - point4_normalized[2];

    // Check crossproduct to see what direction the quadrilateral is (back-face culling)
    if (cross4(x1, y1, x2, y2, x3, y3, x4, y4) > 0) {
        return;
    }
    // Begin our funny ass math
    if (x1 >= -1 && x1 <= 1 && y1 >= -1 && y1 <= 1 && z1 <= 0 &&
        x2 >= -1 && x2 <= 1 && y2 >= -1 && y2 <= 1 && z2 <= 0 &&
        x3 >= -1 && x3 <= 1 && y3 >= -1 && y3 <= 1 && z3 <= 0 &&
        x4 >= -1 && x4 <= 1 && y4 >= -1 && y4 <= 1 && z4 <= 0) {
        x1 = (x1 + 1) / 2 * screenWidth;
        y1 = (y1 + 1) / 2 * screenHeight;
        x2 = (x2 + 1) / 2 * screenWidth;
        y2 = (y2 + 1) / 2 * screenHeight;
        x3 = (x3 + 1) / 2 * screenWidth;
        y3 = (y3 + 1) / 2 * screenHeight;
        x4 = (x4 + 1) / 2 * screenWidth;
        y4 = (y4 + 1) / 2 * screenHeight;

        // Find our face centers for normalizing to bounce light around :)
        double faceCenterX = (x1 + x2 + x3 + x4) / 4.0;
        double faceCenterY = (y1 + y2 + y3 + y4) / 4.0;
        double faceCenterZ = (z1 + z2 + z3 + z4) / 4.0;

        // Create our two edges for scan fill coming soon
        std::array<double, 3> edge1 = { x2 - x1, y2 - y1, z2 - z1 };
        std::array<double, 3> edge2 = { x3 - x2, y3 - y2, z3 - z2 };

        // Normalize our edges
        edge1 = normalize(edge1);
        edge2 = normalize(edge2);

        // Get our face normal for s h a d i n g
        std::array<double, 3> faceNormal = cross2(edge1, edge2);
        faceNormal = normalize(faceNormal);

        // Normalize our light position as well
        std::array<double, 3> lightVector = { lightSource[0] - faceCenterX, lightSource[1] - faceCenterY, lightSource[2] - faceCenterZ };
        lightVector = normalize(lightVector);

        // Convert our Normals into Vectors
        std::array<double, 4> faceNormalVec4 = vec4(faceNormal[0], faceNormal[1], faceNormal[2]);
        std::array<double, 4> lightVectorVec4 = vec4(lightVector[0], lightVector[1], lightVector[2]);

        // Calculate shading "value"
        double shading = calculateShading(faceNormalVec4, lightVectorVec4);

        // Call drawQuadrilateral
        drawQuadrilateral(x1, y1, x2, y2, x3, y3, x4, y4, shading);


    }
}

int main() {
    system("cls"); // Clear the screen
    std::cout << "Dont forget to press a key dumbass... \n";
    _getch(); // Wait for key press to start so I can record :)
    
    // Initialing values for FPS calculation
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();


    // Begin main render loop
    while (true) {

        r = mul(r, rotateYXZ(-rotationSpeed, rotationSpeed, 0));
        for (const auto& cubeFace : cubeFaces) {
            connectPoints(cubeFace[0], cubeFace[1], cubeFace[2], cubeFace[3]);
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
        printValue("Light X", lightSource[0], 0, 1);
        printValue("Light Y", lightSource[1], 0, 2);
        printValue("Light Z", lightSource[2], 0, 3);
        // Spin the cube
       
        // Render the frame directly to the console
        renderFrame();
        // Clear buffer
        clearFramebuffer();
        if (_kbhit()) {
            char key = _getch();
            if (key == 'q' || key == 'Q') {
                lightSource[1] -= 1.0 * cubeSize; // -Y
            }
            if (key == 'e' || key == 'E') {
                lightSource[1] += 1.0 * cubeSize; // +Y
            }
            if (key == 'w' || key == 'W') {
                lightSource[0] -= 1.0 * cubeSize; // -X
            }
            if (key == 's' || key == 'S') {
                lightSource[0] += 1.0 * cubeSize; // +X
            }
            if (key == 'a' || key == 'A') {
                lightSource[2] -= 1.0 * cubeSize; // -Z
            }
            if (key == 'd' || key == 'D') {
                lightSource[2] += 1.0 * cubeSize; // +Z
            }
        }


        // Input Testing
        /*
        if (_kbhit()) {
            char key = _getch();
            if (key == 'q' || key == 'Q') {
                l = mul(l, translate(0, 0, -rotationSpeed));
            }
            if (key == 'e' || key == 'E') {
                l = mul(l, translate(0, 0, rotationSpeed));
            }
            if (key == 'w' || key == 'W') {
                l = mul(l, translate(0, -rotationSpeed, 0));
            }
            if (key == 's' || key == 'S') {
                l = mul(l, translate(0, rotationSpeed, 0));
            }
            if (key == 'a' || key == 'A') {
                l = mul(l, translate(rotationSpeed, 0, 0));
            }
            if (key == 'd' || key == 'D') {
                l = mul(l, translate(-rotationSpeed, 0, 0));
            }
            if (key == 'a' || key == 'A') {
                l = mul(l, translate(rotationSpeed, 0, 0));
            }
            if (key == 'l' || key == 'L') {
                r = mul(r, rotateYXZ(rotationSpeed, 0, 0));
            }
            if (key == 'o' || key == 'O') {
                r = mul(r, rotateYXZ(-rotationSpeed, 0, 0));
            }
            if (key == 'k' || key == 'K') {
                r = mul(r, rotateYXZ(0, rotationSpeed, 0));
            }
            if (key == ';' || key == ':') {
                r = mul(r, rotateYXZ(0, -rotationSpeed, 0));
            }
        }
        */
    }
   
    return 0;
}
