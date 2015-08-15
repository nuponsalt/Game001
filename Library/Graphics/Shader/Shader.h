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

	class Shader;
	typedef std::shared_ptr<Shader> ShaderSP;
	typedef std::weak_ptr<Shader> ShaderWP;

	class Shader
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
		~Shader();
		
		// 破棄
		static void Destroy(std::string name = "all");

		//* 取得 *//
		// エフェクト
		inline LPD3DXEFFECT GetEffect() { return _effect; }
		// ハンドル
		inline D3DXHANDLE* GetWVP() { return &_wvp; }	// ワールド * ビュー * プロジェクション
		D3DXHANDLE* GetHandle(const std::string &path);	// 指定して取得
		
		//* シェーダー設定 *//
		// アクティブなテクニックをシェーダーに設定
		void SetTechnique() const;
		// ワールド * ビュー * プロジェクション 行列をシェーダーに設定
		void SetWVPMatrix(const CMatrix& wvp) const;
		// カラーをシェーダーに設定
		void SetColor(const CVector4& color) const;
		// テクスチャをシェーダーに設定
		void SetTexture(const LPDIRECT3DTEXTURE9 texture) const;
		// シェーダーの使用開始
		void BeginShader() const;
		// シェーダーの使用終了
		void EndShader() const;
		// パスの使用
		void BeginPass(const bool addsBlend);
		// パスの終了
		void EndPass();
		// 特有のシェーダー設定(純粋仮想関数)
		// rotation : 回転行列
		// cameraPosition : カメラの視点の座標
		virtual void ApplyEffect(const CMatrix &rotation, const CVector4& cameraPosition) = 0;

		//* パラメーター設定 *//
		// ライトの座標
		inline void SetLightDirection(CVector4 direction) { _lightDirection = direction; }
		inline void SetLightDirection(const float x, const float y, const float z) { _lightDirection = CVector4(x, y, z, 0.0f); }
		// 環境光
		inline void SetAmbient(const float ambient) { _ambient = ambient; }
		// スペキュラー範囲
		inline void SetSpecular(const float specular) { _specular = specular; }
		// スペキュラー強度
		inline void SetSpecularPower(const float specularPower) { _specularPower = specularPower; }
		// フォグカラー
		inline void SetFogColor(const CVector4& fogColor) { _fogColor = fogColor; }
		// フォグのパラメーターの設定
		inline void SetFogRange(const float param1, const float param2) { _fogRange = CVector4(param1, param2, 0, 0); }

	protected :
		// 使用するシェーダーファイル名
		std::string	_path;
		// 環境光
		float _ambient;
		// 平行光源
		CVector4 _lightDirection;
		// スペキュラーの範囲
		float _specular;
		// スペキュラーの強度
		float _specularPower;
		// フォグカラー
		CVector4 _fogColor;
		// フォグの範囲
		CVector4 _fogRange;

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
		size_t _type;

		// エフェクトの設定と問い合わせ、およびテクニックの選択
		// 統括するポインタ
		// シェーダ本体
		LPD3DXEFFECT _effect;
		// テクニック
		D3DXHANDLE _technique;
		// ワールド x ビュー x プロジェクション
		D3DXHANDLE _wvp;
		// ディフューズ色
		D3DXHANDLE _color;
		// テクスチャ
		D3DXHANDLE _sourceTexture;
		// シェーダーに渡すハンドルを登録したハッシュマップ
		std::unordered_map<std::string, D3DXHANDLE> _handles;
		// シェーダー管理ハッシュマップ配列
		static std::unordered_map<std::string, ShaderSP> _shaders;

		// コンストラクタ
		Shader();
		Shader(std::string xmlPath);

	};
	
	class ShaderNormal;
	typedef std::shared_ptr<ShaderNormal> ShaderNormalSP;
	typedef std::weak_ptr<ShaderNormal> ShaderNormalWP;

	class ShaderNormal : public Shader
	{
	public :
		// デストラクタ
		~ShaderNormal() { }
		// シェーダー生成
		static ShaderSP Create();
		// モデルに対するエフェクト適用
		void ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition);

	private :
		// コンストラクタ
		ShaderNormal();

	};

}