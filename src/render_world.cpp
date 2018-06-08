#include <vector>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
    int min_t = -1;
    std::vector<Hit> hits;

    for(unsigned int i = 0; i < this->objects.size(); i++) // finds the hit
        objects[i]->Intersection(ray, hits);

    if(hits.size() == 0) // no hits
        return 0;
    else
    {
        Object* closest_object; 
        for(unsigned int i = 0; i < hits.size(); i++)
        {
            if(hits[i].t > small_t && (min_t == -1 || hits[min_t].t > hits[i].t))
                min_t = i;
        }
        if (hits[min_t].t <= small_t)
            return 0;
        if (min_t == -1)
            return 0;

        hit = hits[min_t];
        closest_object =  (Object*)hit.object;
        return closest_object;
    }

    return 0;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray(camera.World_Position(pixel_index), 
           (camera.World_Position(pixel_index) - camera.position).normalized()); 
    vec3 color = Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO
    vec3 color;

    Shader * shading = background_shader;

    Hit intersected;
    vec3 point;
    vec3 normal;

    if(Closest_Intersection(ray, intersected) && recursion_depth <= recursion_depth_limit) 
    {
        point = ray.Point(intersected.t);
        normal = intersected.object->Normal(point);
        shading = intersected.object->material_shader;
    }

    color = shading->Shade_Surface(ray, point, normal, recursion_depth+1, intersected.ray_exiting);

    return color;
}
