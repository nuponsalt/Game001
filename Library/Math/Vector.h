//*********************************************************************
// �x�N�g�����
// Last Update : 2014.01.08
// Yuta Komatsu
//*********************************************************************
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT {

	class CQuaternion;
	class CMatrix;

	class Vector3;
	float Vector3Length(const Vector3* pV);
	float Vector3LengthSq(const Vector3 *pV);
	float Vector3Dot(const Vector3 *pV1, const Vector3 *pV2);
	Vector3* Vector3Cross(Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2);
	Vector3* Vector3Lerp(Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2, float s);
	Vector3* Vector3Normalize(Vector3 *pOut, const Vector3 *pV);
	Vector3* Vector3Hermite();

	//-----------------------------------------------------
	// 3DVector

	class Vector3
	{
	public:
		// x
		float x;
		// y
		float y;
		// z
		float z;

		// �R���X�g���N�^
		Vector3() : x(0), y(0), z(0) { }
		// �z��̗v�f012��XYZ�Ƃ���
		Vector3(const float *pf) : x(pf[0]), y(pf[1]), z(pf[2]) { }
		// �w����W�Ő���
		Vector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
		// ���
		Vector3& operator=(const Vector3& v) {
			x = v.x, y = v.y, z = v.z;
			return *this;
		}
		// D3DXVECTOR3����̕ϊ�
		Vector3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) { }
		// D3DXVECTOR3�ւ̕ϊ�
		operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, z); }
		// D3DXVECTOR4����̕ϊ�
		Vector3(const D3DXVECTOR4& v) : x(v.x), y(v.y), z(v.z) {}
		// D3DXVECTOR4�ւ̕ϊ�
		operator D3DXVECTOR4() const {
			return D3DXVECTOR4(x, y, z, 1);
		}
		// XMFLOAT3����̕ϊ�
		Vector3(const XMFLOAT3& v) : x(v.x), y(v.y), z(v.z) { }
		// XMFLOAT3�ւ̕ϊ�
		operator XMFLOAT3() const { return XMFLOAT3(x, y, z); }
		// ��r
			// ==
		bool operator== (const Vector3& v) {
			return x == v.x && y == v.y && z == v.z;
		}
		// !=
		bool operator!= (const Vector3& v) {
			return x != v.x || y != v.y || z != v.z;
		}
		// ���Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator+ (const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
		// ���Z
		// �����̃x�N�g���ɑ��
		Vector3& operator+= (const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		// �������]
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator- () const {
			return Vector3(-x, -y, -z);
		}
		// ���Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator- (const Vector3& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		// ���Z
		// �����̃x�N�g���ɑ��
		Vector3& operator-= (const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		// ��Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator* (const Vector3& v) const {
			return Vector3(x * v.x, y * v.y, z * v.z);
		}
		// ��Z
		// �����̃x�N�g���ɑ��
		Vector3& operator*= (const Vector3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		// ���Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator/ (const Vector3& v) const {
			return Vector3(x / v.x, y / v.y, z / v.z);
		}
		// ���Z
		// �����̃x�N�g���ɑ��
		Vector3& operator/= (const Vector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		// �X�J����Z
		// ���ʂ̃x�N�g���𐶐�
		// �X�J��*�x�N�g���̏ꍇ
		friend Vector3 operator* (float f, const Vector3& v) {
			return Vector3(v.x * f, v.y * f, v.z * f);
		}
		// �X�J����Z
		// ���ʂ̃x�N�g���𐶐�
		// �x�N�g��*�X�J���̏ꍇ
		Vector3 operator*(float f) const {
			return Vector3(x * f, y * f, z * f);
		}
		// �X�J���[��Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator* (float f) {
			return Vector3(x * f, y * f, z * f);
		}
		// �X�J���[��Z
		// �����̃x�N�g���ɑ��
		Vector3& operator*= (float f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		// �X�J���[���Z
		// ���ʂ̃x�N�g���𐶐�
		Vector3 operator/ (float f) const {
			return Vector3(x / f, y / f, z / f);
		}
		// �X�J���[���Z
		// �����̃x�N�g���ɑ��
		Vector3& operator/= (float f) {
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}
		// ����
		float Dot(const Vector3& pv) {
			return Vector3Dot(this, &pv);
		}
		// ����
		friend float Dot(Vector3 a, const Vector3& b) {
			return a.Dot(b);
		}
		// ���s����
		Vector3& Parallel(Vector3 to) {
			to.Normalize();
			return *this = Dot(to) * to;
		}
		// ���s����
		friend Vector3 Parallel(Vector3 from, const Vector3& to) {
			return from.Parallel(to);
		}
		// ��������
		Vector3& Vertical(Vector3 to) {
			to.Normalize();
			return *this -= Dot(to) * to;
		}
		// ��������
		friend Vector3 Vertical(Vector3 from, const Vector3& to) {
			return from.Vertical(to);
		}
		// �ˉe
		Vector3& Projection(Vector3 to) {
			return Parallel(to);
		}
		// �ˉe
		friend Vector3 Projection(Vector3 from, const Vector3& to) {
			return from.Parallel(to);
		}
		// �O��
		Vector3& Cross(const Vector3& pv) {
			return *Vector3Cross(this, this, &pv);
		}
		// �O��
		friend Vector3 Cross(Vector3 a, Vector3& b) {
			return a.Cross(b);
		}
		// ����
		float Length() const {
			return Vector3Length(this);
		}
		// ����
		friend float Length(const Vector3& v) {
			return v.Length();
		}
		// ������2��
		float LengthSq() const {
			return Vector3LengthSq(this);
		}
		// ������2��
		friend float LengthSq(const Vector3& v) {
			return v.LengthSq();
		}
		// ���`���
		// tips: f = 0 �̂Ƃ�V1, f = 1 �̂Ƃ�V2�ƂȂ�
		friend Vector3 Lerp(const Vector3& a, const Vector3& b, float f) {
			return a * (1 - f) + b * f;
		}
		// ���K��
		Vector3& Unit() {
			Vector3Normalize(this, this);
			return *this;
		}
		// ���K��
		friend Vector3 Unit(Vector3 v) {
			return v.Normalize();
		}
		// ���K��
		Vector3& Normalize() {
			return Unit();
		}
		// ���K��
		friend Vector3 Normalize(Vector3 v) {
			return v.Unit();
		}
		// ��]
		// �����̃x�N�g����ύX
		Vector3& Rotate(const CQuaternion& q) {
			D3DXQUATERNION a(x, y, z, 0), &b(*(D3DXQUATERNION*)&q), c(-b.x, -b.y, -b.z, b.w);
			D3DXQUATERNION d(c * a * b);
			x = d.x; y = d.y; z = d.z;
			return *this;
		}
		// ��]
		// �����̃x�N�g����ύX
		Vector3& operator*= (const CQuaternion& q) {
			return Rotate(q);
		}
		// ��]
		// ���ʂ̃x�N�g���𐶐�
		friend Vector3 Rotate(Vector3 v, const CQuaternion& q) {
			return v.Rotate(q);
		}
		// ��]
		// ���ʂ̃x�N�g���𐶐�
		friend Vector3 operator*(Vector3 v, const CQuaternion& q) {
			return v.Rotate(q);
		}
		// ���S���w�肵����]
		Vector3& Rotate(const CQuaternion& q, const Vector3& center) {
			return *this = (*this - center) * q + center;
		}
		// ���S���w�肵����]
		friend Vector3 Rotate(Vector3 v, const CQuaternion& q, const Vector3& center) {
			return v.Rotate(q, center);
		}
		// �s��ɂ��ϊ�
		Vector3& Transform(const CMatrix& m) {
			D3DXVec3TransformCoord((D3DXVECTOR3*)this, (D3DXVECTOR3*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// �s��ɂ��ϊ�
		// �����̃x�N�g���ɑ��
		Vector3& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// �s��ɂ��ϊ�
		// ���ʂ̃x�N�g���𐶐�
		friend Vector3 Transform(Vector3 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// �s��ɂ��ϊ�
		// ���ʂ̃x�N�g���𐶐�
		friend Vector3 operator* (Vector3 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// XYZ�����̍ő�l
		float Max() {
			return max(x, max(y, z));
		}
		// XYZ�����̍ŏ��l
		float Min() {
			return min(x, min(y, z));
		}
		// XYZ�����̍ő�l
		friend float Max(const Vector3& v) {
			return max(v.x, max(v.y, v.z));
		}
		// XYZ�����̍ŏ��l
		friend float Min(const Vector3& v) {
			return min(v.x, min(v.y, v.z));
		}
		// �Q�x�N�g����XYZ�����̍ő�l��g�ݍ��킹���x�N�g��
		friend Vector3 Max(const Vector3& a, const Vector3& b) {
			return Vector3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
		}
		// �Q�x�N�g���̂w�C�x�C�y�����̍ŏ��l��g�ݍ��킹���x�N�g��
		friend Vector3 Min(const Vector3& a, const Vector3& b) {
			return Vector3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		}
		// �Y�����g����XYZ�����̑���(0, 1, 2)
		float& operator[](int index) const {
			return *((float*)this + index);
		}
	};

	//-----------------------------------------------------
	// 2DVector

	class Vector2
	{
	public:
		// x
		float x;
		// y
		float y;

		// ����0�̃x�N�g���𐶐�
		Vector2() : x(0), y(0) { }
		// �w�肵��x,y�̃x�N�g���𐶐�
		Vector2(float fx, float fy) : x(fx), y(fy) { }
		// ���
		Vector2& operator= (const Vector2& v) {
			x = v.x, y = v.y;
			return *this;
		}
		// CVector3����̕ϊ�
		Vector2(const Vector3& v) : x(v.x), y(v.y) { }
		// CVector3�ւ̕ϊ�
		operator Vector3() const {
			return Vector3(x, y, 0);
		}
		// D3DXVECTOR3����̕ϊ�
		Vector2(const D3DXVECTOR3& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR3�ւ̕ϊ�
		operator D3DXVECTOR3() const {
			return D3DXVECTOR3(x, y, 0);
		}
		// D3DXVECTOR2����̕ϊ�
		Vector2(const D3DXVECTOR2& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR2�ւ̕ϊ�
		operator D3DXVECTOR2() const {
			return D3DXVECTOR2(x, y);
		}
		// ��r�B�x�N�g�����������Ƃ���true
		bool operator== (const Vector2& v) {
			return x == v.x && y == v.y;
		}
		// ��r�B�x�N�g�����������Ȃ��Ƃ���true
		bool operator!= (const Vector2& v) {
			return x != v.x || y != v.y;
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector2 operator+ (const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector2& operator+= (const Vector2& v) {
			x += v.x, y += v.y;
			return *this;
		}
		// �������]�B���ʂ̃x�N�g���𐶐�
		Vector2 operator- () const {
			return Vector2(-x, -y);
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector2 operator- (const Vector2& v) const {
			return Vector2(x - v.x, y - v.y);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector2& operator-= (const Vector2& v) {
			x -= v.x, y -= v.y;
			return *this;
		}
		// ��Z�B���ʂ̃x�N�g���𐶐�
		Vector2 operator* (const Vector2& v) const {
			return Vector2(x * v.x, y * v.y);
		}
		// ��Z�B�����̃x�N�g����ύX
		Vector2& operator*= (const Vector2& v) {
			x *= v.x, y *= v.y;
			return *this;
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector2 operator/ (const Vector2& v) const {
			return Vector2(x / v.x, y / v.y);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector2& operator/= (const Vector2& v) {
			x /= v.x, y /= v.y;
			return *this;
		}

		// �X�J����Z�B���ʂ̃x�N�g���𐶐��B�X�J�����x�N�g���̏ꍇ
		friend Vector2 operator* (float f, const Vector2& v) {
			return Vector2(v.x * f, v.y * f);
		}
		// �X�J����Z�B���ʂ̃x�N�g���𐶐��B�x�N�g�����X�J���̏ꍇ
		Vector2 operator* (float f) const {
			return Vector2(x * f, y * f);
		}
		// �X�J����Z�B�����̃x�N�g����ύX
		Vector2& operator*= (float f) {
			x *= f, y *= f;
			return *this;
		}
		// �X�J�����Z�B���ʂ̃x�N�g���𐶐�
		Vector2 operator/ (float f) const {
			return Vector2(x / f, y / f);
		}
		// �X�J�����Z�B�����̃x�N�g����ύX
		Vector2& operator/= (float f) {
			x /= f, y /= f;
			return *this;
		}
		// ����
		float Length() const {
			return D3DXVec2Length((D3DXVECTOR2*)this);
		}
		// ������2�� ���g��Ԃ�
		float LengthSq() const {
			return D3DXVec2LengthSq((D3DXVECTOR2*)this);
		}
		// ���ρB���ʂ̃X�J���l�𐶐�
		float Dot(const Vector2& v) const {
			return D3DXVec2Dot((D3DXVECTOR2*)this, (D3DXVECTOR2*)&v);
		}
		// ���ρB���ʂ̃X�J���l�𐶐�
		friend float Dot(const Vector2& a, const Vector2& b) {
			return a.Dot(b);
		}
		// ���K��
		Vector2& Normalize() {
			D3DXVec2Normalize((D3DXVECTOR2*)this, (D3DXVECTOR2*)this);
			return *this;
		}
		// �s��ɂ��ϊ�
		// �����̃x�N�g���ɑ��
		Vector2& Transform(const CMatrix& m) {
			D3DXVec2TransformCoord((D3DXVECTOR2*)this, (D3DXVECTOR2*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// �s��ɂ��ϊ�
		// �����̃x�N�g���ɑ��
		Vector2& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// �s��ɂ��ϊ�
		// ���ʂ̃x�N�g���𐶐�
		friend Vector2 Transform(Vector2 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// �s��ɂ��ϊ�
		// ���ʂ̃x�N�g���𐶐�
		friend Vector2 operator* (Vector2 v, const CMatrix& m) {
			return v.Transform(m);
		}

	};

	//-----------------------------------------------------
	// 4DVector

	class Vector4
	{
	public:
		// x
		float x;
		// y
		float y;
		// z
		float z;
		// w
		float w;

		// ����0�̃x�N�g���𐶐�
		Vector4() : x(0), y(0), z(0), w(0) { }
		// �w�肵��x, y, z, w�̃x�N�g���𐶐�
		Vector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) { }
		// CVector3����̕ϊ�
		Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// CVector3�ւ̕ϊ�
		operator Vector3() const {
			return Vector3(x / w, y / w, z / w);
		}
		// D3DXVECTOR3����̕ϊ�
		Vector4(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// D3DXVECTOR3�ւ̕ϊ�
		operator D3DXVECTOR3() const {
			return D3DXVECTOR3(x / w, y / w, z / w);
		}
		// D3DXVECTOR4����̕ϊ�
		Vector4(const D3DXVECTOR4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
		// D3DXVECTOR4�ւ̕ϊ�
		operator D3DXVECTOR4() const {
			return D3DXVECTOR4(x, y, z, w);
		}
		// x��ݒ�
		Vector4& SetX(float fx) {
			x = fx;
			return *this;
		}
		// y��ݒ�
		Vector4& SetY(float fy) {
			y = fy;
			return *this;
		}
		// z��ݒ�
		Vector4& SetZ(float fz) {
			z = fz;
			return *this;
		}
		// w��ݒ�
		Vector4& SetW(float fw) {
			w = fw;
			return *this;
		}
		// ��r�B�x�N�g�����������Ƃ���true
		bool operator== (const Vector4& v) {
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		// ��r�B�x�N�g�����������Ȃ��Ƃ���true
		bool operator!= (const Vector4& v) {
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector4 operator+ (const Vector4& v) const {
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector4& operator+= (const Vector4& v) {
			x += v.x, y += v.y, z += v.z, w += v.w;
			return *this;
		}
		// �������]�B���ʂ̃x�N�g���𐶐�
		Vector4 operator- () const {
			return Vector4(-x, -y, -z, -w);
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector4 operator- (const Vector4& v) const {
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector4& operator-= (const Vector4& v) {
			x -= v.x, y -= v.y, z -= v.z, w -= v.w;
			return *this;
		}
		// ��Z�B���ʂ̃x�N�g���𐶐�
		Vector4 operator* (const Vector4& v) const {
			return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
		}
		// ��Z�B�����̃x�N�g����ύX
		Vector4& operator*=(const Vector4& v) {
			x *= v.x, y *= v.y, z *= v.z, w *= v.w;
			return *this;
		}
		// ���Z�B���ʂ̃x�N�g���𐶐�
		Vector4 operator/ (const Vector4& v) const {
			return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
		}
		// ���Z�B�����̃x�N�g����ύX
		Vector4& operator/= (const Vector4& v) {
			x /= v.x, y /= v.y, z /= v.z, w /= v.w;
			return *this;
		}
		// �X�J����Z�B���ʂ̃x�N�g���𐶐��B�X�J�����x�N�g���̏ꍇ
		friend Vector4 operator*(float f, const Vector4& v) {
			return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
		}
		// �X�J����Z�B���ʂ̃x�N�g���𐶐��B�x�N�g�����X�J���̏ꍇ
		Vector4 operator*(float f) const {
			return Vector4(x * f, y * f, z * f, w * f);
		}
		// �X�J����Z�B�����̃x�N�g����ύX
		Vector4& operator*= (float f) {
			x *= f, y *= f, z *= f, w *= f;
			return *this;
		}
		// �X�J�����Z�B���ʂ̃x�N�g���𐶐�
		Vector4 operator/ (float f) const {
			return Vector4(x / f, y / f, z / f, w / f);
		}
		// �X�J�����Z�B�����̃x�N�g����ύX
		Vector4& operator/= (float f) {
			x /= f, y /= f, z /= f, w /= f;
			return *this;
		}
		// ���ρB���ʂ̃X�J���l�𐶐�
		float Dot(const Vector4& v) const {
			return D3DXVec4Dot((D3DXVECTOR4*)this, (D3DXVECTOR4*)&v);
		}
		// ���ρB���ʂ̃X�J���l�𐶐�
		friend float Dot(const Vector4& a, const Vector4& b) {
			return a.Dot(b);
		}
		// �s��ɂ��ϊ��B�����̃x�N�g����ύX
		Vector4& Transform(const CMatrix& m) {
			D3DXVec4Transform((D3DXVECTOR4*)this, (D3DXVECTOR4*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// �s��ɂ��ϊ��B�����̃x�N�g����ύX
		Vector4& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// �s��ɂ��ϊ��B���ʂ̃x�N�g���𐶐�
		friend Vector4 Transform(Vector4 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// �s��ɂ��ϊ��B���ʂ̃x�N�g���𐶐�
		friend Vector4 operator*(Vector4 v, const CMatrix& m) {
			return v.Transform(m);
		}
		//�Y�����g�����w,�x,�y�����̑���(0, 1, 2)
		float& operator[](int index) const {
			return *((float*)this + index);
		}

	};
}