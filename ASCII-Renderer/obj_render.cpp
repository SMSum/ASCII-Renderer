#define NOMINMAX 
//Windows.h causes problems trying to use min/max. 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <conio.h> 
#include <cmath>
#include <limits>

// Our custom math functions
#include "calc.h"


// Funny character array for "shading"
//@#W$9876543210?!abc;:+=-,._
const std::string charString = "_.,-=+:;cba!?0123456789$W#@";
const int totalShades = charString.length();
// Declaring the screen size, really its the buffer size.
const int screenWidth = 300;
const int screenHeight = 180;
// Frame buffer to store the char, position and color.
std::vector<CHAR_INFO> frameBuffer(screenWidth* screenHeight, { ' ', 7 });
// Z-Buffer for each pixel position
std::vector<float> zBuffer(screenWidth* screenHeight, std::numeric_limits<float>::max());
// Initilize the perspective, and tools for translation/rotation
const int FOV = 60;
const float rotationSpeed = 0.1;
const float modelScale = 40.0f;
const std::array<std::array<float, 4>, 4> p = perspective(screenWidth, screenHeight, 0.05f, 500, FOV);
std::array<std::array<float, 4>, 4> l = translate(10, -1, 15);
std::array<std::array<float, 4>, 4> r = rotateYXZ(-rotationSpeed, rotationSpeed, 0);

