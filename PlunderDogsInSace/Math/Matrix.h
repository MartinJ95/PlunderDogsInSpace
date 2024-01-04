#pragma once
#include <stdio.h>
#include <iostream>
class Matrix
{
public:
	Matrix();
	Matrix(float S);
	Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	inline Matrix operator*(const Matrix& other) const
	{
		return Matrix(
			// first row
			m_00 * other.m_00 + m_01 * other.m_10 + m_02 * other.m_20 + m_03 * other.m_30,
			m_00 * other.m_01 + m_01 * other.m_11 + m_02 * other.m_21 + m_03 * other.m_31,
			m_00 * other.m_02 + m_01 * other.m_12 + m_02 * other.m_22 + m_03 * other.m_32,
			m_00 * other.m_03 + m_01 * other.m_13 + m_02 * other.m_23 + m_03 * other.m_33,
			//second row
			m_10 * other.m_00 + m_11 * other.m_10 + m_12 * other.m_20 + m_13 * other.m_30,
			m_10 * other.m_01 + m_11 * other.m_11 + m_12 * other.m_21 + m_13 * other.m_31,
			m_10 * other.m_02 + m_11 * other.m_12 + m_12 * other.m_22 + m_13 * other.m_32,
			m_10 * other.m_03 + m_11 * other.m_13 + m_12 * other.m_23 + m_13 * other.m_33,
			//third row
			m_20 * other.m_00 + m_01 * other.m_10 + m_22 * other.m_20 + m_23 * other.m_30,
			m_20 * other.m_01 + m_21 * other.m_11 + m_22 * other.m_21 + m_23 * other.m_31,
			m_20 * other.m_02 + m_21 * other.m_12 + m_22 * other.m_22 + m_23 * other.m_32,
			m_20 * other.m_03 + m_21 * other.m_13 + m_22 * other.m_23 + m_23 * other.m_33,
			//forth row
			m_30 * other.m_00 + m_31 * other.m_10 + m_32 * other.m_20 + m_33 * other.m_30,
			m_30 * other.m_01 + m_31 * other.m_11 + m_32 * other.m_21 + m_33 * other.m_31,
			m_30 * other.m_02 + m_31 * other.m_12 + m_32 * other.m_22 + m_33 * other.m_32,
			m_30 * other.m_03 + m_31 * other.m_13 + m_32 * other.m_23 + m_33 * other.m_33);
	}

	inline Matrix Transpose() const
	{
		return Matrix(
			m_00, m_10, m_20, m_30,
			m_01, m_11, m_21, m_31,
			m_02, m_12, m_22, m_32,
			m_03, m_13, m_23, m_33);
	}
public:
	float m_00, m_01, m_02, m_03,
		m_10, m_11, m_12, m_13,
		m_20, m_21, m_22, m_23,
		m_30, m_31, m_32, m_33;
};

