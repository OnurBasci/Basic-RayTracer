#include"Scene.h"
#include<cmath>
#include<list>
using namespace std;

Scene::Scene(Camera camera, list<Object*> objects, list<Light*> ligths){
	this->camera = camera;
	this->objects = objects;
	this->pointLights = ligths;
}

Scene::Scene(list<Object*> objects, list<Light*> ligths)
{
	this->objects = objects;
	this->pointLights = ligths;
}

float getMax(const std::list<float>& values);

void Scene::render(Image& image)
{
	Vector3 intersection_point(0,0,0);
	Vector3 normal(1, 0, 0);
	Vector3 light_dir(0, 0, 0);
	Vector3 light_intensity(0, 0, 0);

	Vector3 pixelValue(0, 0, 0);
	float intensity_sum = 0;
	float Id = 0, Is = 0;

	Vector3 shadow_check_intersect_point(0, 0, 0);
	Vector3 shadow_check_normal(0, 0, 0);

	//to normalize the colors
	list<float> red_values;
	list<float> green_values;
	list<float> blue_values;
	//Calculate rays foreach pixel
	for (int i = 0; i < image.width; i ++)
	{
		for (int j = 0; j < image.height; j ++)
		{
			Ray ray = camera.pixelToRay(i, j);

			//cout << ray.direction << " ";

			//Check intersection for all objects
			for (Object* obj : objects) {

				if (obj->intersect(ray, intersection_point, normal))
				{
					//cout << intersection_point;
					intensity_sum = 0;
					//calculate color for all lights
					for (Light* light : pointLights)
					{
						light->at(intersection_point, light_dir, light_intensity);

						//check the shadows
						bool on_shadow = false;
						Ray shadow_veirfier(light->position, light_dir);
						for (Object* obj2 : objects) {
							if (obj2 != obj) //check if same object
							{
								if (obj2->intersect(shadow_veirfier, shadow_check_intersect_point, shadow_check_normal) \
									&& (shadow_check_intersect_point - light->position).length() < (intersection_point - light->position).length())
								{
									on_shadow = true;
									//std::cout << "On shadow";
								}
							}
						}

						if (on_shadow)
						{
							image.pixels[i][j] = Vector3(0, 0, 0);
							continue;
						}

						//light calculation
						//diffusion
						Id = obj->m_params.kd * (normal*light_dir.normalized()) * light_intensity.length();

						//specular
						Vector3 I = ray.direction.normalized();
						Vector3 S = I - normal * 2 * (I * normal);
						Is = obj->m_params.ks * light_intensity.length() * (S * light_dir.normalized());

						intensity_sum += Id + Is;
					}
					//normalize the intensity
					//if (intensity_sum > 1) intensity_sum = 1;
					//else if (intensity_sum < 0) intensity_sum = 0;
					//update the pixel color
					image.pixels[i][j] = obj->color * intensity_sum;
					red_values.push_back(image.pixels[i][j].x);
					green_values.push_back(image.pixels[i][j].y);
					blue_values.push_back(image.pixels[i][j].z);
					//cout << (light_intensity / 255) << " ";
				}
			}
		}
	}

	//normilize the pixel colors
	float max_red = getMax(red_values);
	float max_green = getMax(green_values);
	float max_blue = getMax(blue_values);
	for (int i = 0; i < image.width; i++)
	{
		for (int j = 0; j < image.height; j++)
		{
			if (image.pixels[i][j].x == image.bg_color.x && image.pixels[i][j].y == image.bg_color.y && image.pixels[i][j].z == image.bg_color.z)
				continue;
			float scaled_red = (max_red > 0) ? (1 - ((max_red - image.pixels[i][j].x) / max_red))*255 : 0;
			float scaled_green = (max_green > 0) ? (1 - ((max_green - image.pixels[i][j].y) / max_green))*255 : 0;
			float scaled_blue = (max_blue > 0) ? (1 - ((max_blue - image.pixels[i][j].z) / max_blue))*255 : 0;
			image.pixels[i][j] = Vector3(scaled_red, scaled_green, scaled_blue);
		}
	}
}

float getMax(const std::list<float>& values) {
	if (values.empty()) {
		return 0;
	}
	else {
		return *std::max_element(values.begin(), values.end());
	}
}