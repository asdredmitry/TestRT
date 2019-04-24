#include "ray.hpp"
#include "camera.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sphere.hpp"
#include "hitable_list.hpp"
#include "matherials.hpp"
void write_header(std::ofstream & out, int nx, int ny)
{
    out << "P3" << std::endl;
    out << nx << " " << ny << std::endl;
    out << 255 << std::endl;
}
glm::vec3 randVec()
{
    return glm::vec3(drand48(), drand48(), drand48());
}
glm::vec3 color(ray r, hitable_list& world, int depth)
{
    hit_rec rec;
    glm::vec3 atten;
    ray scattered;
    if(world.hit(r, 0.001f, std::numeric_limits<float>::max(), rec) )
    {
        if(rec.p_mat->scatter(r, rec, atten, scattered) && depth < 20)
        {
            return atten*color(scattered, world, depth + 1);
        }
        else
        {
            return glm::vec3(0.f, 0.f, 0.f);
        }
    }
    glm::vec3 unit_direction = r.direction/glm::length(r.direction);
    float t1 = 0.5*(unit_direction.y + 1.0);
    return (1.0f - t1)*glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(0.5f, 0.7f, 1.0f)*t1;   
}
int main()
{
    std::ofstream out("Picture.ppm");
    int nx = 2000;
    int ny = 1000;
    int ns = 40;
    write_header(out, nx, ny);
    //list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.f, new lambertian(vec3(0.1f, 0.9f, 0.1f)));
    //camera cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-2.0f, 1.0f, -1.0f), 4.f, 2.f, 2000, 1000, 1);
    camera cam;
    cam.origin = glm::vec3(0.f, 0.0f, 5.f);
    glm::mat3x3 t(cos(M_PI/4), -sin(M_PI/4), 0, sin(M_PI/4), cos(M_PI/4), 0, 0, 0, 1);
    sphere sp(glm::vec3(0.f, 0.f, -1.f), 0.45f, new dielectric(1.5f));//new glass(glm::vec3(1.f, 1.f, 1.f), 1.5)); //new lambertian(glm::vec3(1.f, 0.1f, 0.4f)));
    sphere sp1(glm::vec3(0.f, 0.9f, -3.f), 0.45f, new metal(glm::vec3(0.5f, 0.5f, 0.5f), 0.0f));
    sphere sp2(glm::vec3(0.f, -100.5f, -1.0f), 100.f, new metal(glm::vec3(0.f, 1.f, 0.f), 0.2f));//glm::vec3(0.8f, 0.8f, 0.8f));
    sphere sp3(glm::vec3(0.5f, 0.f, -2.f), 0.30f, new metal(glm::vec3(0.8f, 0.7f, 0.6f), 0.1f));
    //sphere sp(glm::vec3(0.f, 0.f, -1.f), 0.45f, n;// glm::vec3(0.5f, 0.5f, 0.5f));
    //sphere sp1(glm::vec3(1.f, 0.f, -1.f), 0.45f, randVec()); //glm::vec3(0.5f, 0.5f, 0.5f));
    //sphere sp2(glm::vec3(0.f, -100.5f, -1.0f), 100.f, randVec());//glm::vec3(0.8f, 0.8f, 0.8f));
    //sphere sp3(glm::vec3(-1.f, 0.f, -1.f), 0.45f, randVec());//glm::vec3(0.8f, 0.8f, 0.9f));
    //sphere sp4(glm::vec3(0.f, 2.3f, -2.9f), 0.7f, randVec());//glm::vec3(0.7f, 0.4f, 0.6f));
    hitable_list world;
    world.list.push_back(&sp);
    world.list.push_back(&sp1);
    world.list.push_back(&sp2);
    world.list.push_back(&sp3);
    //world.list.push_back(&sp);
    //world.list.push_back(&sp1);
    //world.list.push_back(&sp2);
    //world.list.push_back(&sp3);
    //world.list.push_back(&sp4);
    for(int i = ny - 1; i >= 0; i--)
    {
        for(int j = 0; j < nx; j++)
        {
            glm::vec3 col(0.0f, 0.0f, 0.0f);
            for(int s = 0; s < ns; s++)
            {
                float u = (float(j) + 0.5f + (0.5 - drand48()))/float(nx);
                float v = (float(i) + 0.5f + (0.5 - drand48()))/float(ny);
                ray r = cam.getRay(u, v);
                //std::cout <<" I am here " << std::endl;
                col += color(r, world, 0);
                //std::cout << " And i am here" << std::endl;
            }
            col /= float(ns);
            col *= float(255.99);
            out << int(col.x) << " " << int(col.y) << " " << int(col.z) << " ";
        }
        out << std::endl;
    }   
    return 0;
} 