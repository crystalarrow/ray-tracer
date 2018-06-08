#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{

    /*
     From quadratic formula
     at^2 + bt + c = 0
     a = dot(u, u)
     b = 2*dot(u, e-c)
     c = dot(e-c, e-c) - r^2
     */
    double a = dot(ray.direction, ray.direction);
	double b = 2 * dot(ray.direction, ray.endpoint - this->center);
	double c = dot(ray.endpoint - this->center, ray.endpoint - this->center) - this->radius * this->radius;

	
	// discriminant = b^2 - 4ac
	
	double discriminant = b*b - 4*a*c;

	if(discriminant <= 0) // no intersections
		return false;

	// intersections
	Hit t1 = {.object = this, .t = (-b+sqrt(discriminant)) / (2*a), .ray_exiting = true};
	Hit t2 = {.object = this, .t = (-b-sqrt(discriminant)) / (2*a), .ray_exiting = false};

	// t2 always larger than t1
	if (t1.t < 0)
    	return false;
 	hits.push_back(t1);
	if (t2.t < 0)
		return true;
	hits.push_back(t2);

	return true;
}

vec3 Sphere::Normal(const vec3& point) const
{

    return (point - center).normalized();
}
