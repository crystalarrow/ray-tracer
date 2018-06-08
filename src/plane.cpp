#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    if(dot(normal, ray.direction) == 0)
    	return false;

    double t = dot(normal, x1 - ray.endpoint) / dot(normal, ray.direction);

    if(t < 0)
    	return false;

	Hit t1 = {.object = this, .t = t, .ray_exiting = (dot(normal, x1 - ray.endpoint) > 0)};
    hits.push_back(t1);

    return true;
}

vec3 Plane::Normal(const vec3& point) const
{
    return normal;
}
