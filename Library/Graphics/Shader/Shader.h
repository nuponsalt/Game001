//*********************************************************************
// シェーダークラス
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once 

#pragma warning( disable:4238 )

#include <memory>
#include <string>
#include <unordered_map>

#include "../../Math/Vector.h"

namespace KMT {

	class CGraphicBehavior;

	class CShader;
	typedef std::shared_ptr<CShader> CShaderSP;
	typedef std::weak_ptr<CShader> CShaderWP;

	class CShader
	{
	public :
		enum BEGINorEND
		{
			BEGIN = true,
			END = false
		};

		// フォグの種類
		enum FogType
		{
			LINEAR_DISTANCE_FOG,
			EXPONENTIAL_DISTANCE_FOG,
			HEIGHT_FOG
		};

		// デストラクタ
		~CShader();
		
		// 破棄
		static void Destroy(std::string _name = "all");

		//* 取得 *//
		// エフェクト
		inline LPD3DXEFFECT getpd3dEffect() { return pd3dEffect; }
		// ハンドル
		inline D3DXHANDLE* getpWVP() { return &pWVP; }	// ワールド * ビュー * プロジェクション
		D3DXHANDLE* getpHandle(const std::string &_path);	// 指定して取得
		
		//* シェーダー設定 *//
		// アクティブなテクニックをシェーダーに設定
		void setTechnique() const;
		// ワールド * ビュー * プロジェクション 行列をシェーダーに設定
		void setWVPMatrix(const CMatrix& wvp) const;
		// カラーをシェーダーに設定
		void setColor(const CVector4& color) const;
		// テクスチャをシェーダーに設定
		void setTexture(const LPDIRECT3DTEXTURE9 texture) const;
		// シェーダーの使用開始
		void BeginShader() const;
		// シェーダーの使用終了
		void EndShader() const;
		// パスの使用
		void BeginPass(const bool isAddBlend);
		// パスの終了
		void EndPass();
		// 特有のシェーダー設定(純粋仮想関数)
		// _rotmtx : 回転行列
		// _campos : カメラの視点の座標
		virtual void applyEffect(const CMatrix &_rotmtx, const CVector4& _campos) = 0;

		//* パラメーター設定 *//
		// ライトの座標
		inline void setLightDirection(CVector4 _dir) { LightDirection = _dir; }
		inline void setLightDirection(const float _x, const float _y, const float _z) { LightDirection = CVector4(_x, _y, _z, 0.0f);	}
		// 環境光
		inline void setAmbient(const float _ambient) { Ambient = _ambient; }
		// スペキュラー範囲
		inline void setSpecular(const float _specular) { Specular = _specular; }
		// スペキュラー強度
		inline void setSpecularPower(const float _specularpower) { SpecularPower = _specularpower; }
		// フォグカラー
		inline void setFogColor(const CVector4& _fogcolor) { fogColor = _fogcolor; }
		// フォグのパラメーターの設定
		inline void setfogCoord(const float param1, const float param2) { fogCoord = CVector4(param1, param2, 0, 0); }

	protected :
		// 使用するシェーダーファイル名
		std::string	FilePath;
		// 環境光
		float Ambient;
		// 平行光源
		CVector4 LightDirection;
		// スペキュラーの範囲
		float Specular;
		// スペキュラーの強度
		float SpecularPower;
		// フォグカラー
		CVector4 fogColor;
		// フォグの範囲
		CVector4 fogCoord;

		// シェーダーのタイプの列挙型
		enum ShaderType_
		{
			SHADER_NORMAL,
			SHADER_LAMBERT,
			SHADER_HLAMBERT,
			SHADER_PHONG,
			SHADER_TOON,
			SHADER_TYPEMAX
		};
		// シェーダーのタイプ
		size_t Type;

		// エフェクトの設定と問い合わせ、およびテクニックの選択
		// 統括するポインタ
		// シェーダ本体
		LPD3DXEFFECT pd3dEffect;
		// テクニック
		D3DXHANDLE	pTechnique;
		// ワールド x ビュー x プロジェクション
		D3DXHANDLE	pWVP;
		// ディフューズ色
		D3DXHANDLE pColor;
		// テクスチャ
		D3DXHANDLE pSrcTex;
		// シェーダーに渡すハンドルを登録したハッシュマップ
		std::unordered_map<std::string, D3DXHANDLE> handles;
		// シェーダー管理ハッシュマップ配列
		static std::unordered_map<std::string, CShaderSP> Shaders;

		// コンストラクタ
		CShader();
		CShader(std::string _xmlpath);

	};

	class CShaderNormal;
	typedef std::shared_ptr<CShaderNormal> CShaderNormalSP;
	typedef std::weak_ptr<CShaderNormal> CShaderNormalWP;

	class CShaderNormal : public CShader
	{
	public :
		// デストラクタ
		~CShaderNormal() { }
		// シェーダー生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト適用
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);

	private :
		// コンストラクタ
		CShaderNormal();

	};

}