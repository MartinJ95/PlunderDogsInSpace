#include "Matrix.h"

Matrix::Matrix()
{
	std::memset(&m_00, 0.f, 16);
}

Matrix::Matrix(float S)
{
	std::memset(&m_00, S, 16);
}

Matrix::Matrix(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) : m_00(m00),m_01(m01),m_02(m02),m_03(m03),
	m_10(m10),m_11(m11),m_12(m02),m_13(m13),
	m_20(m20),m_21(m21),m_22(m02),m_23(m23),
	m_30(m30),m_31(m31),m_32(m02),m_33(m33)
{}