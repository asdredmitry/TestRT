#ifndef RAY_HPP
#define RAY_HPP

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <cmath>
#include <string>
#include <sstream>

class ray
{
    //private:
    public:
        glm::vec3 origin;
        glm::vec3 direction;
    public:
        ray():
            origin(0.f, 0.f, 0.f),
            direction(0.f, 0.f, 0.f)
        {}
        ray(glm::vec3 orig, glm::vec3 direct):
            origin(orig),
            direction(direct)
        {}
        ray(float orx, float ory, float orz, float dx, float dy, float dz):
            origin(orx, ory, orz),
            direction(dx, dy, dz)
        {}
        const glm::vec3& getDirection() const
        {
            return direction;
        }
        glm::vec3& getDirection()
        {
            return direction;
        }
        const glm::vec3& getOrigin() const
        {
            return origin;
        }
        glm::vec3& getOrigin()
        {
            return origin;
        }
        glm::vec3 point_at_parameter(float t) const
        {
            return origin + t*direction;
        }
        void normalize()
        {
            if(direction.length() != 0)
                direction /= glm::length(direction);
        }
        std::string toString()
        {
            std::stringstream ss;
            ss << " Origin - " << origin.x << " " << origin.y << " " << origin.y << "| Direction - " << 
            direction.x << " " << direction.y << " " << direction.z << " ";
            return ss.str();
        }
};

#endif