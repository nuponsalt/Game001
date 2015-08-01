//*********************************************************************
// ベクトル情報
// Last Update : 2014.01.08
// Yuta Komatsu
//*********************************************************************
#pragma once

namespace KMT {

	class CQuaternion;
	class CMatrix;

	class CVector3;
	float Vector3Length(const CVector3* pV);
	float Vector3LengthSq(const CVector3 *pV);
	float Vector3Dot(const CVector3 *pV1, const CVector3 *pV2);
	CVector3* Vector3Cross(CVector3 *pOut, const CVector3 *pV1, const CVector3 *pV2);
	CVector3* Vector3Lerp(CVector3 *pOut, const CVector3 *pV1, const CVector3 *pV2, float s);
	CVector3* Vector3Normalize(CVector3 *pOut, const CVector3 *pV);
	CVector3* Vector3Hermite();

	//-----------------------------------------------------
	// 3DVector
	
	class CVector3
	{
	public :
		// x
		float x;
		// y
		float y;
		// z
		float z;
		
		// コンストラクタ
		CVector3() : x(0), y(0), z(0) { }
		// 配列の要素012をXYZとする
		CVector3(const float *pf) : x(pf[0]), y(pf[1]), z(pf[2]) { }
		// 指定座標で生成
		CVector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
		// 代入
		CVector3& operator=(const CVector3& v) {
			x = v.x, y = v.y, z = v.z;
			return *this;
		}
		// D3DXVECTOR3からの変換
		CVector3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, z) ; }
		// D3DXVECTOR4からの変換
		CVector3( const D3DXVECTOR4& v ) : x( v.x ), y( v.y ), z( v.z ){}
		// D3DXVECTOR4への変換
		operator D3DXVECTOR4() const {
			return D3DXVECTOR4(x, y, z, 1);
		}
		// 比較
			// ==
		bool operator== (const CVector3& v) {
			return x == v.x && y == v.y && z == v.z;
		}
			// !=
		bool operator!= (const CVector3& v) {
			return x != v.x || y != v.y || z != v.z;
		}
		// 加算
		// 結果のベクトルを生成
		CVector3 operator+ (const CVector3& v) const {
			return CVector3(x + v.x, y + v.y, z + v.z);
		}
		// 加算
		// 既存のベクトルに代入
		CVector3& operator+= (const CVector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		// 符号反転
		// 結果のベクトルを生成
		CVector3 operator- () const {
			return CVector3(-x, -y, -z);
		}
		// 減算
		// 結果のベクトルを生成
		CVector3 operator- (const CVector3& v) const {
			return CVector3(x - v.x, y - v.y, z - v.z);
		}
		// 減算
		// 既存のベクトルに代入
		CVector3& operator-= (const CVector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		// 乗算
		// 結果のベクトルを生成
		CVector3 operator* (const CVector3& v) const {
			return CVector3(x * v.x, y * v.y, z * v.z);
		}
		// 乗算
		// 既存のベクトルに代入
		CVector3& operator*= (const CVector3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		// 除算
		// 結果のベクトルを生成
		CVector3 operator/ (const CVector3& v) const {
			return CVector3(x / v.x, y / v.y, z / v.z);
		}
		// 除算
		// 既存のベクトルに代入
		CVector3& operator/= (const CVector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		// スカラ乗算
		// 結果のベクトルを生成
		// スカラ*ベクトルの場合
		friend CVector3 operator* (float f, const CVector3& v) {
			return CVector3(v.x * f, v.y * f, v.z * f);
		}
		// スカラ乗算
		// 結果のベクトルを生成
		// ベクトル*スカラの場合
		CVector3 operator*(float f) const {
			return CVector3(x * f, y * f, z * f);
		}
		// スカラー乗算
		// 結果のベクトルを生成
		CVector3 operator* (float f) {
			return CVector3(x * f, y * f, z * f);
		}
		// スカラー乗算
		// 既存のベクトルに代入
		CVector3& operator*= (float f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		// スカラー除算
		// 結果のベクトルを生成
		CVector3 operator/ (float f) const {
			return CVector3(x / f, y / f, z / f);
		}
		// スカラー除算
		// 既存のベクトルに代入
		CVector3& operator/= (float f) {
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}
		// 内積
		float Dot(const CVector3& pv) {
			return Vector3Dot(this, &pv);
		}
		// 内積
		friend float Dot(CVector3 a, const CVector3& b){
			return a.Dot(b);
		}
		// 平行成分
		CVector3& Parallel(CVector3 to){
			to.Normalize();
			return *this = Dot(to) * to;
		}
		// 平行成分
		friend CVector3 Parallel(CVector3 from, const CVector3& to) {
			return from.Parallel(to);
		}
		// 垂直成分
		CVector3& Vertical(CVector3 to){
			to.Normalize();
			return *this -= Dot(to) * to;
		}
		// 垂直成分
		friend CVector3 Vertical(CVector3 from, const CVector3& to) {
			return from.Vertical(to);
		}
		// 射影
		CVector3& Projection(CVector3 to) {
			return Parallel(to);
		}
		// 射影
		friend CVector3 Projection(CVector3 from, const CVector3& to) {
			return from.Parallel(to);
		}
		// 外積
		CVector3& Cross(const CVector3& pv) {
			return *Vector3Cross(this, this, &pv);
		}
		// 外積
		friend CVector3 Cross(CVector3 a, CVector3& b) {
			return a.Cross(b);
		}
		// 長さ
		float Length() const {
			return Vector3Length(this); 
		}
		// 長さ
		friend float Length(const CVector3& v) {
			return v.Length();
		}
		// 長さの2乗
		float LengthSq() const {
			return Vector3LengthSq(this);
		}
		// 長さの2乗
		friend float LengthSq(const CVector3& v) {
			return v.LengthSq();
		}
		// 線形補間
		// tips: f = 0 のときV1, f = 1 のときV2となる
		friend CVector3 Lerp(const CVector3& a, const CVector3& b, float f) {
			return a * (1 - f) + b * f;
		}
		// 正規化
		CVector3& Unit() {
			Vector3Normalize(this, this);
			return *this;
		}
		// 正規化
		friend CVector3 Unit(CVector3 v) {
			return v.Normalize();
		}
		// 正規化
		CVector3& Normalize() {
			return Unit();
		}
		// 正規化
		friend CVector3 Normalize(CVector3 v) {
			return v.Unit();
		}
		// 回転
		// 既存のベクトルを変更
		CVector3& Rotate(const CQuaternion& q) {
			D3DXQUATERNION a(x, y, z, 0), &b(*(D3DXQUATERNION*)&q), c(-b.x, -b.y, -b.z, b.w);
			D3DXQUATERNION d(c * a * b);
			x = d.x; y = d.y; z = d.z;
			return *this;
		}
		// 回転
		// 既存のベクトルを変更
		CVector3& operator*= (const CQuaternion& q) {
			return Rotate(q);
		}
		// 回転
		// 結果のベクトルを生成
		friend CVector3 Rotate(CVector3 v, const CQuaternion& q) {
			return v.Rotate(q);
		}
		// 回転
		// 結果のベクトルを生成
		friend CVector3 operator*(CVector3 v, const CQuaternion& q) {
			return v.Rotate(q);
		}
		// 中心を指定した回転
		CVector3& Rotate(const CQuaternion& q, const CVector3& center) {
			return *this = (*this - center) * q + center;
		}
		// 中心を指定した回転
		friend CVector3 Rotate(CVector3 v, const CQuaternion& q, const CVector3& center){
			return v.Rotate(q, center);
		}
		// 行列による変換
		CVector3& Transform(const CMatrix& m) {
			D3DXVec3TransformCoord((D3DXVECTOR3*)this, (D3DXVECTOR3*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		CVector3& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend CVector3 Transform(CVector3 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend CVector3 operator* (CVector3 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// XYZ成分の最大値
		float Max() {
			return max(x, max(y, z));
		}
		// XYZ成分の最小値
		float Min() {
			return min(x, min(y, z));
		}
		// XYZ成分の最大値
		friend float Max(const CVector3& v) {
			return max(v.x, max(v.y, v.z));
		}
		// XYZ成分の最小値
		friend float Min(const CVector3& v) {
			return min(v.x, min(v.y, v.z));
		}
		// ２ベクトルのXYZ成分の最大値を組み合わせたベクトル
		friend CVector3 Max(const CVector3& a, const CVector3& b) {
			return CVector3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
		}
		// ２ベクトルのＸ，Ｙ，Ｚ成分の最小値を組み合わせたベクトル
		friend CVector3 Min(const CVector3& a, const CVector3& b) {
			return CVector3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		}
		// 添字を使ったXYZ成分の操作(0, 1, 2)
		float& operator[](int index) const {
			return *((float*)this + index);
		}
	};

	//-----------------------------------------------------
	// 2DVector

	class CVector2
	{
	public :
		// x
		float x;
		// y
		float y;

		// 成分0のベクトルを生成
		CVector2() : x(0), y(0) { }
		// 指定したx,yのベクトルを生成
		CVector2(float fx, float fy) : x(fx), y(fy) { }
		// 代入
		CVector2& operator= (const CVector2& v) {
			x = v.x, y = v.y;
			return *this;
		}
		// CVector3からの変換
		CVector2(const CVector3& v) : x(v.x), y(v.y) { }
		// CVector3への変換
		operator CVector3() const {
			return CVector3(x, y, 0);
		}
		// D3DXVECTOR3からの変換
		CVector2(const D3DXVECTOR3& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const { 
			return D3DXVECTOR3(x, y, 0); 
		}
		// D3DXVECTOR2からの変換
		CVector2(const D3DXVECTOR2& v) : x(v.x), y(v.y) { }
		// D3DXVECTOR2への変換
		operator D3DXVECTOR2() const { 
			return D3DXVECTOR2(x, y);
		}
		// 比較。ベクトルが等しいときにtrue
		bool operator== (const CVector2& v) {
			return x == v.x && y == v.y;
		}
		// 比較。ベクトルが等しくないときにtrue
		bool operator!= (const CVector2& v) {
			return x != v.x || y != v.y;
		}
		// 加算。結果のベクトルを生成
		CVector2 operator+ (const CVector2& v) const {
			return CVector2(x + v.x, y + v.y);
		}
		// 加算。既存のベクトルを変更
		CVector2& operator+= (const CVector2& v) {
			x += v.x, y += v.y;
			return *this;
		}
		// 符号反転。結果のベクトルを生成
		CVector2 operator- () const {
			return CVector2(-x, -y);
		}
		// 減算。結果のベクトルを生成
		CVector2 operator- (const CVector2& v) const {
			return CVector2(x - v.x, y - v.y);
		}
		// 減算。既存のベクトルを変更
		CVector2& operator-= (const CVector2& v) {
			x -= v.x, y -= v.y;
			return *this;
		}
		// 乗算。結果のベクトルを生成
		CVector2 operator* (const CVector2& v) const {
			return CVector2(x * v.x, y * v.y);
		}
		// 乗算。既存のベクトルを変更
		CVector2& operator*= (const CVector2& v) {
			x *= v.x, y *= v.y;
			return *this;
		}
		// 除算。結果のベクトルを生成
		CVector2 operator/ (const CVector2& v) const {
			return CVector2(x / v.x, y / v.y);
		}
		// 除算。既存のベクトルを変更
		CVector2& operator/= (const CVector2& v) {
			x /= v.x, y /= v.y;
			return *this;
		}

		// スカラ乗算。結果のベクトルを生成。スカラ＊ベクトルの場合
		friend CVector2 operator* (float f, const CVector2& v) {
			return CVector2(v.x * f, v.y * f);
		}
		// スカラ乗算。結果のベクトルを生成。ベクトル＊スカラの場合
		CVector2 operator* (float f) const {
			return CVector2(x * f, y * f);
		}
		// スカラ乗算。既存のベクトルを変更
		CVector2& operator*= (float f) {
			x *= f, y *= f;
			return *this;
		}
		// スカラ除算。結果のベクトルを生成
		CVector2 operator/ (float f) const {
			return CVector2(x / f, y / f);
		}
		// スカラ除算。既存のベクトルを変更
		CVector2& operator/= (float f) {
			x /= f, y /= f;
			return *this;
		}
		// 長さ
		float Length() const {
			return D3DXVec2Length((D3DXVECTOR2*)this); 
		}
		// 長さの2乗 自身を返す
		float LengthSq() const {
			return D3DXVec2LengthSq((D3DXVECTOR2*)this);
		}
		// 内積。結果のスカラ値を生成
		float Dot(const CVector2& v) const {
			return D3DXVec2Dot((D3DXVECTOR2*)this, (D3DXVECTOR2*)&v);
		}
		// 内積。結果のスカラ値を生成
		friend float Dot(const CVector2& a, const CVector2& b) {
			return a.Dot(b);
		}
		// 正規化
		CVector2& Normalize() {
			D3DXVec2Normalize((D3DXVECTOR2*)this, (D3DXVECTOR2*)this);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		CVector2& Transform(const CMatrix& m) {
			D3DXVec2TransformCoord((D3DXVECTOR2*)this, (D3DXVECTOR2*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換
		// 既存のベクトルに代入
		CVector2& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend CVector2 Transform(CVector2 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// 行列による変換
		// 結果のベクトルを生成
		friend CVector2 operator* (CVector2 v, const CMatrix& m) {
			return v.Transform(m);
		}

	} ;

	//-----------------------------------------------------
	// 4DVector

	class CVector4 
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
		CVector4() : x(0), y(0), z(0), w(0) { }
		// 指定したx, y, z, wのベクトルを生成
		CVector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) { }
		// CVector3からの変換
		CVector4(const CVector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// CVector3への変換
		operator CVector3() const { 
			return CVector3(x / w, y / w, z / w); 
		}
		// D3DXVECTOR3からの変換
		CVector4(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		// D3DXVECTOR3への変換
		operator D3DXVECTOR3() const { 
			return D3DXVECTOR3(x / w, y / w, z / w); 
		}
		// D3DXVECTOR4からの変換
		CVector4(const D3DXVECTOR4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
		// D3DXVECTOR4への変換
		operator D3DXVECTOR4() const { 
			return D3DXVECTOR4(x, y, z, w);
		}
		// xを設定
		CVector4& SetX(float fx) { 
			x = fx;
			return *this;
		}
		// yを設定
		CVector4& SetY(float fy) { 
			y = fy;
			return *this;
		}
		// zを設定
		CVector4& SetZ(float fz) { 
			z = fz;
			return *this;
		}
		// wを設定
		CVector4& SetW(float fw) { 
			w = fw;
			return *this;
		}
		// 比較。ベクトルが等しいときにtrue
		bool operator== (const CVector4& v) {
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		// 比較。ベクトルが等しくないときにtrue
		bool operator!= (const CVector4& v) {
			return x != v.x || y != v.y || z != v.z || w != v.w;
		}
		// 加算。結果のベクトルを生成
		CVector4 operator+ (const CVector4& v) const {
			return CVector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		// 加算。既存のベクトルを変更
		CVector4& operator+= (const CVector4& v) {
			x += v.x, y += v.y, z += v.z, w += v.w;
			return *this;
		}
		// 符号反転。結果のベクトルを生成
		CVector4 operator- () const {
			return CVector4(-x, -y, -z, -w);
		}
		// 減算。結果のベクトルを生成
		CVector4 operator- (const CVector4& v) const {
			return CVector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		// 減算。既存のベクトルを変更
		CVector4& operator-= (const CVector4& v) {
			x -= v.x, y -= v.y, z -= v.z, w -= v.w;
			return *this;
		}
		// 乗算。結果のベクトルを生成
		CVector4 operator* (const CVector4& v) const {
			return CVector4(x * v.x, y * v.y, z * v.z, w * v.w);
		}
		// 乗算。既存のベクトルを変更
		CVector4& operator*=(const CVector4& v) {
			x *= v.x, y *= v.y, z *= v.z, w *= v.w;
			return *this;
		}
		// 除算。結果のベクトルを生成
		CVector4 operator/ (const CVector4& v) const {
			return CVector4(x / v.x, y / v.y, z / v.z, w / v.w);
		}
		// 除算。既存のベクトルを変更
		CVector4& operator/= (const CVector4& v) {
			x /= v.x, y /= v.y, z /= v.z, w /= v.w;
			return *this;
		}
		// スカラ乗算。結果のベクトルを生成。スカラ＊ベクトルの場合
		friend CVector4 operator*(float f, const CVector4& v) {
			return CVector4(v.x * f, v.y * f, v.z * f, v.w * f);
		}
		// スカラ乗算。結果のベクトルを生成。ベクトル＊スカラの場合
		CVector4 operator*(float f) const {
			return CVector4(x * f, y * f, z * f, w * f);
		}
		// スカラ乗算。既存のベクトルを変更
		CVector4& operator*= (float f) {
			x *= f, y *= f, z *= f, w *= f;
			return *this;
		}
		// スカラ除算。結果のベクトルを生成
		CVector4 operator/ (float f) const {
			return CVector4(x / f, y / f, z / f, w / f);
		}
		// スカラ除算。既存のベクトルを変更
		CVector4& operator/= (float f) {
			x /= f, y /= f, z /= f, w /= f;
			return *this;
		}
		// 内積。結果のスカラ値を生成
		float Dot(const CVector4& v) const {
			return D3DXVec4Dot((D3DXVECTOR4*)this, (D3DXVECTOR4*)&v);
		}
		// 内積。結果のスカラ値を生成
		friend float Dot(const CVector4& a, const CVector4& b) {
			return a.Dot(b);
		}
		// 行列による変換。既存のベクトルを変更
		CVector4& Transform(const CMatrix& m) {
			D3DXVec4Transform((D3DXVECTOR4*)this, (D3DXVECTOR4*)this, (D3DXMATRIX*)&m);
			return *this;
		}
		// 行列による変換。既存のベクトルを変更
		CVector4& operator*= (const CMatrix& m) {
			return Transform(m);
		}
		// 行列による変換。結果のベクトルを生成
		friend CVector4 Transform(CVector4 v, const CMatrix& m) {
			return v.Transform(m);
		}
		// 行列による変換。結果のベクトルを生成
		friend CVector4 operator*(CVector4 v, const CMatrix& m) {
			return v.Transform(m);
		}
		//添字を使ったＸ,Ｙ,Ｚ成分の操作(0, 1, 2)
		float& operator[](int index) const {
			return *((float*)this + index);
		}

	};
}