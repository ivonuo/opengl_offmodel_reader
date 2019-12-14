#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>



class vec2 {
public:
	float x, y;
};

class vec3
{
public:
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	inline float x() const 
	{ 
		return e[0]; 
	}
	inline float y() const 
	{ 
		return e[1]; 
	}
	inline float z() const 
	{ 
		return e[2]; 
	}
	inline float r() const 
	{ 
		return e[0]; 
	}
	inline float g() const 
	{ 
		return e[1]; 
	}
	inline float b() const 
	{ 
		return e[2]; 
	}

	inline const vec3& operator+() 
	{ 
		return *this; 
	}
	inline vec3 operator-() const
	{ 
		return vec3(-e[0], -e[1], -e[2]); 
	}
	inline float operator[](int i) const 
	{ 
		return e[i]; 
	}
	inline float& operator[](int i) 
	{ 
		return e[i]; 
	}

	inline vec3& operator+=(const vec3 &v2)
	{
		e[0] += v2.e[0];
		e[1] += v2.e[1];
		e[2] += v2.e[2];
		return *this;
	}
	inline vec3& operator-=(const vec3 &v2)
	{
		e[0] -= v2.e[0];
		e[1] -= v2.e[1];
		e[2] -= v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const vec3 &v2)
	{
		e[0] *= v2.e[0];
		e[1] *= v2.e[1];
		e[2] *= v2.e[2];
		return *this;
	}
	inline vec3& operator/=(const vec3 &v2)
	{
		e[0] /= v2.e[0];
		e[1] /= v2.e[1];
		e[2] /= v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const float &t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	inline vec3& operator/=(const float &t)
	{
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	inline float length() const
	{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector()
	{
		float k = 1.0 / this->length();
		e[0] *= k;
		e[1] *= k;
		e[2] *= k;
	}


	float e[3];

};

inline float drand48() {
	return rand() / (RAND_MAX + 1.0);
}

inline vec3 random_unit_vector()
{
	return { drand48(), drand48() ,drand48() };
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
	return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}
inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
	return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}
inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
	
	return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}
inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
	return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}
inline vec3 operator*(float t, const vec3 &v2)
{
	return vec3(t * v2[0], t * v2[1], t * v2[2]);
}
inline vec3 operator/(float t, const vec3 &v2)
{
	return vec3(t / v2[0], t / v2[1], t / v2[2]);
}
inline vec3 operator*(const vec3 &v2, float t)
{
	return vec3(t * v2[0], t * v2[1], t * v2[2]);
}
inline vec3 operator/(const vec3 &v2, float t)
{
	return vec3(v2[0]/t, v2[1]/t, v2[2]/t);
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
	return vec3(v1[1] * v2[2] - v1[2] * v2[1], -(v1[0] * v2[2] - v1[2] * v2[0]), v1[0] * v2[1] - v1[1] * v2[0]);
}
inline vec3 unit_vector(const vec3& v1)
{
	return v1/v1.length();
}
inline std::ostream& operator<<(std::ostream& out, const vec3& v) 
{
	out << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
	return out;
}