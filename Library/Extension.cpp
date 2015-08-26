//*********************************************
// 名前空間オブジェクト管理/ヘルパー関数管理
// Last Update : 2013.12.9
// Yuta Komatsu
//*********************************************
#include "DXUT.h"
#include "Extension.h"

// 
#include <time.h>

namespace KMT {

	HRESULT DXconvAnsiToWide(WCHAR* wstrDest, const CHAR* strSrc, int ct){
		
		// 引数の*がNULLの場合と、文字数が1以下の場合はエラー
		if(wstrDest==NULL || strSrc==NULL || ct < 1)
			return E_INVALIDARG;

		int Result = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, wstrDest, ct);
		wstrDest[ct - 1] = 0;

		if(Result == 0)
			return E_FAIL;
		return S_OK;

	}

	int GetRandom(const int &min, const int &max)
	{
		// 一番最初の関数呼び出し時にtime()関数呼び出し
		static bool bflag = false;
		if (!bflag) 
		{
			srand((unsigned int)time(NULL));
			bflag = true;
		}
		// 
		return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
	}

	float calculateSphereVolume(float _rad)
	{
		return _rad * _rad * _rad * D3DX_PI * 3 / 4;
	}

	D3DXVECTOR3 QuaternionTransformCoord(D3DXVECTOR3 v, D3DXQUATERNION qtn)
	{
		D3DXVECTOR3 ret;
		D3DXQUATERNION wkQtn, wkAns, wkQtnC;
		wkQtn.x = v.x;
		wkQtn.y = v.y;
		wkQtn.z = v.z;
		wkQtn.w = 0.0f;
		wkAns = wkQtn * qtn;
		D3DXQuaternionConjugate(&wkQtnC, &qtn);	
		wkAns = wkQtnC * wkAns;
		ret = D3DXVECTOR3(wkAns.x, wkAns.y, wkAns.z);
		return ret;
	}

	float Vector2Cross(Vector2& v1, Vector2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	Vector3 NearOnLine(const Vector3 &p, const Vector3 &a, const Vector3 &b)
	{
		//ベクトルAB AP
		Vector3 AB = b - a;
		Vector3 AP = p - a;		

		//ABの単位ベクトルを計算
		Vector3 nAB = Normalize(AB);

		//Aから線上最近点までの距離(ABベクトルの後ろにあるときはマイナス値)
		float distAX = Dot(nAB, AP);

		//線上最近点
		Vector3 ret = a + (nAB * distAX);

		return ret;
	}

	Vector3 NormalOnPlane(const Vector3 vA, const Vector3& vB, const Vector3& vC)
	{
		// 各頂点から基準点までのベクトル
		Vector3 vAB = vB - vA;
		Vector3 vAC = vC - vA;
		// 法線算出
		Vector3 v = Cross(vAB, vAC);
		return v.Normalize();
	}
}