#include"SceneSetUp.h"
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
#include<chrono>

SceneSetUp::SceneSetUp(float numPx, float numPy, float dsmRes, float dsmSampleNum, bool useDsm, bool self_shadowing)
{
	this->numPixelX = numPx;
	this->numPixelY = numPy;
	this->deepShadowMapRes = dsmRes;
	this->deepShadowMapSample = dsmSampleNum;
    this->useDeepShadowMap = useDsm;
    this->self_shadowing = self_shadowing;
}

SceneSetUp::SceneSetUp()
{
	this->numPixelX = 400;
	this->numPixelY = 400;
	this->deepShadowMapRes = 64;
	this->deepShadowMapSample = 16;
    this->useDeepShadowMap = false;
    this->self_shadowing = false;
}

void SceneSetUp::render3BoxSceneSetUp()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(4, 0, 0), Vector3(4 - 0.3, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);

    PointLight* light1 = new PointLight(Vector3(8, 0, 11), Vector3(255, 255, 255), 20);
    //PointLight light2(Vector3(3, 4, -1), Vector3(255, 255, 255), 20);

    lights.push_back(light1);
    //lights.push_back(light2);

    //Set Objects
    //box 1
    Vector3 box1Center(-2, 0, 11);
    float width = 0.1;
    float height = 4;
    float depth = 4;
    Vector3 color(255, 0, 0);
    //rectangle 1
    Vector3 p1(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p2(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    Vector3 p3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    Rectangle* rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(1, 0.5, 0.1));
    rect1->id = 1;

    //rectangle 2
    Vector3 p21(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p22(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    Vector3 p23(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    Rectangle* rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(1, 0.5, 0.1));
    rect2->id = 1;

    //rectangle 3
    Vector3 p31(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    Vector3 p32(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    Vector3 p33(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    Rectangle* rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(1, 0.5, 0.1));
    rect3->id = 1;

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);

    
    //box 2
    box1Center = Vector3(0.75, 0, 11);
    width = 0.2f;
    height = 2;
    depth = 2;
    color = Vector3(0, 255, 0);
    //rectangle 1
    p1 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p2 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    p3 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(1, 0.5, 0.2));
    rect1->id = 2;

    //rectangle 2
    p21 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p22 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    p23 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(1, 0.5, 0.2));
    rect2->id = 2;

    //rectangle 3
    p31 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    p32 = Vector3(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    p33 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(1, 0.5, 0.2));
    rect3->id = 2;

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);

    //box 3
    box1Center = Vector3(2.25, 0, 11);
    width = 0.2f;
    height = 1;
    depth = 1;
    color = Vector3(0, 0, 255);
    //rectangle 1
    p1 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p2 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    p3 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(1, 0.5, 0.3));
    rect1->id = 3;

    //rectangle 2
    p21 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p22 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    p23 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(1, 0.5, 0.3));
    rect2->id = 3;

    //rectangle 3
    p31 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    p32 = Vector3(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    p33 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(1, 0.5, 0.3));
    rect3->id = 3;

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);

    //box 4
    box1Center = Vector3(3, 0, 11);
    width = 0.2f;
    height = 0.5;
    depth = 0.5;
    color = Vector3(255, 0, 255);
    //rectangle 1
    p1 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p2 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    p3 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(1, 0.5, 0.3));
    rect1->id = 3;

    //rectangle 2
    p21 = Vector3(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    p22 = Vector3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    p23 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(1, 0.5, 0.3));
    rect2->id = 3;

    //rectangle 3
    p31 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    p32 = Vector3(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    p33 = Vector3(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(1, 0.5, 0.3));
    rect3->id = 3;

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);
    
    
    //Render the Scene
    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if(useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(-2, 0, 11), Vector3(0, 1, 0), 1, 1, deepShadowMapRes, deepShadowMapSample);

        scene.renderWithShadowMap(image, deepShadowMap, self_shadowing);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);
    }

    //scene.renderWithShadowMap(image, deepShadowMap);
    image.write("render.ppm");

}

void SceneSetUp::renderCylinderScene()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(0, 0, -2), Vector3(0, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);

    PointLight* light1 = new PointLight(Vector3(3, 0, 0), Vector3(255, 255, 255), 20);
    //PointLight light2(Vector3(3, 4, -1), Vector3(255, 255, 255), 20);

    lights.push_back(light1);
    //lights.push_back(light2);

    //SET THE OBJECTS

    for (float i = 0; i < 30; i++)
    {
        objects.push_back(new Cylinder(Vector3(((static_cast<float>(rand()) / RAND_MAX)-0.5), 0, ((static_cast<float>(rand()) / RAND_MAX) - 0.5) * 2), 0.01, 10, Vector3(255, 255, 0), MaterialParameters(0.5,0.5,1)));
    }

    //Render the scene
    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if (useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(0, 0, 0), Vector3(0, 1, 0), 2, 2, deepShadowMapRes, deepShadowMapSample);
        scene.renderWithShadowMap(image, deepShadowMap, self_shadowing);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);
    }
    //scene.renderWithShadowMap(image, deepShadowMap);
    image.write("render.ppm");
}

