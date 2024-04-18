#pragma once
#include <map>
#include <glm/glm.hpp>


class Vec3Table{
public:
    void Clear();
    void Insert(const glm::vec3& key, const int val);
    int Get(const glm::vec3&key, const float eps)const;
    int GetOrInsert(const glm::vec3& key, const int val, const float eps);

private:
    static float m_eps;
    struct vec3Less{
        bool operator()(const glm::vec3&a,const glm::vec3&b)const{
            if(std::fabs(a.x-b.x)>m_eps)return a.x<b.x;
            if(std::fabs(a.y-b.y)>m_eps)return a.y<b.y;
            return a.z+m_eps<b.z;
        }
    };
    std::map<glm::vec3, int, vec3Less> m_table;
};
