#include "GeomSphere.h"

#include <iostream>
#include <utility>

#include "Intersection.h"
#include "Ray.h"

std::vector<Intersection> GeomSphere::intersect(Ray& ray) {
    /**
     * NOTE: Ray is already transformed to the Model coordinate space.
     */
    using namespace glm;

    // vector to store the intersections
    std::vector<Intersection> intersections;

    // SPHERE
    vec3 center = this->center;
    float radius = this->radius;

    // RAY
    vec3 origin = ray.p0;
    vec3 direction = normalize(ray.dir);

    vec3 p0_c = origin - center;
    float dir_p0c = dot(direction, p0_c);
    float c_term = dot(p0_c, p0_c) - (radius * radius);
    float discriminant = dir_p0c * dir_p0c - c_term;

    // If inside sqrt is negative then no intersection
    if (discriminant < 0) {
        return intersections;
    }

    float sqrt_discriminant = sqrt(discriminant);
    float t1 = -dir_p0c - sqrt_discriminant;
    float t2 = -dir_p0c + sqrt_discriminant;

    // Add valid intersections
    if (t1 > 0) {
        vec3 point = origin + t1 * direction;
        vec3 normal = normalize(point - center);
        intersections.push_back({ t1, point, normal, this, nullptr });
    }
    if (t2 > 0) {
        vec3 point = origin + t2 * direction;
        vec3 normal = normalize(point - center);
        intersections.push_back({ t2, point, normal, this, nullptr });
    }

    return intersections;
}