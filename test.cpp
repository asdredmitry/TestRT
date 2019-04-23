#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <sstream>
#include <string.h>
std::string toString(const glm::vec3 v)
{
    std::stringstream ss;
    ss << v.x;
    ss << " ";
    ss << v.y;
    ss << " ";
    ss << v.z;
    ss << " ";
    return ss.str();
}
int main()
{
    glm::vec3 v(0.0f, 0.0f, 0.1f);
    glm::vec3 v1(1.0f, 0.0f, 0.1f);
    glm::vec3 v2(v1);
    return 0;
}