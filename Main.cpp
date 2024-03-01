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

    /*Ray r(Vector3(0, 0, 0), Vector3(1, 0, 0));
    Object o(Vector3(5, 1, 0), 1);

    o.intersect(r);*/

    /*
    //define the scene
    float numpixelX = 100;//increase these for smoothness
    float numpixelY = 100;
    Camera camera(1, Vector3(0, 0, 0), Vector3(5, 0, 0), Vector3(0, 1, 0), 4, 4, numpixelX, numpixelY);

    list<Object> objects;

    objects.push_back(Object(Vector3(-4, 3, -7), 3, Vector3(0, 255, 0)));
    objects.push_back(Object(Vector3(0, 0, -7), 5, Vector3(255, 0, 0)));
    
    Image image(numpixelX, numpixelY);

    Scene scene(camera, objects);

    scene.render(image);
    image.write("first render.ppm");
    */

    return 0;
}