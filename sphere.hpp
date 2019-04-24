#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "hitable.hpp"
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <algorithm>
#include "matherials.hpp"
class sphere : public  hitable
{
    //private:
    public:
        glm::vec3 center;
        float radius;
        material * mat;
    public:
        sphere():
            center(0.f, 0.f, 0.f),
            radius(0.f),
            mat(NULL)
        {}
        sphere(glm::vec3 c, float r, material * mat1):
            center(c),
            radius(r),
            mat(mat1)
        {}
       virtual bool hit(const ray& r, float tmin, float tmax, hit_rec& rec)const;
};
bool sphere::hit(const ray& r, float tmin, float tmax, hit_rec& rec)const 
{
    glm::vec3 oc = r.origin - center;
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(oc, r.direction);
    float c = glm::dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if(discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.norm = (rec.p - center) /radius;
            rec.p_mat = mat;
            return true;
        }
        temp = (-b + sqrt(discriminant))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.norm = (rec.p - center)/radius;
            rec.p_mat = mat;
            return true;
        }
    }
    return false;
}
#endif