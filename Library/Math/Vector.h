//*********************************************************************
// ベクトル情報
// Last Update : 2014.01.08
// Yuta Komatsu
//*********************************************************************
#pragma once

#include "DXUT.h"
#include <DirectXMath.h>
using namespace DirectX;

namespace KMT
{
	class Quaternion;
	class Matrix;

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

		// コンストラクタ
		Vector3() : x(0), y(0), z(0) { }
		// 配列の要素012をXYZとする
		Vector3(const float *pf) : x(pf[0]), y(pf[1]), z(pf[2]) { }
		// 指定座標で生成
		Vector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
		// 代入
		Vector3& operator=(const Vector3& v)
		{
			x = v.x, y = v.y, z = v.z;
			return *this;
		}
		// D3DXVECTOR3からの変換
		Vector3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, z); }
		// D3DXVECTOR4からの変換
		Vector3(const D3DXVECTOR4& v) : x(v.x), y(v.y), z(v.z) { }
		// D3DXVECTOR4への変換
		operator D3DXVECTOR4() const { return D3DXVECTOR4(this->x, this->y, this->z, 1); }
		// XMFLOAT3からの変換
		Vector3(const XMFLOAT3& v) : x(v.x), y(v.y), z(v.z) { }
		// XMFLOAT3への変換
		operator XMFLOAT3() const { return XMFLOAT3(this->x, this->y, this->z); }
		// XMVECTORからの変換
		Vector3(const XMVECTOR& v) : x(XMVectorGetX(v)), y(XMVectorGetY(v)), z(XMVectorGetZ(v)) { }
		// XMVECTORへの変換
		operator XMVECTOR() const { return XMVectorSet(this->x, this->y, this->z, 1); }
		// 比較
			// ==
		bool operator== (const Vector3& v) { return this->x == v.x && this->y == v.y && this->z == v.z; }
			// !=
		bool operator!= (const Vector3& v) { return this->x != v.x || this->y != v.y || this->z != v.z; }
		// 加算
		// 結果のベクトルを生成
		Vector3 operator+ (const Vector3& v) const
		{
			return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
		}
		// 加算
		// 既存のベクトルに代入
		Vector3& operator+= (const Vector3& v)
		{
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;
		}
		// 符号反転
		// 結果のベクトルを生成
		Vector3 operator- () const
		{
			return Vector3(-this->x, -this->y, -this->z);
		}
		// 減算
		// 結果のベクトルを生成
		Vector3 operator- (const Vector3& v) const
		{
			return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
		}
		// 減算
		// 既存のベクトルに代入
		Vector3& operator-= (const Vector3& v)
		{
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}
		// 乗算
		// 結果のベクトルを生成
		Vector3 operator* (const Vector3& v) const
		{
			return Vector3(x * v.x, y * v.y, z * v.z);
		}
		// 乗算
		// 既存のベクトルに代入
		Vector3& operator*= (const Vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		// 除算
		// 結果のベクトルを生成
		Vector3 operator/ (const Vector3& v) const {
			return Vector3(x / v.x, y / v.y, z / v.z);
		}
		// 除算
		// 既存のベクトルに代入
		Vector3& operator/= (const Vector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		// スカラ乗算
		// 結果のベクトルを生成
		// スカラ*ベクトルの場合
		friend Vector3 operator* (float f, const Vector3& v)
		{
			return Vector3(v.x * f, v.y * f, v.z * f);
		}
		// スカラ乗算
		// 結果のベクトルを生成
		// ベクトル*スカラの場合
		Vector3 operator*(float f) const
		{
			return Vector3(x * f, y * f, z * f);
		}
		// スカラー乗算
		// 結果のベクトルを生成
		Vector3 operator* (float f)
		{
			return Vector3(x * f, y * f, z * f);
		}
		// スカラー乗算
		// 既存のベクトルに代入
		Vector3& operator*= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		// スカラー除算
		// 結果のベクトルを生成
		Vector3 operator/ (float f) const
		{
			return Vector3(x / f, y / f, z / f);
		}
		// スカラー除算
		// 既存のベクトルに代入
		Vector3& operator/= (float f)
		{
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}

		//----------------------------------------------------------
		// static const field		
		
		// 左
		static const Vector3 Left()
		{
			return Vector3(-1, 0, 0);
		}
		// 右
		static const Vector3 Right()
		{
			return Vector3(1, 0, 0);
		}
		// 上
		static const Vector3 Up()
		{
			return Vector3(0, 1, 0);
		}
		// 下
		static const Vector3 Down()
		{
			return Vector3(0, -1, 0);
		}
		// 前
		static const Vector3 Forward()
		{
			return Vector3(0, 0, 1);
		}
		// 後
		static const Vector3 Back()
		{
			return Vector3(0, 0, -1);
		}

		//------------------------------------------------------------------
		// Methods

		// 内積
		float Dot(const Vector3& source)
		{
			auto v = (*this);
			return v.x * source.x + v.y * source.y + v.z * source.z;
		}
		// 内積
		friend float Dot(Vector3 a, const Vector3& b)
		{
			return a.Dot(b);
		}
		// 平行成分
		Vector3& Parallel(Vector3 to)
		{
			to.Normalize();
			return *this = Dot(to) * to;
		}
		// 平行成分
		friend Vector3 Parallel(Vector3 from, const Vector3& to)
		{
			return from.Parallel(to);
		}
		// 垂直成分
		Vector3& Vertical(Vector3 to)
		{
			to.Normalize();
			return *this -= Dot(to) * to;
		}
		// 垂直成分
		friend Vector3 Vertical(Vector3 from, const Vector3& to)
		{
			return from.Vertical(to);
		}
		// 射影
		Vector3& Projection(Vector3 to)
		{
			return Parallel(to);
		}
		// 射影
		friend Vector3 Projection(Vector3 from, const Vector3& to)
		{
			return from.Parallel(to);
		}
		// 外積
		Vector3& Cross(const Vector3& source)
		{
			//return *Vector3Cross(this, this, &pv);
			auto v = (*this);
			Vector3 cross;
			cross.x = v.y * source.z - v.z * source.y;
			cross.y = v.z * source.x - v.x * source.z;
			cross.z = v.x * source.y - v.y * source.x;
			return cross;
		}
		// 外積
		friend Vector3 Cross(Vector3 a, Vector3& b)
		{
			return a.Cross(b);
		}
		// 長さ
		float Length() const
		{
			auto v = (*this);
			return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		}
		// 長さ
		friend float Length(const Vector3& v)
		{
			return v.Length();
		}
		// 長さの2乗
		float LengthSq() const
		{
			auto v = (*this);
			return v.x * v.x + v.y * v.y + v.z * v.z;
		}
		// 長さの2乗
		friend float LengthSq(const Vector3& v)
		{
			return v.LengthSq();
		}
		// 線形補間
		// tips: t = 0 のときV1, t = 1 のときV2となる
		friend Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
		{
			return a * (1 - t) + b * t;
		}
		// 正規化
		Vector3& Unit()
		{
			auto v = (*this);
			auto f = 1 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
			return v * f;
		}
		// 正規化
		friend Vector3 Unit(Vector3 v)
		{
			return v.Normalize();
		}
		// 正規化
		Vector3& Normalize()
		{
			return Unit();
		}
		// 正規化
		friend Vector3 Normalize(Vector3 v)
		{
			return v.Unit();
		}
		// 回転
		// 既存のベクトルを変更
		Vector3& Rotate(const Quaternion& q)
		{
			D3DXQUATERNION a(x, y, z, 0), &b(*(D3DXQUATERNION*)&q), c(-b.x, -b.y, -b.z, b.w);
			D3DXQUATERNION d(c * a * b);
			x = d.x; y = d.y; z = d.z;
			return *this;
		}
		// 回転
		// 既存のベクトルを変更
		Vector3& operator*= (const Quaternion& q)
		{
			return Rotate(q);
		}
		// 回転
		// 結果のベクトルを生成
		friend Vector3 Rotate(Vector3 v, const Quaternion& q)
		{
			return v.Rotate(q);
		}
		// 回転
		// 結果のベクトルを生成
		friend Vector3 operator*(Vector3 v, const Quaternion& q)
		{
			return v.Rotate(q);
		}
		// 中心を指定した回転
		Vector3& Rotate(const Quaternion& q, const Vector3& center)
		{
			return *this = (*this - center) * q + center;
		}
		// 中心を指定した回転
		friend Vector3 Rotate(Vector3 v, const Quaternion& q, const Vector3& center)
		{
			return v.Rotate(q, center);
		}
		// 行列による変換
		Vector3& Transform(const Matrix& m)
		{
			D3DXVec3TransformCoord((D3DXVECTOR3*)this, (D3DXVECTOR3*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		Vector3& operator*= (const Matrix& m)
		{
			return Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend Vector3 Transform(Vector3 v, const Matrix& m)
		{
			return v.Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend Vector3 operator* (Vector3 v, const Matrix& m)
		{
			return v.Transform(m);
		}
		// XYZ成分の最大値
		float Max()
		{
			return max(x, max(y, z));
		}
		// XYZ成分の最小値
		float Min()
		{
			return min(x, min(y, z));
		}
		// XYZ成分の最大値
		friend float Max(const Vector3& v)
		{
			return max(v.x, max(v.y, v.z));
		}
		// XYZ成分の最小値
		friend float Min(const Vector3& v)
		{
			return min(v.x, min(v.y, v.z));
		}
		// ２ベクトルのXYZ成分の最大値を組み合わせたベクトル
		friend Vector3 Max(const Vector3& a, const Vector3& b)
		{
			return Vector3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
		}
		// ２ベクトルのＸ，Ｙ，Ｚ成分の最小値を組み合わせたベクトル
		friend Vector3 Min(const Vector3& a, const Vector3& b)
		{
			return Vector3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		}
		// 添字を使ったXYZ成分の操作(0, 1, 2)
		float& operator[](int index) const
		{
			return *((float*)this + index);
		}
		
		// スプライン補間
		Vector3& Hermite(const Vector3& position, const Vector3& tangentA, const Vector3& tangentB, float s)
		{
			Vector3 out;
			D3DXVec3Hermite((D3DXVECTOR3*)&out,
				(D3DXVECTOR3*)this, (D3DXVECTOR3*)&tangentA,
				(D3DXVECTOR3*)&position, (D3DXVECTOR3*)&tangentB,
				s);
			return out;
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

		// 成分0のベクトルを生成
		Vector2() : x(0), y(0) { }
		// 指定したx,yのベクトルを生成
		Vector2(float x_, float y_) : x(x_), y(y_) { }
		// 代入
		Vector2& operator= (const Vector2& v)
		{
			x = v.x, y = v.y;
			return *this;
		}
		// CVector3からの変換
		Vector2(const Vector3& v) : x(v.x), y(v.y) { }
		// CVector3への変換
		operator Vector3() const
		{
			return Vector3(x, y, 0);
		}
		// D3DXVECTOR3からの変換
		Vector2(const D3DXVECTOR3& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const
		{
			return D3DXVECTOR3(x, y, 0);
		}
		// D3DXVECTOR2からの変換
		Vector2(const D3DXVECTOR2& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR2への変換
		operator D3DXVECTOR2() const
		{
			return D3DXVECTOR2(x, y);
		}
		// 比較。ベクトルが等しいときにtrue
		bool operator== (const Vector2& v)
		{
			return x == v.x && y == v.y;
		}
		// 比較。ベクトルが等しくないときにtrue
		bool operator!= (const Vector2& v)
		{
			return x != v.x || y != v.y;
		}
		// 加算。結果のベクトルを生成
		Vector2 operator+ (const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}
		// 加算。既存のベクトルを変更
		Vector2& operator+= (const Vector2& v) {
			x += v.x, y += v.y;
			return *this;
		}
		// 符号反転。結果のベクトルを生成
		Vector2 operator- () const
		{
			return Vector2(-x, -y);
		}
		// 減算。結果のベクトルを生成
		Vector2 operator- (const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}
		// 減算。既存のベクトルを変更
		Vector2& operator-= (const Vector2& v)
		{
			x -= v.x, y -= v.y;
			return *this;
		}
		// 乗算。結果のベクトルを生成
		Vector2 operator* (const Vector2& v) const
		{
			return Vector2(x * v.x, y * v.y);
		}
		// 乗算。既存のベクトルを変更
		Vector2& operator*= (const Vector2& v)
		{
			x *= v.x, y *= v.y;
			return *this;
		}
		// 除算。結果のベクトルを生成
		Vector2 operator/ (const Vector2& v) const
		{
			return Vector2(x / v.x, y / v.y);
		}
		// 除算。既存のベクトルを変更
		Vector2& operator/= (const Vector2& v)
		{
			x /= v.x, y /= v.y;
			return *this;
		}

		// スカラ乗算。結果のベクトルを生成。スカラ＊ベクトルの場合
		friend Vector2 operator* (float f, const Vector2& v)
		{
			return Vector2(v.x * f, v.y * f);
		}
		// スカラ乗算。結果のベクトルを生成。ベクトル＊スカラの場合
		Vector2 operator* (float f) const
		{
			return Vector2(x * f, y * f);
		}
		// スカラ乗算。既存のベクトルを変更
		Vector2& operator*= (float f)
		{
			x *= f, y *= f;
			return *this;
		}
		// スカラ除算。結果のベクトルを生成
		Vector2 operator/ (float f) const
		{
			return Vector2(x / f, y / f);
		}
		// スカラ除算。既存のベクトルを変更
		Vector2& operator/= (float f)
		{
			x /= f, y /= f;
			return *this;
		}
		// 長さ
		float Length() const
		{
			return D3DXVec2Length((D3DXVECTOR2*)this);
		}
		// 長さの2乗 自身を返す
		float LengthSq() const
		{
			return D3DXVec2LengthSq((D3DXVECTOR2*)this);
		}
		// 内積。結果のスカラ値を生成
		float Dot(const Vector2& v) const
		{
			return D3DXVec2Dot((D3DXVECTOR2*)this, (D3DXVECTOR2*)&v);
		}
		// 内積。結果のスカラ値を生成
		friend float Dot(const Vector2& a, const Vector2& b)
		{
			return a.Dot(b);
		}
		// 正規化
		Vector2& Normalize()
		{
			D3DXVec2Normalize((D3DXVECTOR2*)this, (D3DXVECTOR2*)this);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		Vector2& Transform(const Matrix& m)
		{
			D3DXVec2TransformCoord((D3DXVECTOR2*)this, (D3DXVECTOR2*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		Vector2& operator*= (const Matrix& m)
		{
			return Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend Vector2 Transform(Vector2 v, const Matrix& m)
		{
			return v.Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend Vector2 operator* (Vector2 v, const Matrix& m)
		{
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

		// 成分0のベクトルを生成
		Vector4() : x(0), y(0), z(0), w(0) { }
		// 指定したx, y, z, wのベクトルを生成
		Vector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) { }
		// CVector3からの変換
		Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// CVector3への変換
		operator Vector3() const {
			return Vector3(x / w, y / w, z / w);
		}
		// D3DXVECTOR3からの変換
		Vector4(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const {
			return D3DXVECTOR3(x / w, y / w, z / w);
		}
		// D3DXVECTOR4からの変換
		Vector4(const D3DXVECTOR4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
		// D3DXVECTOR4への変換
		operator D3DXVECTOR4() const {
			return D3DXVECTOR4(x, y, z, w);
		}
		// xを設定
		Vector4& SetX(float fx) {
			x = fx;
			return *this;
		}
		// yを設定
		Vector4& SetY(float fy) {
			y = fy;
			return *this;
		}
		// zを設定
		Vector4& SetZ(float fz) {
			z = fz;
			return *this;
		}
		// wを設定
		Vector4& SetW(float fw) {
			w = fw;
			return *this;
		}
		// 比較。ベクトルが等しいときにtrue
		bool operator== (const Vector4& v) {
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		// 比較。ベクトルが等しくないときにtrue
		bool operator!= (const Vector4& v) {
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		// 加算。結果のベクトルを生成
		Vector4 operator+ (const Vector4& v) const {
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		// 加算。既存のベクトルを変更
		Vector4& operator+= (const Vector4& v) {
			x += v.x, y += v.y, z += v.z, w += v.w;
			return *this;
		}
		// 符号反転。結果のベクトルを生成
		Vector4 operator- () const {
			return Vector4(-x, -y, -z, -w);
		}
		// 減算。結果のベクトルを生成
		Vector4 operator- (const Vector4& v) const {
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		// 減算。既存のベクトルを変更
		Vector4& operator-= (const Vector4& v) {
			x -= v.x, y -= v.y, z -= v.z, w -= v.w;
			return *this;
		}
		// 乗算。結果のベクトルを生成
		Vector4 operator* (const Vector4& v) const {
			return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
		}
		// 乗算。既存のベクトルを変更
		Vector4& operator*=(const Vector4& v) {
			x *= v.x, y *= v.y, z *= v.z, w *= v.w;
			return *this;
		}
		// 除算。結果のベクトルを生成
		Vector4 operator/ (const Vector4& v) const {
			return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
		}
		// 除算。既存のベクトルを変更
		Vector4& operator/= (const Vector4& v) {
			x /= v.x, y /= v.y, z /= v.z, w /= v.w;
			return *this;
		}
		// スカラ乗算。結果のベクトルを生成。スカラ＊ベクトルの場合
		friend Vector4 operator*(float f, const Vector4& v) {
			return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
		}
		// スカラ乗算。結果のベクトルを生成。ベクトル＊スカラの場合
		Vector4 operator*(float f) const {
			return Vector4(x * f, y * f, z * f, w * f);
		}
		// スカラ乗算。既存のベクトルを変更
		Vector4& operator*= (float f) {
			x *= f, y *= f, z *= f, w *= f;
			return *this;
		}
		// スカラ除算。結果のベクトルを生成
		Vector4 operator/ (float f) const {
			return Vector4(x / f, y / f, z / f, w / f);
		}
		// スカラ除算。既存のベクトルを変更
		Vector4& operator/= (float f) {
			x /= f, y /= f, z /= f, w /= f;
			return *this;
		}
		// 内積。結果のスカラ値を生成
		float Dot(const Vector4& v) const {
			return D3DXVec4Dot((D3DXVECTOR4*)this, (D3DXVECTOR4*)&v);
		}
		// 内積。結果のスカラ値を生成
		friend float Dot(const Vector4& a, const Vector4& b) {
			return a.Dot(b);
		}
		// 行列による変換。既存のベクトルを変更
		Vector4& Transform(const Matrix& m) {
			D3DXVec4Transform((D3DXVECTOR4*)this, (D3DXVECTOR4*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換。既存のベクトルを変更
		Vector4& operator*= (const Matrix& m) {
			return Transform(m);
		}
		// 行列による変換。結果のベクトルを生成
		friend Vector4 Transform(Vector4 v, const Matrix& m) {
			return v.Transform(m);
		}
		// 行列による変換。結果のベクトルを生成
		friend Vector4 operator*(Vector4 v, const Matrix& m) {
			return v.Transform(m);
		}
		//添字を使ったＸ,Ｙ,Ｚ成分の操作(0, 1, 2)
		float& operator[](int index) const {
			return *((float*)this + index);
		}

	};
}