void SceneSetUp::volumetricObjectTestScene()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(0, 0, -8), Vector3(0, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);

    PointLight* light1 = new PointLight(Vector3(4, 0, 0), Vector3(255, 255, 255), 20);
    lights.push_back(light1);

    Sphere* volumetricSphere = new Sphere(Vector3(0, 0, 0), 2.5, Vector3(255, 255, 255), MaterialParameters(1, 0.2, 0.2, 0.2, 4, 1, MathHelper::abs_perlin_noise));
    volumetricSphere->is_volumetric_object = true;

    //add floor
    float size = 5;
    Vector3 p1(-1 * size, 1* size, 3);
    Vector3 p2(1*size, 1*size, 3);
    Vector3 p3(-1*size,-1*size, 3);

    Rectangle* floor = new Rectangle(p1, p2, p3, Vector3(255,255,255), MaterialParameters(1, 0.5, 1));

    //objects.push_back(floor);
    objects.push_back(volumetricSphere);

    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if (useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(0, 0, 0), Vector3(0, 1, 0), 2, 2, deepShadowMapRes, deepShadowMapSample);

        scene.renderWithShadowMap(image, deepShadowMap, self_shadowing);
        //cout << "visibility: " << deepShadowMap->shadowCells[(14) * deepShadowMapRes + 28].getVisibility(3.5);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);   
    }

    image.write("render.ppm");
}

void SceneSetUp::box_volumetric_interraction()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(4, 0, 0), Vector3(4 - 0.3, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);

    PointLight* light1 = new PointLight(Vector3(8, 0, 11), Vector3(255, 255, 255), 20);
    //PointLight light2(Vector3(3, 4, -1), Vector3(255, 255, 255), 20);

    lights.push_back(light1);
    //lights.push_back(light2);

    //Set Objects
    //box 1
    Vector3 box1Center(-2, 0, 11);
    float width = 0.1;
    float height = 4;
    float depth = 4;
    Vector3 color(227, 240, 111);
    //rectangle 1
    Vector3 p1(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p2(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    Vector3 p3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    Rectangle* rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(0.5, 0.5, 0.1));
    rect1->id = 1;

    //rectangle 2
    Vector3 p21(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p22(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    Vector3 p23(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    Rectangle* rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(0.5, 0.5, 0.1));
    rect2->id = 1;

    //rectangle 3
    Vector3 p31(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    Vector3 p32(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    Vector3 p33(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    Rectangle* rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(0.5, 0.5, 0.1));
    rect3->id = 1;

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);

    //volumetric object
    Sphere* volumetricSphere = new Sphere(Vector3(2, 0, 11), 1.5, Vector3(255, 255, 255), MaterialParameters(1, 0.2, 0.2, 0.2, 4, 0.5, MathHelper::abs_perlin_noise));
    volumetricSphere->is_volumetric_object = true;

    objects.push_back(volumetricSphere);

    //Render the Scene
    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if (useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(-2, 0, 11), Vector3(0, 1, 0), 1, 1, deepShadowMapRes, deepShadowMapSample);
        scene.renderWithShadowMap(image, deepShadowMap, self_shadowing);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);
    }

    //scene.renderWithShadowMap(image, deepShadowMap);
    image.write("render.ppm");

}

void SceneSetUp::cylinder_box_interaction()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(4, 0, 0), Vector3(4 - 0.3, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);
    
    PointLight* light1 = new PointLight(Vector3(8, 0, 11), Vector3(255, 255, 255), 5);
    //PointLight light2(Vector3(3, 4, -1), Vector3(255, 255, 255), 20);

    lights.push_back(light1);
    //lights.push_back(light2);

    //Set Objects
    //box 1
    Vector3 box1Center(-2, 0, 11);
    float width = 0.1;
    float height = 4;
    float depth = 4;
    Vector3 color(255, 0, 0);
    //rectangle 1
    Vector3 p1(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p2(box1Center.x - width, box1Center.y - height, box1Center.z + depth);
    Vector3 p3(box1Center.x - width, box1Center.y + height, box1Center.z - depth);

    Rectangle* rect1 = new Rectangle(p1, p2, p3, color, MaterialParameters(1, 0.5, 0.1));

    rect1->id = 1;

    //rectangle 2
    Vector3 p21(box1Center.x - width, box1Center.y - height, box1Center.z - depth);
    Vector3 p22(box1Center.x - width, box1Center.y + height, box1Center.z - depth);
    Vector3 p23(box1Center.x + width, box1Center.y - height, box1Center.z - depth);

    Rectangle* rect2 = new Rectangle(p21, p22, p23, color, MaterialParameters(1, 0.5, 0.1));
    rect2->id = 2;

    //rectangle 3
    Vector3 p31(box1Center.x + width, box1Center.y - height, box1Center.z - depth);
    Vector3 p32(box1Center.x + width, box1Center.y + height, box1Center.z - depth);
    Vector3 p33(box1Center.x + width, box1Center.y - height, box1Center.z + depth);

    Rectangle* rect3 = new Rectangle(p31, p32, p33, color, MaterialParameters(0.5, 0.5, 0.1));
    rect3->id = 3;
    

    objects.push_back(rect1);
    objects.push_back(rect3);
    objects.push_back(rect2);

    //add cylinders
    
    for (float i = 0; i < 60; i++)
    {
        objects.push_back(new Cylinder(Vector3(((static_cast<float>(rand()) / RAND_MAX) - 0.5) + 2, 0, ((static_cast<float>(rand()) / RAND_MAX) - 0.5) * 2 + 11), 0.01, 1, Vector3(255, 255, 0), MaterialParameters(0.5,0.5,1)));
    }

    //Render the Scene
    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if (useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(-2, 0, 11), Vector3(0, 1, 0), 1, 1, deepShadowMapRes, deepShadowMapSample);
        scene.renderWithShadowMap(image, deepShadowMap, self_shadowing);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);
    }

    //scene.renderWithShadowMap(image, deepShadowMap);
    image.write("render.ppm");
}


SceneSetUp::~SceneSetUp()
{
    //Dealocate the memory
    for (Object* obj : objects)
    {
        delete obj;
    }
    for (Light* l : lights)
    {
        delete l;
    }
}