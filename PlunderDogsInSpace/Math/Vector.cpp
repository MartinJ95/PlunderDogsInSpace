#include "Vector.h"
#include <cmath>

Vector::Vector() : x(0), y(0), z(0)
{
}

Vector::Vector(float S) : x(S), y(S), z(S)
{
}

Vector::Vector(float X, float Y, float Z) : x(X), y(Y), z(Z)
{
}

Vector::Vector(const Vector& other) : x(other.x), y(other.y), z(other.z)
{}

inline float Vector::LengthSquare() const
{
	return x * x + y * y + z * z;
}

inline float Vector::Length() const
{
	return sqrt(LengthSquare());
}

inline Vector Vector::Normalised() const
{
	float l = Length();
	Vector v(x/l, y/l, z/l);
	return v;
}

inline float Vector::Dot(const Vector& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

inline Vector Vector::Cross(const Vector& other) const
{
	return Vector(
	y * other.z - z * other.y,
	z * other.x - x * other.z,
	x * other.y - y * other.x
	);
}

