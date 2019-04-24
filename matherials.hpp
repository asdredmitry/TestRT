#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "ray.hpp"
#include "hitable.hpp"
#include "glm/mat3x3.hpp"
#include <iostream>

class material
{
    public:
        virtual bool scatter(const ray& in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};

glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do
    {
        p = glm::vec3(drand48(), drand48(), drand48())*2.0f - glm::vec3(1.0f, 1.0f, 1.0f);
    } while (glm::length(p)*glm::length(p) >= 1.0f);
    return p;    
}
glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - 2*glm::dot(v,n)*n;
}

glm::vec3 refract(const glm::vec3& in, const glm::vec3& n, float ratio)
{
    float dotinn = glm::dot(in, n);
    glm::vec3 refracted = (in + dotinn*n)*ratio;
    refracted -= dotinn*n;
    return refracted;
}

class glass: public material
{
    //private:
    public:
        glm::vec3 albego;
        float coeff;
    public:
        glass(glm::vec3 a, float c):
            albego(a),
            coeff(c)
        {}
        virtual bool scatter(const ray& in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered)const
        {
            glm::vec3 tmp;
            if(glm::dot(in.direction, rec.norm) > 0)
            {
                tmp = refract(in.direction, -rec.norm, 1.f/coeff);
                attenuation = albego;
                scattered = ray(rec.p, tmp);
                return true;
            }
            else
            {
                tmp = refract(in.direction, rec.norm, coeff);
                attenuation = albego;
                scattered = ray(rec.p, tmp);
                return true;
            }         
            return true;
        }
};

class const_change: public material
{
    //private;
    public:
        glm::mat3x3 m;
        glm::vec3 albego;
    public:
        const_change(glm::mat3x3 m1, glm::vec3 v):
            m(m1),
            albego(v)
        {}
        virtual bool scatter(const ray& in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered)const
        {
            glm::vec3 tmp = in.direction*m;
            scattered = ray(rec.p, tmp);
            attenuation = albego;
            return true;
        }

};

class lambertian : public material
{
    //private:
    public:
        glm::vec3 albego;
    public:
        lambertian(const glm::vec3 a): albego(a){}
        virtual bool scatter(const ray& in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered) const
        {
            glm::vec3 target = rec.p + rec.norm + random_in_unit_sphere();
            scattered = ray(rec.p, target - rec.p);
            attenuation = albego;
            return true;
        }
};
class metal : public material
{
    //private:
    public:
        glm::vec3 albego;
        float fuzz;
    public:
        metal(glm::vec3 a, float f):
            albego(a),
            fuzz(f)
        {}
        virtual bool scatter(const ray& in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered) const
        {
            //std::cout << in.toString() << std::endl;
            glm::vec3 reflected = reflect(in.direction/glm::length(in.direction), rec.norm);
            //std::cout << reflected.x << " " << reflected.y << " " << reflected.z << " std::endl;" << std::endl;
            scattered = ray(rec.p, reflected + random_in_unit_sphere()*fuzz);
            attenuation = albego;
            //std::cout << glm::dot(scattered.direction, rec.norm) << std::endl;
            return (glm::dot(scattered.direction, rec.norm) > 0);
        }
};
#endif
