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
#include<chrono>

using namespace std;

int main(void)
{

    //Image image(10,10);
    
    //image.write("imagefile2.ppm");

    //define the scene    X Right Y Upward Z FORWARD 
    float numpixelX = 500;//increase these for smoothness
    float numpixelY = 500;
    float deepShadowMapResolution = 128;
    float deepShadowMapSampleNum = 8;
    
    bool render_with_deep_shadow_map = true;
    bool include_self_shadowing = true;

    cout << "Rendering starts:";
    auto start = std::chrono::steady_clock::now();

    SceneSetUp sceneRenderer(numpixelX, numpixelY, deepShadowMapResolution, deepShadowMapSampleNum, render_with_deep_shadow_map, include_self_shadowing);

    sceneRenderer.render3BoxSceneSetUp();
    //sceneRenderer.renderCylinderScene();
    //sceneRenderer.volumetricObjectTestScene();
    //sceneRenderer.box_volumetric_interraction();
    //sceneRenderer.cylinder_box_interaction();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Rendering ends: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}

