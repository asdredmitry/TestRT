#ifndef OBJ_LOADER
#define OBJ_LOADER
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>
#include "matherials.hpp"
#include <sstream>
#include <glm/vec3.hpp>
#include "triangle.hpp"
#include <cstring>
#include <fstream>
#include "rectangle.hpp"
#include <vector>
#include "hitable_list.hpp"

glm::vec3 split(std::string & s, int offset)
{
    //std::cout << "this is s " << s << std::endl;
    std::stringstream ss;
    glm::vec3 tmp;
    ss << s.substr(offset, s.size());
    ss >> tmp.x;
    ss >> tmp.y;
    ss >> tmp.z;
    return tmp;
}
std::string toString1(glm::vec3 v)
{
    std::stringstream ss;
    ss << v.x  << " " << v.y << " " << v.z << " ";
    return ss.str();
}

class loader_obj
{
    //private:
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<triangle> mesh;
        std::string nameOfObjFile;
        std::ifstream input;
    public:
        loader_obj(std::string nameOfFile)
        {
            nameOfObjFile = nameOfFile;
            input.open(nameOfFile.c_str());
        }
        void loadData()
        {
            while(!input.eof())
            {
                std::string tmp;
                std::getline(input, tmp);
                if(tmp.size() >= 2 && (tmp[0] == 'v') && tmp[1] == ' ')
                {
                    vertices.push_back(split(tmp, 2));// + glm::vec3(-0.5f, -0.5f, 0.f));                    
                }
                if(tmp.size() >= 2 && tmp[0] == 'v' && tmp[1] == 'n')
                {
                    normals.push_back(split(tmp, 3));
                }
                if(tmp.size() >= 2 && tmp[0] == 'f')
                {
                    mesh.push_back(splitr(tmp, 1));
                }
            }
        }
        void printVerticies()
        {
            for(int i = 0; i < vertices.size(); i++)
            {
                vertices[i] += glm::vec3(-0.5f, -0.5f, 0.f);
            }
            for(int i = 0; i < vertices.size(); i++)
            {
               std::cout << toString1(vertices[i]) << std::endl;
            }
        }
        void printMesh()
        {
            for(triangle tr : mesh)
            {
                std::cout << toString1(tr.v1) << std::endl;
                std::cout << toString1(tr.v2) << std::endl;
                std::cout << toString1(tr.v3) << std::endl;
                std::cout << toString1(tr.n) << std::endl;
                std::cout << "----------------------------------------------------------" << std::endl;
            }
        }
        triangle splitr(std::string s, int offset)
        {
            std::string tmp;
            std::stringstream ss;
            triangle tr;
            std::vector<int> vn;
            bool tail = true;
            for(int i = s.size()  - 1; i >=0; i--)
            {
                if(s[i] == '/')
                    s[i] = ' ';
                if(s[i] != ' ')
                    tail = false;
                if(s[i] == ' ' && tail)
                    s.erase(i);
            }
            std::cout << s << "  this is i " << std::endl;
            ss << s.substr(offset, s.size());
            int tmp1;
            while(!ss.eof())
            {
                ss >> tmp1;
                vn.push_back(tmp1);
                std::cout << tmp1 << std::endl;
            }
            tr.mat = new metal(glm::vec3(0.5f, 0.5f, 0.5f), 0.01f);//new dielectric(1.5f);//new lambertian(glm::vec3(0.8f + 0.2*drand48(), 0.8f + 0.4*drand48(), 1.f + 0.5*drand48()));
            tr.v1 = vertices[vn[0] - 1];
            tr.v2 = vertices[vn[2] - 1];
            tr.v3 = vertices[vn[4] - 1];
            tr.n = (glm::dot(glm::cross(tr.v1 - tr.v2,tr.v1 - tr.v3), normals[vn[1] - 1]) > 0)? glm::cross(tr.v1 - tr.v2, tr.v1 - tr.v3) : -glm::cross(tr.v1 - tr.v2, tr.v1 - tr.v3);
            return tr; 
        }
        void writeInWorld(hitable_list & world)
        {
            triangle tr1(glm::vec3(-0.5f, -0.5f, 2.f), glm::vec3(0.5f, 0.5f, 2.f), glm::vec3(-0.5f, 0.5f, 2.f), new lambertian(glm::vec3(1.0f, 0.f, 0.f)));
            mesh.push_back(tr1);
            for(int i = 0; i < 12;i++)
            {
                world.list.push_back(&(mesh[i]));
            }
            //world.list.push_back(&mesh[mesh.size() - 1]);
        }    
};

#endif