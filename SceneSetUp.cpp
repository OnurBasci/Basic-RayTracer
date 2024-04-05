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

SceneSetUp::SceneSetUp(float numPx, float numPy, float dsmRes, float dsmSampleNum, bool useDsm)
{
	this->numPixelX = numPx;
	this->numPixelY = numPy;
	this->deepShadowMapRes = dsmRes;
	this->deepShadowMapSample = dsmSampleNum;
    this->useDeepShadowMap = useDsm;
}

SceneSetUp::SceneSetUp()
{
	this->numPixelX = 400;
	this->numPixelY = 400;
	this->deepShadowMapRes = 64;
	this->deepShadowMapSample = 16;
    this->useDeepShadowMap = false;
}

void SceneSetUp::render3BoxSceneSetUp()
{
    //Set Camera and Lightning
    Camera camera(1, Vector3(4, 0, 0), Vector3(4 - 0.3, 0, 1), Vector3(-1, 0, 0), 1, 1, numPixelX, numPixelY);

    PointLight* light1 = new PointLight(Vector3(8, 0, 11), Vector3(255, 255, 255), 20);
    //PointLight light2(Vector3(3, 4, -1), Vector3(255, 255, 255), 20);

    list<Light*> lights;
    lights.push_back(light1);
    //lights.push_back(light2);

    list<Object*> objects;

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
    box1Center = Vector3(1, 0, 11);
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
    box1Center = Vector3(3, 0, 11);
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


    //Render the Scene
    Image image(numPixelX, numPixelY);

    Scene scene(camera, objects, lights);

    if(useDeepShadowMap)
    {
        DeepShadowMap* deepShadowMap = new DeepShadowMap(objects, 1, light1->position, Vector3(-2, 0, 11), Vector3(0, 1, 0), 1, 1, deepShadowMapRes, deepShadowMapSample);
        scene.renderWithShadowMap(image, deepShadowMap);
        delete deepShadowMap;
    }
    else
    {
        scene.render(image);
    }

    //scene.renderWithShadowMap(image, deepShadowMap);
    image.write("render.ppm");


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