#ifndef HITABLE_HPP
#define HITABLE_HPP
#include "ray.hpp"

class material;

struct hit_rec
{
    float t;
    glm::vec3 p;
    glm::vec3 norm;
    material * p_mat;
};

class hitable
{
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_rec& rec) const = 0;
};

#endif