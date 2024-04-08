#include"Scene.h"
#include<cmath>
#include<list>
using namespace std;

Scene::Scene(Camera camera, list<Object*> objects, list<Light*> ligths){
	this->camera = camera;
	this->objects = objects;
	this->pointLights = ligths;
}

/*Scene::Scene(Camera camera, list<Object*> objects, list<Light*> ligths, DeepShadowMap shadowMap) {
	Scene(camera, objects, ligths);
	this->shadowMap = shadowMap;
}*/

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
	float t0, t1; //intersection values

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

				//Solid objec intersection
				if (!obj->is_volumetric_object && obj->intersect(ray, intersection_point, normal))
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

					//update the pixel color
					image.pixels[i][j] = obj->color * intensity_sum;
					
					cout << "image color: " << image.pixels[i][j];
				}

				//Volume Object intersection
				if (obj->is_volumetric_object && obj->volumeIntersect(ray, t0, t1))
				{
					//volume absorption calculation
					float distance = t1 - t0;
					float transmission = exp(-distance * obj->m_params.sigma_a);
					
					image.pixels[i][j] = image.bg_color * transmission + obj->color * (1 - transmission);
					//cout << " t: " << image.bg_color * transmission + obj->color * (1 - transmission);
				}

				//for normalization
				red_values.push_back(image.pixels[i][j].x);
				green_values.push_back(image.pixels[i][j].y);
				blue_values.push_back(image.pixels[i][j].z);
			}
		}
	}

	//normilize the pixel colors
	/*float max_red = getMax(red_values);
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
	*/
}


void Scene::renderWithShadowMap(Image& image, DeepShadowMap* shadowMap)
{
	//This function renders shadows with a deep shadow map
	Vector3 intersection_point(0, 0, 0);
	Vector3 normal(1, 0, 0);
	Vector3 light_dir(0, 0, 0);
	Vector3 light_intensity(0, 0, 0);
	float t0, t1; //intersection values

	Vector3 pixelValue(0, 0, 0);
	float intensity_sum = 0;
	float Id = 0, Is = 0;
	float visibility;

	Vector3 shadow_check_intersect_point(0, 0, 0);
	Vector3 shadow_check_normal(0, 0, 0);

	//to normalize the colors
	list<float> red_values;
	list<float> green_values;
	list<float> blue_values;
	//Calculate rays foreach pixel
	for (int i = 0; i < image.width; i++)
	{
		for (int j = 0; j < image.height; j++)
		{
			//cout << "(" << i << ", " << j << ") \n";
			Ray ray = camera.pixelToRay(i, j);

			//cout << ray.direction << " ";

			//Check intersection for all objects
			for (Object* obj : objects) {

				if (obj->intersect(ray, intersection_point, normal))
				{
					//Solid objec intersection
					if (!obj->is_volumetric_object && obj->intersect(ray, intersection_point, normal))
					{
						intensity_sum = 0;
						//calculate color for all lights
						for (Light* light : pointLights)
						{
							light->at(intersection_point, light_dir, light_intensity);

							//Shadow calculations
							visibility = shadowMap->getAveragesVisibilityFromWorldPos(intersection_point);
							//visibility = shadowMap->getVisibilityFromWorldPos(intersection_point);

							//light calculation
							//diffusion
							Id = obj->m_params.kd * (normal * light_dir.normalized()) * light_intensity.length();

							//specular
							Vector3 I = ray.direction.normalized();
							Vector3 S = I - normal * 2 * (I * normal);
							Is = obj->m_params.ks * light_intensity.length() * (S * light_dir.normalized());

							intensity_sum += Id + Is;
						}
						image.pixels[i][j] = obj->color * intensity_sum * visibility;
					}
					//Volume Object intersection
					if (obj->is_volumetric_object && obj->volumeIntersect(ray, t0, t1))
					{
						
						//Ray marching to calculate absorption
						//get the correct step size 
						float step_size = 0.1f;
						int num_sample = std::ceil((t1 - t0) / step_size);
						step_size = (t1 - t0) / num_sample;

						float transmission = 1;
						Vector3 result(0, 0, 0);
						Vector3 lightColor(255, 255, 255);

						
						float sigma_t = obj->m_params.sigma_a + obj->m_params.sigma_s; // extinction coefficient
						for (int i = 0; i < num_sample; i++)
						{
							float t = t0 + step_size * (i + 0.5);
							Vector3 sample = ray.center + ray.direction * t;
							
							transmission *= exp(-step_size * obj->m_params.density(sample) * sigma_t);
							
							
							// in scattering
							Ray to_light_Ray(sample, shadowMap->position);
							
							float t0_light, t1_light;
							if (obj->volumeIntersect(to_light_Ray, t0_light, t1_light)) {

								size_t num_steps_light = std::ceil(t1_light / step_size);
								float stide_light = t1_light / num_steps_light;

								float tau = 0;
								// Ray-march along the light ray. Store the density values in the tau variable.
								for (size_t nl = 0; nl < num_steps_light; ++nl) {
									float t_light = stide_light * (nl + 0.5);
									Vector3 light_sample_pos = sample + light_dir * t_light;
									tau += obj->m_params.density(light_sample_pos);
								}
								float light_ray_att = exp(-tau * stide_light * sigma_t);

								result = result + lightColor * light_ray_att * obj->m_params.sigma_s * transmission * step_size * obj->m_params.density(sample);
							}
							
						}

						//visibility = shadowMap->getAveragesVisibilityFromWorldPos(intersection_point);

						//image.pixels[i][j] = (image.bg_color * transmission + obj->color * (1 - transmission));
						image.pixels[i][j] = (image.bg_color * transmission + result);
						//cout << "color: " << image.pixels[i][j];
					}
					//update the pixel color
					red_values.push_back(image.pixels[i][j].x);
					green_values.push_back(image.pixels[i][j].y);
					blue_values.push_back(image.pixels[i][j].z);
					//cout << (light_intensity / 255) << " ";
				}
			}
		}
	}
	
	//normilize the pixel colors
	/*
	float max_red = getMax(red_values);
	float max_green = getMax(green_values);
	float max_blue = getMax(blue_values);
	for (int i = 0; i < image.width; i++)
	{
		for (int j = 0; j < image.height; j++)
		{
			if (image.pixels[i][j].x == image.bg_color.x && image.pixels[i][j].y == image.bg_color.y && image.pixels[i][j].z == image.bg_color.z)
				continue;
			float scaled_red = (max_red > 0) ? (1 - ((max_red - image.pixels[i][j].x) / max_red)) * 255 : 0;
			float scaled_green = (max_green > 0) ? (1 - ((max_green - image.pixels[i][j].y) / max_green)) * 255 : 0;
			float scaled_blue = (max_blue > 0) ? (1 - ((max_blue - image.pixels[i][j].z) / max_blue)) * 255 : 0;
			image.pixels[i][j] = Vector3(scaled_red, scaled_green, scaled_blue);
		}
	}
	*/
}


float getMax(const std::list<float>& values) {
	if (values.empty()) {
		return 0;
	}
	else {
		return *std::max_element(values.begin(), values.end());
	}
}