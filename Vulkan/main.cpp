#pragma region --- INCLUDES & DEFINES ---
// tell glfw to include vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Graphics.h"
#include "Debug.h"
#include "Benchmark.h"
#include "main.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//const string MODEL_PATH = "models/cat.obj";
//const string TEXTURE_PATH = "textures/cat.jpg";

//const string MODEL_PATH = "models/suzanne.obj";
//const string TEXTURE_PATH = "textures/suzanne.png";

//const string MODEL_PATH = "models/viking_room.obj";
//const string TEXTURE_PATH = "textures/viking_room.png";

int main(int argc, char* argv[]) {
    int runTime;
    string name;
    string MODEL_PATH;
    string TEXTURE_PATH;
    if (argc == 1 ) {
        runTime = 20;
        name = "computersName";
        MODEL_PATH = "models/viking_room.obj";
        TEXTURE_PATH = "textures/viking_room.png";
    }
    else if (argc == 5) {
        name = (string) argv[1];
        runTime = stoi(argv[2]);
        MODEL_PATH = (string)argv[3];
        TEXTURE_PATH = (string)argv[4];
    }
    else
    {
        MessageBox(NULL, "Please specifiy at least 4 arguments \n\nExample: ./vulkan.exe ManfredsPc 20 models\\object.obj textures\\texture.jpg \n\nFirst arg: refference name \nSecond arg: benchmark run time \nThird arg: path of the model \nFourth arg: path of the texture", "Wrong arguments", MB_OK);
        return EXIT_FAILURE;
    }      

    Window window(WIDTH, HEIGHT, name);
    Graphics graphics(window.GetWindow(), MODEL_PATH, TEXTURE_PATH);

    Benchmark benchmark(runTime, name, "vulkan", MODEL_PATH);
    try {

        while (!glfwWindowShouldClose(window.GetWindow()) && benchmark.run()) {
            glfwPollEvents();

            graphics.DrawFrame();
            benchmark.UpdateBenchmark();
        }

        vkDeviceWaitIdle(graphics.GetDevice());
        graphics.CleanUp();
        window.CleanUp();
    }
    catch (const exceptionn& e) {
        printError << e.what() << newLine;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
