#pragma once
#include <vector>

template<typename T>
class Vector3{
public:
    int sizex()const{return m_x;}
    int sizey()const{return m_y;}
    int sizez()const{return m_z;}
    inline T& operator[](int ind){
        return m_data[ind];
    }
    inline T& Get(int x,int y,int z){
        return m_data[x*m_y*m_z+y*m_z+z];
    }
    inline const T& Get(int x,int y,int z)const{
        return m_data[x*m_y*m_z+y*m_z+z];
    }
    inline void resize(int x,int y,int z){
        m_x=x;m_y=y;m_z=z;
        m_data.resize(x*y*z);
    }
    inline void resize(int x,int y,int z, T w){
        m_x=x;m_y=y;m_z=z;
        m_data.resize(x*y*z, w);
    }
    inline void assign(const T v){
        for(auto&it:m_data){
            it = v;
        }
    }
private:
    int m_x,m_y,m_z;
    std::vector<T> m_data;
};

