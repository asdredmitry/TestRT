#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "hitable.hpp"
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <algorithm>
#include "matherials.hpp"
#include <glm/glm.hpp>
#include "triangle.hpp"

class rectangle : public hitable
{
    //private:
    public:
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 v3;
        glm::vec3 v4;
        glm::vec3 norm;
        material * mat;
    public:
        rectangle():
            v1(glm::vec3(1.f, 0.f, 0.f)),
            v2(glm::vec3(0.f, 0.f, 0.f)),
            v3(glm::vec3(0.f, 1.f, 0.f)),
            v4(glm::vec3(1.f, 1.f, 0.f)),
            norm(0.f, 0.f, 1.f),
            mat(NULL)
        {}
        rectangle(glm::vec3 v1i, glm::vec3 v2i, glm::vec3 v3i, glm::vec3 v4i, glm::vec3 normi, material * mati):
            v1(v1i),
            v2(v2i),
            v3(v3i),
            v4(v4i),
            norm(normi),
            mat(mati)
        {}
        virtual bool hit(const ray& r, float tmin, float tmax, hit_rec& rec)const;
        void compute(glm::mat3x3 rot)
        {
            v1 = rot*v1;
            v2 = rot*v2;
            v3 = rot*v3;
            v4 = rot*v4;
            norm = rot*norm;
        }
};
bool rectangle::hit(const ray& r, float tmin, float tmax, hit_rec& rec) const
{
    glm::vec3 oc = r.origin - v1;
    glm::mat3x3 m(v2 - v1, v3 - v1, -r.direction);
    glm::vec3 t = glm::inverse(m)*oc;
    glm::vec3 point = r.origin + t.z*r.direction;
    if(t.z > tmin && t.z < tmax)
    {
       if(inside_angle(v1, v2, v4, point) && inside_angle(v3, v2, v4, point) && inside_angle(v2, v1, v3, point))//glm::dot(point - v1, v3 - v1) > 0 && glm::dot(point  - v3, v3 - v1) > 0 && glm::dot(point - v2, v3 - v2) > 0 && glm::dot(point - v3, v2 - v3) > 0 && glm::dot(point - v2, v1 - v2) > 0 && glm::dot(point - v1, v2 - v1) > 0)
       {
           //std::cout << "I am here " << std::endl;
           rec.p = point;
           rec.norm = norm;
           rec.p_mat = mat;
           return  true;
       }
    }
    return false;
}
#endif