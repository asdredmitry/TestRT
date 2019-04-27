#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "hitable.hpp"
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <algorithm>
#include "matherials.hpp"
#include <glm/glm.hpp>

//glm::mat3x3 identity()
//{
    //return glm::mat3x3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
//}

bool inside_angle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p)
{
    glm::vec3 v12 = glm::cross(v2 - v1, p - v1);
    glm::vec3 v13 = glm::cross(v3 - v1, p - v1);
    return glm::dot(v12, v13) < 0.f;
}

class triangle : public hitable
{
    //private:
    public:
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 v3;
        glm::vec3 n;
        material * mat;
    public:
        triangle()
        {
            v1=glm::vec3(0.f, 0.f, 0.f);
            v2=glm::vec3(0.f, 0.f, 0.f);
            v3=glm::vec3(0.f, 0.f, 0.f);
            n=glm::vec3(1.f, 0.f, 0.f);
            mat = NULL;
        }
        triangle(glm::vec3 v11, glm::vec3 v21, glm::vec3 v31, material * mat1):
            v1(v11),
            v2(v21),
            v3(v31)
        {
            n = glm::cross(v21 - v11, v31 - v11);
            n /= glm::length(n);
            mat = mat1;
        }
        triangle(glm::vec3 v11, glm::vec3 v21, glm::vec3 v31, glm::vec3 n1, material * mat1):
            v1(v11), 
            v2(v21),
            v3(v31),
            n(n1),
            mat(mat1)
        {}
        virtual bool hit(const ray& r, float tmin, float tmax, hit_rec& rec)const;
        void compute(glm::mat3x3 modelView)
        {
            v1 = modelView*v1;
            v2 = modelView*v2;
            v3 = modelView*v3;
            n = modelView*n;
        }
        
};

bool triangle::hit(const ray& r, float tmin, float tmax, hit_rec& rec) const
{
    glm::vec3 oc = r.origin - v1;
    glm::mat3x3 m(v2 - v1, v3 - v1, -r.direction);
    glm::vec3 t = glm::inverse(m)*oc;
    glm::vec3 point = r.origin + t.z*r.direction;
    //std::cout << point.x << " " << point.y << " " << point.z << std::endl;
    if(t.z > tmin && t.z < tmax)
    {
       if(inside_angle(v1, v2, v3, point) && inside_angle(v2, v3, v1, point))//glm::dot(point - v1, v3 - v1) > 0 && glm::dot(point  - v3, v3 - v1) > 0 && glm::dot(point - v2, v3 - v2) > 0 && glm::dot(point - v3, v2 - v3) > 0 && glm::dot(point - v2, v1 - v2) > 0 && glm::dot(point - v1, v2 - v1) > 0)
       {
           //std::cout << "I am here " << std::endl;
           rec.t = t.z;
           rec.p = point;
           rec.norm = n;
           rec.p_mat = mat;
           return  true;
       }
    }
    return false;
}

#endif