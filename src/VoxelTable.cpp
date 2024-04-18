#include "VoxelTable.h"
#include <iostream>

void VoxelTable::Add(int x, int y, const glm::ivec3& val){
    auto xit = m_data.begin();
    for(;xit!=m_data.end();xit++){
        if(xit->first>=x)break;
    }
    if(xit==m_data.end()||xit->first!=x){
        std::pair<int,std::vector<std::pair<int,std::vector<glm::ivec3>>>> item{x,{{y, {val}}}};
        m_data.insert(xit, item);
        return;
    }
    auto& arry = xit->second;
    auto yit = arry.begin();
    for(;yit!=arry.end();yit++){
        if(yit->first<=y)break;
    }
    if(yit==arry.end()||yit->first!=y){
        std::pair<int,std::vector<glm::ivec3>> item{y, {val}};
        arry.insert(yit, item);
        return;
    }
    yit->second.push_back(val);
}

void VoxelTable::ForKey(const float key, std::function<void(int,int,int)> func)const{
    auto itx = m_data.begin();
    for(;itx!=m_data.end()&&itx->first<=key;itx++){
        auto arry = itx->second;
        auto ity = arry.begin();
        for(;ity!=arry.end()&&ity->first>=key;ity++){
            for(glm::ivec3& x:ity->second){
                func(x.x, x.y, x.z);
            }
        }
    }
}
