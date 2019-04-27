#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include "obj_loader.hpp"
#include <string.h>
std::string toString(const glm::vec3& v)
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
std::string toString(const glm::mat3x3& m)
{
}
int main()
{  
    /*std::string s("add 0.1000   7.0 \n");
    std::stringstream ss;
    ss << s.substr(3, s.size());
    float tmp;
    while(!ss.eof())
    {
        ss >> tmp;
        std::cout << tmp << std::endl;
    }
    */
    //float tmp;
    //float tmp1;
    //ss >> tmp;
    //ss >> tmp1;
    //std::cout << tmp << std::endl;
    //std::cout << tmp1 << std::endl;
    loader_obj loader("cube.obj");
    loader.loadData();
    loader.printVerticies();
    loader.printMesh();
    
    
}