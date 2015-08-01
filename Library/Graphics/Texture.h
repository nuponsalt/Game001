//*********************************************
// DirectXテクスチャリソース管理クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace KMT {

	//--------------------------------------------------------------------
	// CTexture

	class CTexture;

	typedef std::shared_ptr<CTexture> CTextureSP;
	typedef std::weak_ptr<CTexture> CTextureWP;

	class CTexture sealed
	{
	private :
		// テクスチャのマップ
		static std::unordered_map<std::string, CTextureWP> Textures;
		// 文字テクスチャマップ
		static std::unordered_map<std::string, CTextureWP> stringTextures;
		// ファイルパス
		std::string FilePath;
		// テクスチャデータ
		LPDIRECT3DTEXTURE9 pd3dTexture;
		// 画像情報
		D3DXIMAGE_INFO d3dImageInfo;
		// テクスチャーの番号
		static size_t TexCount;

		// コンストラクタ
		CTexture();

	public :
		// デストラクタ
		~CTexture();
		// 空のテクスチャ生成
		// _name : ハッシュマップ登録名
		// nWidth : 生成時のテクスチャの幅
		// nHeight : 生成時のテクスチャの高さ
		static CTextureSP CreateEmpty(const size_t Width, const size_t Height);
		// 文字が書き込まれたテクスチャの生成
		// nWidth : テクスチャの幅
		// nHeight : テクスチャの高さ
		// nFromRGB : フォントのグラデーションの開始色( ARGBの論理和指定 )
		// nToRGB : フォントのグラデーションの終了色( ARGBの論理和指定 )
		// cFont : 使用するフォント
		// cChar : 生成する文字
		// 備考 : マップ登録キーはフォント名＋生成文字
		static CTextureSP CreateToFont(int nWidth, int nHeight, int nFromRGB, int nToRGB, const std::string Font, const std::string Char);
		// リソースからのテクスチャ生成
		// _path : ファイルパス
		static CTextureSP CreateFromFile(const std::string &_path, const DWORD dwFilter = D3DX_FILTER_NONE);
		// 破棄
		// m_name : 破棄するオブジェクトの名前
		// tips : デフォルト引数のallで全ての配列内オブジェクトを破棄
		static void Destroy(std::string _name = "all");

		// 取得 : ファイルパス
		inline std::string getFilePath() { return FilePath; }
		// 取得 : DirectXテクスチャ情報
		inline LPDIRECT3DTEXTURE9 getpd3dTexture() { return pd3dTexture; }
		// 取得 : イメージ情報取得
		inline D3DXIMAGE_INFO getd3dImageInfo() { return d3dImageInfo; }

	} ;

}