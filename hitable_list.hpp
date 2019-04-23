#ifndef HITABLE_LIST_HPP
#define HITABLE_LIST_HPP
#include "hitable.hpp"
#include <vector>

class hitable_list : public hitable
{
    //private:
    public:
        std::vector<hitable*> list;
    public:
        hitable_list(){}
        hitable_list(std::vector<hitable*> l):
            list(l)
        {}
        virtual bool hit(const ray& r, float t_min, float t_max, hit_rec& rec)const;
};
bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_rec& rec)const
{
    hit_rec temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    for(hitable* i : list)
    {
        if(i->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
#endif