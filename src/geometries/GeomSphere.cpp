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

    /**
     * TODO: Implement the Ray intersection with the current geometry
     */
     // SPHERE
    vec3 center = this->center;
    float radius = this->radius;

    // RAY
    vec3 origin = ray.p0;
    vec3 direction = normalize(ray.dir);

    vec3 p0_c = origin - center;
    float dir_p0c = dot(direction, p0_c);
    float dir_p0c_squared = dir_p0c * dir_p0c;
    float c_term = dot(p0_c, p0_c) - (radius * radius);
    float interior = dir_p0c_squared - c_term;

    // If inside sqrt is negative then no intersection
    if (interior < 0) {
        return intersections;
    }

    float sqrt_discriminant = sqrt(interior);
    float t1 = -dir_p0c - sqrt_discriminant;
    float t2 = -dir_p0c + sqrt_discriminant;

    // If interior is 0, add the tangent
    if (interior == 0) {
        if (t1 > 0) {
            vec3 point = origin + t1 * direction;
            vec3 normal = normalize(point - center);
            intersections.push_back({ t1, point, normal, this, nullptr });
        }
        return intersections;
    }

    // If interior is positive then take the smallest positive t
    float smallest_positive_t = (t1 > 0 && (t1 < t2 || t2 <= 0)) ? t1 : t2;

    vec3 point = origin + smallest_positive_t * direction;
    vec3 normal = normalize(point - center);
    intersections.push_back({ smallest_positive_t, point, normal, this, nullptr });

    /**
     * Once you find the intersection, add it to the `intersections` vector.
     *
     * Example:
     * Suppose the ray intersects the current geometry at a point `vec3 point`
     * at a distance `float t`, and the unit normal vector at the intersection
     * point is `vec3 normal`. You would then insert an intersection into the
     * vector as follows:
     *
     * intersections.push_back({t, point, normal, this, nullptr});
     *
     * Note:
     * - Here we pass the Model pointer as `nullptr` because it will be
     *   populated by the Model::intersect() function call.
     * - Only add intersections that are in front of the camera, i.e.,
     *   where t > 0.
     */

     /**
      * TODO: Update `intersections`
      */

    return intersections;
};