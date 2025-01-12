#pragma once
#include <vector>

template<typename T>
class Vector3{
public:
    int sizex()const{return m_x;}
    int sizey()const{return m_y;}
    int sizez()const{return m_z;}
    inline T& operator[](const int ind){
        return m_data[ind];
    }
    inline T& Get(const int x,const int y,const int z){
        return m_data[x*m_y*m_z+y*m_z+z];
    }
    inline const T& Get(const int x,const int y,const int z)const{
        return m_data[x*m_y*m_z+y*m_z+z];
    }
    inline void resize(const int x,const int y,const int z){
        m_x=x;m_y=y;m_z=z;
        m_data.resize(x*y*z);
    }
    inline void resize(const int x,const int y,const int z, const T& w){
        m_x=x;m_y=y;m_z=z;
        m_data.resize(x*y*z, w);
    }
    inline void assign(const T& v){
        for(auto&it:m_data){
            it = v;
        }
    }
    inline T* data(){
        return m_data.data();
    }
private:
    int m_x,m_y,m_z;
    std::vector<T> m_data;
};

