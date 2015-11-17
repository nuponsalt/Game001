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
	// Texture

	class Texture;

	typedef std::shared_ptr<Texture> TextureSP;
	typedef std::weak_ptr<Texture> TextureWP;

	class Texture sealed
	{
	private :
		// テクスチャのマップ
		static std::unordered_map<std::string, TextureWP> _textures;
		// 文字テクスチャマップ
		static std::unordered_map<std::string, TextureWP> _stringTextures;
		// ファイルパス
		std::string _filePath;
		// テクスチャデータ
		LPDIRECT3DTEXTURE9 _textureData;
		// 画像情報
		D3DXIMAGE_INFO _imageInfo;
		// テクスチャーの番号
		static size_t _textureCount;

		// コンストラクタ
		Texture();

	public :
		// デストラクタ
		~Texture();
		// 空のテクスチャ生成
		// width : 生成時のテクスチャの幅
		// height : 生成時のテクスチャの高さ
		static TextureSP CreateEmpty(const size_t width, const size_t height);
		// 文字が書き込まれたテクスチャの生成
		// width : テクスチャの幅
		// height : テクスチャの高さ
		// color1 : フォントのグラデーションの開始色( ARGBの論理和指定 )
		// color1 : フォントのグラデーションの終了色( ARGBの論理和指定 )
		// font : 使用するフォント
		// string_ : 生成する文字
		// 備考 : マップ登録キーはフォント名＋生成文字
		static TextureSP CreateToFont(int width, int height, int color1, int color2, const std::string font, const std::string string_);
		// リソースからのテクスチャ生成
		// path : ファイルパス
		// filter : フィルタリングの設定
		static TextureSP CreateFromFile(const std::string &path, const DWORD filter = D3DX_FILTER_NONE);
		// 破棄
		// name : 破棄するオブジェクトの名前
		// tips : デフォルト引数のallで全ての配列内オブジェクトを破棄
		static void Destroy(std::string name = "all");

		// 取得 : ファイルパス
		inline std::string GetFilePath() { return _filePath; }
		// 取得 : DirectXテクスチャ情報
		inline LPDIRECT3DTEXTURE9 GetTextureData() { return _textureData; }
		// 取得 : イメージ情報取得
		inline D3DXIMAGE_INFO GetImageInfo() { return _imageInfo; }

	} ;

}