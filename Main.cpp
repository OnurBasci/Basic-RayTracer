#include <GLFW/glfw3.h>
#include<iostream>
#include<list>
#include"Image.h"
#include"Vector3.h"
#include"Ray.h"
#include"Sphere.h"
#include"Plane.h"
#include"Camera.h"
#include"Scene.h"
#include"PointLight.h"
#include"Triangle.h"
#include"Rectangle.h"
#include"DeepShadowMap.h"
#include"ShadowCell.h"
#include"Cylinder.h"
#include"SceneSetUp.h"

using namespace std;

int main(void)
{

    //Image image(10,10);
    
    //image.write("imagefile2.ppm");

    //define the scene    X Right Y Upward Z FORWARD 
    float numpixelX = 1500;//increase these for smoothness
    float numpixelY = 1500;
    float deepShadowMapResolution = 32;
    float deepShadowMapSampleNum = 16;
    
    SceneSetUp sceneRenderer(numpixelX, numpixelY, deepShadowMapResolution, deepShadowMapSampleNum, false);

    //sceneRenderer.render3BoxSceneSetUp();
    sceneRenderer.renderCylinderScene();

    return 0;
}

