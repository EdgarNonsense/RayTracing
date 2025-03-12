#define GLM_ENABLE_EXPERIMENTAL
#include "GlossyMaterial.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace glm;

Ray GlossyMaterial::sample_ray_and_update_radiance(Ray& ray, Intersection& intersection) {
    /**
     * Calculate the next ray after intersection with the model.
     * This will be used for recursive ray tracing.
     */

     // Decide if diffuse or specular reflection
    float random = linearRand(0.0f, 1.0f);
    vec3 normal = intersection.normal;
    vec3 point = intersection.point;

    // Diffuse reflection
    if (random > shininess) {
        // Step 1: Sample ray direction
        /**
         * TODO: Task 6.1
         * Implement cosine-weighted hemisphere sampling
         */

         // Cosine-weighted hemisphere sampling
        float s = linearRand(0.0f, 1.0f);
        float t = linearRand(0.0f, 1.0f);
        float u = sqrt(1.0f - s);
        float v = 2.0f * pi<float>() * t;
        vec3 hemisphere_sample = vec3(
            u * cos(v),
            u * sin(v),
            sqrt(s)
        );

        // The direction we sampled above is in local coordinate frame
        // we need to align it with the surface normal
        vec3 new_dir = align_with_normal(hemisphere_sample, normal);

        // Step 2: Calculate radiance
        /**
         * TODO: Task 6.1
         * Note:
         * - C_diffuse = `this->diffuse`
         */
        vec3 W_diffuse = diffuse / pi<float>();

        // update radiance
        ray.W_wip = ray.W_wip * W_diffuse;

        // update ray direction and position
        ray.p0 = point + 0.001f * normal;  // offset point slightly to avoid self intersection
        ray.dir = new_dir;
        ray.is_diffuse_bounce = true;
        ray.n_bounces++;

        return ray;
    }

    // Specular Reflection

    // Step 1: Calculate reflection direction
    /**
     * TODO: Task 6.2
     * Calculate the perfect mirror reflection direction
     */
    vec3 reflection_dir = reflect(ray.dir, normal);

    // Step 2: Calculate radiance
    /**
     * TODO: Task 6.2
     * Note:
     * - C_specular = `this->specular`
     */
    vec3 W_specular = specular;

    // update radiance
    ray.W_wip = ray.W_wip * W_specular;
    ray.p0 = point + 0.001f * normal;  // offset point slightly to avoid self intersection
    ray.dir = reflection_dir;
    ray.is_diffuse_bounce = false;
    ray.n_bounces++;

    return ray;
}


glm::vec3 GlossyMaterial::get_direct_lighting(Intersection& intersection, Scene const& scene) {
    using namespace glm;
    vec3 cumulative_direct_light = vec3(0.0f);

    for (unsigned int idx = 0; idx < scene.light_sources.size(); idx++) {
        // Skip if the intersection is on the light source itself
        if (scene.light_sources[idx] == intersection.model)
            continue;

        vec3 light_pos = scene.light_sources[idx]->get_surface_point();
        vec3 light_dir = normalize(light_pos - intersection.point);

        // Shadow Ray
        Ray shadow_ray;
        shadow_ray.p0 = intersection.point + 0.001f * intersection.normal; // Small offset to avoid self-intersection
        shadow_ray.dir = light_dir;
        shadow_ray.intersections.clear();

        // Cast shadow ray to check visibility
        for (unsigned int i = 0; i < scene.models.size(); i++) {
            scene.models[i]->intersect(shadow_ray);
        }

        // Check visibility by finding closest intersection
        bool light_visible = true;
        float distance_to_light = length(light_pos - intersection.point);

        for (unsigned int i = 0; i < shadow_ray.intersections.size(); i++) {
            // If there's an intersection that's not the light source and it's closer than the light, then light is blocked
            if (shadow_ray.intersections[i].model != scene.light_sources[idx] &&
                shadow_ray.intersections[i].t < distance_to_light) {
                light_visible = false;
                break;
            }
        }

        // Only add light contribution if light is visible
        if (light_visible) {
            vec3 light_emission = scene.light_sources[idx]->material->emission;
            float attenuation_factor = scene.light_sources[idx]->material->get_light_attenuation_factor(distance_to_light);

            // Compute L_ℓ/attenuation_ℓ term
            vec3 attenuated_light = light_emission / attenuation_factor;

            // Compute max(l_ℓ · n, 0) term (cosine of angle between light direction and normal)
            float cos_theta = max(dot(intersection.normal, light_dir), 0.0f);

            // Add this light's contribution to the cumulative direct lighting
            cumulative_direct_light += attenuated_light * cos_theta;
        }
    }

    // Return L_direct which will be multiplied by C_diffuse in color_of_last_bounce()
    return cumulative_direct_light;
}

vec3 GlossyMaterial::color_of_last_bounce(Ray &ray, Intersection &intersection, Scene const &scene) {
    using namespace glm;
    /**
     * Color after last bounce will be `W_wip * last_bounce_color`
     * We shade last bounce for this Glossy material using direct diffuse lighting
     */

    vec3 direct_diff_light = this->get_direct_lighting(intersection, scene);

    return ray.W_wip * diffuse * (1 - shininess) * direct_diff_light;
}