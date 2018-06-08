#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
	// I = ambient + diffuse + specular
    vec3 color;
    Hit hit;
    vec3 vectorV = (ray.endpoint - intersection_point).normalized();
 	vec3 normal = same_side_normal;
 	if(is_exiting) // makes sure normal is pointing in right direction
		normal = -normal;
	
	// ambient color
	color = color_ambient * world.ambient_color * world.ambient_intensity;
 	
 	for(unsigned int i = 0; i < world.lights.size(); i++)
 	{
		double distance = (world.lights[i]->position - intersection_point).magnitude_squared();
		vec3 light = (world.lights[i]->position - intersection_point).normalized();

		// checks for shadows
		if (world.enable_shadows && world.Closest_Intersection(Ray(intersection_point, light), hit) 
			&& hit.t * hit.t <= distance)
            continue;

        // diffuse
        double diffuse = dot(light, normal);
        if(diffuse < 0)
            continue;
        color += color_diffuse * diffuse * world.lights[i]->Emitted_Light(ray) / distance;

        // specular
        vec3 reflected = 2 * diffuse * normal - light;
        double specular = dot(reflected, vectorV);
        if (specular < 0)
            continue;
        color += color_specular * pow(specular, specular_power) * world.lights[i]->Emitted_Light(ray) / distance;
 	
    }

    return color;
}
