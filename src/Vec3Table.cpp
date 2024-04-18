#include "Vec3Table.h"
float Vec3Table::m_eps;

void Vec3Table::Clear(){
    m_table.clear();
}

void Vec3Table::Insert(const glm::vec3& key, const int val){
    m_table[key] = val;
}

int Vec3Table::Get(const glm::vec3&key, const float eps)const{
    Vec3Table::m_eps = eps;
    auto it = m_table.find(key);
    if(it==m_table.end())return -1;
    return it->second;
}

int Vec3Table::GetOrInsert(const glm::vec3& key, const int val, const float eps){
    int v = Get(key, eps);
    if(v!=-1) return v;
    Insert(key, val);
    return val;
}
