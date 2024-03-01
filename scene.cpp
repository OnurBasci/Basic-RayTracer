#include"Scene.h"

using namespace std;

Scene::Scene(Camera camera, list<Object> objects){
	this->camera = camera;
	this->objects = objects;
}

Scene::Scene(list<Object> objects)
{
	this->objects = objects;
}


void Scene::render(Image& image)
{
	//Calculate rays foreach pixel
	for (int i = 0; i < image.width; i ++)
	{
		for (int j = 0; j < image.height; j ++)
		{
			Ray ray = camera.pixelToRay(i, j);

			//cout << ray.direction << " ";

			//Check intersection for all objects
			for (Object& obj : objects) {
				if (obj.intersect(ray))
				{
					image.pixels[i][j] = obj.color;

					//cout << image.pixels[i][j] << " ";
				}
				/*else
				{
					image.pixels[i][j] = Vector3(0, 0, 0);
				}*/
			}
		}
	}
}