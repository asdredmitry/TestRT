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

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
    glm::vec3 uv = v/glm::length(v);
    glm::vec3 un = n/glm::length(n);
    float dt =  dot(uv, un);
    float discriminant = (1.0f - ni_over_nt*ni_over_nt*(1 - dt*dt));
    if(discriminant > 0)
    {
        refracted = (uv - un*dt)*ni_over_nt - un*sqrtf(discriminant);
        return true;
    }
    else 
        return false;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx)/(1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class dielectric : public material { 
    public:
        dielectric(float ri) : ref_idx(ri) {}
        virtual bool scatter(const ray& r_in, const hit_rec& rec, glm::vec3& attenuation, ray& scattered) const  {
             glm::vec3 outward_normal;
             glm::vec3 reflected = reflect(r_in.direction, rec.norm);
             float ni_over_nt;
             attenuation = glm::vec3(1.0, 1.0, 1.0); 
             glm::vec3 refracted;
             float reflect_prob;
             float cosine;
             if (glm::dot(r_in.direction, rec.norm) > 0) {
                  outward_normal = -rec.norm;
                  ni_over_nt = ref_idx;
         //         cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
                  cosine = glm::dot(r_in.direction, rec.norm) / glm::length(r_in.direction);
                  cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
             }
             else {
                  outward_normal = rec.norm;
                  ni_over_nt = 1.0 / ref_idx;
                  cosine = -glm::dot(r_in.direction, rec.norm) / glm::length(r_in.direction);
             }
             if (refract(r_in.direction, outward_normal, ni_over_nt, refracted)) 
                reflect_prob = 0.0f;
             else 
                reflect_prob = 1.0;
             if (drand48() < reflect_prob) 
                scattered = ray(rec.p, reflected);
             else 
                scattered = ray(rec.p, refracted);
             return true;
        }

        float ref_idx;
};


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
            glm::vec3 refracted;
            if(glm::dot(in.direction, rec.norm) > 0)
            {
                if(refract(in.direction, -rec.norm, 1./coeff, refracted))
                {
                    attenuation = albego;
                    scattered = ray(rec.p, refracted);
                    return true;
                }
            }
            else
            {
                if(refract(in.direction, rec.norm, coeff, refracted))
                {
                    attenuation = albego;
                    scattered = ray(rec.p, refracted);
                    return true;
                }
            }         
            return false;
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
