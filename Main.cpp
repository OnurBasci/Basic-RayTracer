#include <GLFW/glfw3.h>
#include<iostream>
#include<list>
#include"Image.h"
#include"Vector3.h"
#include"Ray.h"
#include"Object.h"
#include"Camera.h"
#include"Scene.h"
#include"PointLight.h"

using namespace std;

int main(void)
{
    /*
    GLFWwindow* window;

    //Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        //Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    
    */

    //Image image(10,10);
    
    //image.write("imagefile2.ppm");

    //define the scene    X UPWARD Y RIGHT Z FORWARD 
    float numpixelX = 200;//increase these for smoothness
    float numpixelY = 200;
    Camera camera(1, Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), 16, 16, numpixelX, numpixelY);

    PointLight light1(Vector3(3, -4, 1), Vector3(255, 255, 255), 20);
    //PointLight light2(Vector3(-1, 1, 1), Vector3(255, 255, 255), 20);

    list<PointLight> lights;
    lights.push_back(light1);
    //lights.push_back(light2);

    list<Object> objects;

    objects.push_back(Object(Vector3(-3, 2, 7), 3, Vector3(0, 255, 0), MaterialParameters(1,0.5)));
    objects.push_back(Object(Vector3(2, -1, 7), 4, Vector3(255, 0, 0), MaterialParameters(20,0.4)));
    
    Image image(numpixelX, numpixelY);

    Scene scene(camera, objects, lights);

    scene.render(image);
    image.write("render.ppm");
   
    

    return 0;
}

