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
	inline float LengthSquare();
	inline float Length();
	inline Vector Normalised();
	inline float Dot(const Vector &other);
	inline Vector Cross(const Vector& other);
    //assignment operators
public:
	inline void operator=(const Vector& other) { x = other.x; y = other.y; z = other.z; }
	//comparison operators
public:
	inline bool operator==(const Vector& other) { return(x == other.x && y == other.y && z == other.z); };
	inline bool operator!=(const Vector& other) { return(x != other.x || y != other.y || z != other.z); };
public:
	float x, y, z;
};

