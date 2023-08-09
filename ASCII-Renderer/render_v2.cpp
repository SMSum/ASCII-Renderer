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
#include "mathFunc.h"

// Declaring the screen size, really its the buffer size.
const int screenWidth = 300;
const int screenHeight = 180;
// Frame buffer to store the char, position and color.
std::vector<CHAR_INFO> frameBuffer(screenWidth* screenHeight, { ' ', 7 });
// Z-Buffer for each pixel position
std::vector<float> zBuffer(screenWidth* screenHeight, std::numeric_limits<float>::max());

// Funny character array for "shading"
//@#W$9876543210?!abc;:+=-,._
const std::string charString = "_.,-=+:;cba!?0123456789$W#@";
const int totalShades = charString.length();

// Camera location/viewport
Vertex cameraPosition = { 0.0f, 0.0f, 5.0f }; // Example camera position in world space
Vertex targetPosition = { 0.0f, 0.0f, 0.0f };  // Point the camera is looking at
Vertex upVector = { 0.0f, 1.0f, 0.0f };       // Up direction

// Initilize the perspective, and tools for translation/rotation
const int FOV = 10;
const float rotationSpeed = 0.25;
const std::array<std::array<float, 4>, 4> p = perspective(screenWidth, screenHeight, 0.05f, 100, FOV);
std::array<std::array<float, 4>, 4> l = translate(0, 0, 0);
std::array<std::array<float, 4>, 4> r = rotateYXZ(0, 0, 0);


// Create storage for Verticies/Faces
const float modelScale = 2.0f;
std::array<std::array<float, 4>, 4> modelTranslate = translate(0, 0, 50);
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
            Vertex vertex{};
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertex.x *= scale;
            vertex.y *= scale;
            vertex.z *= scale;
            vertices.push_back(vertex);
        }
        else if (type == "f") {  // Face
            Face face{};
            iss >> face.v1 >> face.v2 >> face.v3;
            faces.push_back(face);
        }
    }

    objFile.close();
}
// Function to handle creating our polygons + normals
void createActualFaces(const std::vector<Vertex>& vertices, const std::vector<Face>& faces, std::vector<std::vector<Vertex>>& actualFaces) {
    for (const Face& face : faces) {
        std::vector<Vertex> actualFace;

        // Retrieve vertices using the indices from the face definition
        Vertex v1 = vertices[face.v1 - 1];
        Vertex v2 = vertices[face.v2 - 1];
        Vertex v3 = vertices[face.v3 - 1];

        // Calculate face normal
        Vertex edge1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
        Vertex edge2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
        // Cross product
        Vertex temp{};
        temp.x = edge1.y * edge2.z - edge1.z * edge2.y;
        temp.y = edge1.z * edge2.x - edge1.x * edge2.z;
        temp.z = edge1.x * edge2.y - edge1.y * edge2.x;

        // Normalize the face normal
        float length = std::sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
        Vertex normal = { temp.x / length, temp.y / length, temp.z / length };

        // Store vertices and normalized face normal in actualFace
        actualFace.push_back(v1);
        actualFace.push_back(v2);
        actualFace.push_back(v3);
        actualFace.push_back(normal);

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

std::array<std::array<float, 4>, 4> lookAt(const Vertex& eye, const Vertex& target, const Vertex& up) {
    Vertex forward = normalize(Vertex{ target.x - eye.x, target.y - eye.y, target.z - eye.z });
    Vertex right = normalize(cross(up, forward));
    Vertex newUp = cross(forward, right);

    std::array<std::array<float, 4>, 4> viewMatrix = mat4x4();
    viewMatrix[0] = { right.x, newUp.x, -forward.x, 0.0f };
    viewMatrix[1] = { right.y, newUp.y, -forward.y, 0.0f };
    viewMatrix[2] = { right.z, newUp.z, -forward.z, 0.0f };
    viewMatrix[3] = { -dot(right, eye), -dot(newUp, eye), dot(forward, eye), 1.0f };

    return viewMatrix;
}

//Calculate outside.
std::array<std::array<float, 4>, 4> mainMatrix = mul(p, l);
std::array<std::array<float, 4>, 4> inputMatrix = mul(lookAt(cameraPosition, targetPosition, upVector), mainMatrix);
std::array<std::array<float, 4>, 4> modelMatrix = mul(modelTranslate, r);
void processPolygon() {
    modelMatrix = mul(modelMatrix, rotateYXZ(-rotationSpeed, rotationSpeed, 0));

    for (const auto& actualFace : actualFaces) {
        const Vertex& vertex1 = actualFace[0];
        const Vertex& vertex2 = actualFace[1];
        const Vertex& vertex3 = actualFace[2];
        const Vertex& normal = actualFace[3];

        // Apply object rotation to the vertices of the current face
        Vertex rotatedVertex1 = mul(modelMatrix, vertex1);
        Vertex rotatedVertex2 = mul(modelMatrix, vertex2);
        Vertex rotatedVertex3 = mul(modelMatrix, vertex3);

        // Calculate point vectors and normalized values for specific vertices
        std::array<float, 4> point1_vec4 = { rotatedVertex1.x, rotatedVertex1.y, rotatedVertex1.z, 1.0f };
        std::array<float, 4> point2_vec4 = { rotatedVertex2.x, rotatedVertex2.y, rotatedVertex2.z, 1.0f };
        std::array<float, 4> point3_vec4 = { rotatedVertex3.x, rotatedVertex3.y, rotatedVertex3.z, 1.0f };

        point1_vec4 = mul(inputMatrix, point1_vec4);
        point2_vec4 = mul(inputMatrix, point2_vec4);
        point3_vec4 = mul(inputMatrix, point3_vec4);
         
        // Convert to normalized values
        std::array<float, 4> point1_normalized = divW(point1_vec4);
        std::array<float, 4> point2_normalized = divW(point2_vec4);
        std::array<float, 4> point3_normalized = divW(point3_vec4);

        // Calculate viewport coordinates
        float x1 = (point1_normalized[0] + 1) / 2 * screenWidth;
        float y1 = (1 - point1_normalized[1]) / 2 * screenHeight; // Invert y-coordinate
        float z1 = 1 - point1_normalized[2];

        float x2 = (point2_normalized[0] + 1) / 2 * screenWidth;
        float y2 = (1 - point2_normalized[1]) / 2 * screenHeight; // Invert y-coordinate
        float z2 = 1 - point2_normalized[2];

        float x3 = (point3_normalized[0] + 1) / 2 * screenWidth;
        float y3 = (1 - point3_normalized[1]) / 2 * screenHeight; // Invert y-coordinate
        float z3 = 1 - point3_normalized[2];

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
                    if (index >= 0 && index < frameBuffer.size()) {
                    float interpolatedDepth = alpha * z1 + beta * z2 + gamma * z3;
                    if (interpolatedDepth < zBuffer[index]) {
                        zBuffer[index] = interpolatedDepth;

                            frameBuffer[index].Char.AsciiChar = 'x';
                            frameBuffer[index].Attributes = 7;
                        }
                    }
                }
            }
        }
    }
    renderFrame(); // Render and immediately clear frame after each iteration
}

int main() {
    std::cout << "Loading Model... \n";
    // Call to "import" and populate our verticies/faces storage
    importModel("teapot.obj", vertices, faces, modelScale);
    // Call to create polygons + normals from vertices and faces.
    createActualFaces(vertices, faces, actualFaces);

    system("cls"); // Clear the screen
    std::cout << "Model loaded! Press a key to begin the render... \n";
    _getch(); // Wait for key press to start so I can record :)

    //processPolygon();

    
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

