#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::Shade_Surface(const Ray& ray,const vec3& intersection_point, 
	const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 color;

    // makes sure normal is pointing in right direction
 	vec3 normal = same_side_normal;
 	if(is_exiting) 
		normal = -normal;
    
  	vec3 reflect = ray.direction - 2 * dot(ray.direction, normal) * normal;
	Ray reflected_ray(intersection_point, reflect);

	color = this->reflectivity * world.Cast_Ray(reflected_ray, recursion_depth) + (1 - this->reflectivity) 
    * shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth, is_exiting);

    return color;
}
