#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <functional>
class VoxelTable{
public:
    void Add(int x, int y, const glm::ivec3& val);
    void ForKey(const float key, std::function<void(int,int,int)> func)const;

private:
    std::vector<std::pair<int, std::vector<std::pair<int, std::vector<glm::ivec3>>>>> m_data;
};
