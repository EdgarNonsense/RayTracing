#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "GeomTriangle.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Intersection.h"
#include "Ray.h"

GeomTriangle::GeomTriangle(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals) {
    this->vertices[0] = vertices[0];
    this->vertices[1] = vertices[1];
    this->vertices[2] = vertices[2];

    this->normals[0] = normals[0];
    this->normals[1] = normals[1];
    this->normals[2] = normals[2];
}

std::vector<Intersection> GeomTriangle::intersect(Ray& ray) {
    using namespace glm;
    /**
     * NOTE: Ray is already transformed to the Model coordinate space.
     */

     // vector to store the intersections
    std::vector<Intersection> intersections;

    vec3 p1 = vertices[0];
    vec3 p2 = vertices[1];
    vec3 p3 = vertices[2];

    vec3 p0 = ray.p0;
    vec3 d = ray.dir;

    // coefficient matrix
    mat4 A(
        vec4(p1, 1.0f),
        vec4(p2, 1.0f),
        vec4(p3, 1.0f),
        vec4(-d, 0.0f)
    );

    // Vector b
    vec4 b(p0, 1.0f);

    // Solving the lambda/t vector then getting their values
    vec4 x = inverse(A) * b;

    float lambda1 = x.x;
    float lambda2 = x.y;
    float lambda3 = x.z;
    float t = x.w;

    const float ep = 1e-5f;
    float sum = lambda1 + lambda2 + lambda3;

    // Check if intersection is valid (inside the triangle and in front of the camera)
    if (t >= 0 && lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0 && abs(sum - 1.0f) <= ep) {
        // intersection point
        vec3 q = lambda1 * p1 + lambda2 * p2 + lambda3 * p3;

        // normal
        vec3 n1 = normals[0];
        vec3 n2 = normals[1];
        vec3 n3 = normals[2];

        vec3 normal = normalize(lambda1 * n1 + lambda2 * n2 + lambda3 * n3);

        intersections.push_back({ t, q, normal, this, nullptr });
    }

    /**
     * TODO: Implement the Ray intersection with the current geometry
     */

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
}