// IMPORTING
struct Vertex { 
    float x, y, z;
};
struct Face { //Define the outline of a Face
    int v1, v2, v3;
};
// Create storage for Verticies/Faces
std::vector<Vertex> vertices;
std::vector<Face> faces;
std::vector<std::vector<Vertex>> actualFaces;
// Function to handle reading the model and split into verticies/faces
void importModel(const std::string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces, float scale) {
    std::ifstream objFile(filename);

    if (!objFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;

    while (std::getline(objFile, line)) {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {  // Vertex
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertex.x *= scale;
            vertex.y *= scale;
            vertex.z *= scale;
            vertices.push_back(vertex);
        }
        else if (type == "f") {  // Face
            Face face;
            iss >> face.v1 >> face.v2 >> face.v3;
            faces.push_back(face);
        }
    }

    objFile.close();
}
// Function to handle creating our polygons
void createActualFaces(const std::vector<Vertex>& vertices, const std::vector<Face>& faces, std::vector<std::vector<Vertex>>& actualFaces) {
    for (const Face& face : faces) {
        std::vector<Vertex> actualFace;

        // Retrieve vertices using the indices from the face definition
        actualFace.push_back(vertices[face.v1 - 1]);
        actualFace.push_back(vertices[face.v2 - 1]);
        actualFace.push_back(vertices[face.v3 - 1]);

        actualFaces.push_back(actualFace);
    }
}

// RENDERING
// Function to draw a character at a specific position with a specific color to the framebuffer
void drawToBuffer(int x, int y, char ch, float color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        int index = y * screenWidth + x;
        frameBuffer[index].Char.AsciiChar = ch;
        frameBuffer[index].Attributes = color;
    }
}
// Simple function to make debugging values in real time easier.
void printValue(const std::string& name, float value, int x, int y) {
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

    COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };

    WriteConsoleOutput(hConsole, frameBuffer.data(), bufferSize, bufferCoord, &writeRegion);

    std::fill(frameBuffer.begin(), frameBuffer.end(), CHAR_INFO{ ' ', 7 });
    std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::max());
}
// Define to avoid constantly creating&calc
std::array<std::array<float, 4>, 4> mainMatrix = mul(p, l);
void processPolygon() {
    //rotation :)
    mainMatrix = mul(mainMatrix, r);

    for (const auto& actualFace : actualFaces) {
        const Vertex& vertex1 = actualFace[0];
        const Vertex& vertex2 = actualFace[1];
        const Vertex& vertex3 = actualFace[2];

        // Calculate point vectors and normalized values for specific vertices
        std::array<float, 4> point1_vec4 = mul(mainMatrix, vec4(static_cast<float>(vertex1.x), static_cast<float>(vertex1.y), static_cast<float>(vertex1.z)));
        std::array<float, 4> point1_normalized = divW(point1_vec4);
        float x1 = point1_normalized[0];
        float y1 = point1_normalized[1];
        float z1 = 1 - point1_normalized[2];

        std::array<float, 4> point2_vec4 = mul(mainMatrix, vec4(static_cast<float>(vertex2.x), static_cast<float>(vertex2.y), static_cast<float>(vertex2.z)));
        std::array<float, 4> point2_normalized = divW(point2_vec4);
        float x2 = point2_normalized[0];
        float y2 = point2_normalized[1];
        float z2 = 1 - point2_normalized[2];

        std::array<float, 4> point3_vec4 = mul(mainMatrix, vec4(static_cast<float>(vertex3.x), static_cast<float>(vertex3.y), static_cast<float>(vertex3.z)));
        std::array<float, 4> point3_normalized = divW(point3_vec4);
        float x3 = point3_normalized[0];
        float y3 = point3_normalized[1];
        float z3 = 1 - point3_normalized[2];

        //implement z-depth buffer instead of our dogwater backface culling shit

        // Perform the viewport transformation
        if (x1 >= -1 && x1 <= 1 && y1 >= -1 && y1 <= 1 && z1 <= 0 &&
            x2 >= -1 && x2 <= 1 && y2 >= -1 && y2 <= 1 && z2 <= 0 &&
            x3 >= -1 && x3 <= 1 && y3 >= -1 && y3 <= 1 && z3 <= 0) {
            x1 = (x1 + 1) / 2 * screenWidth;
            y1 = (y1 + 1) / 2 * screenHeight;
            x2 = (x2 + 1) / 2 * screenWidth;
            y2 = (y2 + 1) / 2 * screenHeight;
            x3 = (x3 + 1) / 2 * screenWidth;
            y3 = (y3 + 1) / 2 * screenHeight;

            // Find the bounding box of the polygon
            float minX = std::min({ x1, x2, x3 });
            float maxX = std::max({ x1, x2, x3 });
            float minY = std::min({ y1, y2, y3 });
            float maxY = std::max({ y1, y2, y3 });

            for (int y = minY; y <= maxY; ++y) {
                float alpha_denom = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
                float beta_denom = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);

                for (int x = minX; x <= maxX; ++x) {
                    float alpha = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / alpha_denom;
                    float beta = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / beta_denom;
                    float gamma = 1.0f - alpha - beta;

                    if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                        int index = y * screenWidth + x;
                            float interpolatedDepth = alpha * z1 + beta * z2 + gamma * z3;
                            if (interpolatedDepth < zBuffer[index]) {
                                zBuffer[index] = interpolatedDepth;
                                if (index >= 0 && index < frameBuffer.size()) {
                                    float normalizedDepth = interpolatedDepth * 2200.0f;

                                    frameBuffer[index].Char.AsciiChar = charString[static_cast<int>(std::abs(normalizedDepth))];
                                    frameBuffer[index].Attributes = 7;
                            }
                        }
                    }
                }
            }

        }
    }
    renderFrame(); //Render and immediately clear frame after each itteration :)
}



int main() {
    
    std::cout << "Loading Model... \n";
    // Call to "import" and populate our verticies/faces storage
    importModel("bunny.txt", vertices, faces, modelScale);
    // Call to create polygons and populate actualFaces;
    createActualFaces(vertices, faces, actualFaces);

    system("cls"); // Clear the screen
    std::cout << "Model loaded! Press a key to begin the render... \n";
    _getch(); // Wait for key press to start so I can record :)

    // Initialing values for FPS calculation
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    while (true) {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        double fps = static_cast<double>(frameCount) / (duration / 1000.0);
        frameCount = 0;
        startTime = currentTime;
        printValue("FPS: ", fps, 0, 0);
        processPolygon();
    }
 
    return 0;
}

