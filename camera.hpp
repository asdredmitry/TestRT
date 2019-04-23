#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/geometric.hpp>
#include <cmath>
#include <glm/vec3.hpp>
#include "ray.hpp"

class camera
{
    //private:
    public:
        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal;
        glm::vec3 vertical;
    public:
        camera():
            origin(0.f, 0.f, 0.f), 
            lower_left_corner(-2.f, -1.f, -1.f), 
            horizontal(4.0f, 0.0f, 0.0f), 
            vertical(0.0f, 2.0f, 0.f)
        {}
        camera(glm::vec3 origin1, glm::vec3 lower_left_corner1, glm::vec3 horizontal1, glm::vec3 vertical1):
            origin(origin1),
            lower_left_corner(lower_left_corner1),
            horizontal(horizontal1),
            vertical(vertical1)            
        {}
        ray getRay(float u, float v)
        {
            return ray(origin,  lower_left_corner + horizontal*u + vertical*v - origin);
        }

        
};
#endif