#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio = -1;
    vec3 vectorV = (ray.endpoint - intersection_point).normalized();

    // makes sure normal is pointing in right direction
    vec3 normal = same_side_normal;
    if(is_exiting) 
        normal = -normal;

    if(!world.disable_fresnel_refraction) // calculating refracting
    {   
        double nI, nR;
        if(is_exiting) // ray is exiting the surface 
        {
            nI = refractive_index;
            nR = REFRACTIVE_INDICES::AIR;
        }
        else // ray entering a surface
        {
            nI = REFRACTIVE_INDICES::AIR;
            nR = refractive_index;
        }

        double n = nI / nR;
        double cosI = dot(vectorV, normal);
        double cosR = 1 - n * n * (1 - cosI * cosI);
        if(cosR < 0)
            reflectance_ratio = 1;
        else
        {
            cosR = sqrt(cosR);
            vec3 vectorT = n * (dot(vectorV, normal) * normal - vectorV) - cosR * normal;
            refraction_color = world.Cast_Ray(Ray(intersection_point, vectorT), recursion_depth);

            double rOrth = ((nI * cosI - nR * cosR) / (nR * cosR + nI * cosI));
            double rPara = ((nR * cosI - nI * cosR) / (nR * cosI + nI * cosR));
            reflectance_ratio = (rOrth * rOrth + rPara * rPara) / 2.0;
        }

    }

    if(!world.disable_fresnel_reflection) // calculates reflection
    {
       vec3 reflected = 2 * dot(vectorV, normal) * normal - vectorV;
       reflection_color = world.Cast_Ray(Ray(intersection_point, reflected), recursion_depth);
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;

    
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;

    return color;
}

void Refractive_Shader::Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

