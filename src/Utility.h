#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <cmath>
#define ERR_CHECK_BOOL(x) do{if(!(x)){std::cerr<<"Error occurs at file "<<__FILE__<<" , line "<<__LINE__<<std::endl;exit(-1);}}while(0)
#define ERR_LOG_BOOL(x,s) do{if(!(x)){std::cerr<<"Error occurs at file "<<__FILE__<<" , line "<<__LINE__<<std::endl<<"Info "<<(s)<<std::endl;exit(-1);}}while(0)

template <typename Str>
constexpr size_t StringHasher(const Str & toHash)
{
	static_assert(sizeof(size_t) == 8);
	// FNV-1a 64 bit algorithm
	size_t result = 0xcbf29ce484222325; // FNV offset basis

	for (char c : toHash) {
        if(!c) break;
		result ^= c;
		result *= 1099511628211; // FNV prime
	}
	return result;
}

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	float& operator[](int id) const {
		return ((float*)this)[id];
	}
	Vertex() {}
	Vertex(float x0, float x1, float x2, float x3, float x4, float x5) {
		pos.x = x0; pos.y = x1; pos.z = x2;
		normal.x = x3; normal.y = x4; normal.z = x5;
	}
    Vertex(const glm::vec3& p, const glm::vec3& n){
        pos = p;normal = n;
    }
	inline bool operator==(const Vertex& rhs) const {
		return pos == rhs.pos && normal == rhs.normal;
	}
};

template<typename ratio>
struct vec3Less{
    bool operator()(const glm::vec3&a,const glm::vec3&b)const{
        const float eps = (float)ratio::num/ratio::den;
        if(std::fabs(a.x-b.x)>eps)return a.x<b.x;
        if(std::fabs(a.y-b.y)>eps)return a.y<b.y;
        return a.z+eps<b.z;
    }
};

//#define PROFILE_FPS
