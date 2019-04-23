#include "ray.hpp"
#include "camera.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include "sphere.hpp"
#include "hitable_list.hpp"
glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - 2*glm::dot(v,n)*n;
}
void write_header(std::ofstream & out, int nx, int ny)
{
    out << "P3" << std::endl;
    out << nx << " " << ny << std::endl;
    out << 255 << std::endl;
}
glm::vec3 color(ray r, hitable_list& world, int depth)
{
    if(depth == 60)
        return glm::vec3(0.f, 1.f, 0.f);
    hit_rec rec;
    if(world.hit(r, 0.01f, std::numeric_limits<float>::max(), rec))
    {
        return color(ray(rec.p, reflect(r.direction, rec.norm)), world, depth + 1)*rec.anten;
    }
    glm::vec3 unit_direction = r.direction/glm::length(r.direction);
    float t1 = 0.5*(unit_direction.y + 1.0);
    return (1.0f - t1)*glm::vec3(1.0f, 1.0f, 1.0f) + glm::vec3(0.5f, 0.7f, 1.0f)*t1;   
}
int main()
{
    std::ofstream out("Picture.ppm");
    write_header(out, 2000, 1000);
    int nx = 2000;
    int ny = 1000;
    int ns = 20;
    //list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.f, new lambertian(vec3(0.1f, 0.9f, 0.1f)));
    //camera cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-2.0f, 1.0f, -1.0f), 4.f, 2.f, 2000, 1000, 1);
    camera cam;
    cam.origin = glm::vec3(0.f, 0.f, 0.f);
    sphere sp(glm::vec3(0.f, 0.f, -1.f), 0.45f, glm::vec3(0.5f, 0.5f, 0.5f));
    sphere sp1(glm::vec3(1.f, 0.f, -1.f), 0.45f, glm::vec3(0.5f, 0.5f, 0.5f));
    sphere sp2(glm::vec3(0.f, -100.5f, -1.0f), 100.f, glm::vec3(0.8f, 0.8f, 0.8f));
    sphere sp3(glm::vec3(-1.f, 0.f, -1.f), 0.45f, glm::vec3(0.8f, 0.8f, 0.9f));
    sphere sp4(glm::vec3(0.f, 2.3f, -2.9f), 0.7f, glm::vec3(0.7f, 0.4f, 0.6f));
    hitable_list world;
    world.list.push_back(&sp);
    world.list.push_back(&sp1);
    world.list.push_back(&sp2);
    world.list.push_back(&sp3);
    world.list.push_back(&sp4);
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
                col += color(r, world, 0);
            }
            col /= float(ns);
            col *= float(255.99);
            out << int(col.x) << " " << int(col.y) << " " << int(col.z) << " ";
        }
        out << std::endl;
    }   
    return 0;
}