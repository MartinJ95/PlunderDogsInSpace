#pragma once

struct Vector
{
	//constructors
public:
	Vector();
	Vector(float S);
	Vector(float X, float Y, float Z);
	Vector(const Vector& other);
	//functions
public:
	inline float LengthSquare() const;
	inline float Length() const;
	inline Vector Normalised() const;
	inline float Dot(const Vector &other) const;
	inline Vector Cross(const Vector& other) const;
    //assignment operators
public:
	inline void operator=(const Vector& other) { x = other.x; y = other.y; z = other.z; }
	//comparison operators
public:
	inline bool operator==(const Vector& other) const { return(x == other.x && y == other.y && z == other.z); };
	inline bool operator!=(const Vector& other) const { return(x != other.x || y != other.y || z != other.z); };
public:
	float x, y, z;
};

