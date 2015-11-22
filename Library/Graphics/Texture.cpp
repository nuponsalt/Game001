//*********************************************
// DirectXテクスチャリソース管理クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#include "DXUT.h"

#include "Texture.h"
#include "../System/Debug.h"
#include "../Extension.h"

#include <sstream>

namespace KMT {

	// static parameters
	std::unordered_map<std::string, TextureWP> Texture::_textures;
	std::unordered_map<std::string, TextureWP> Texture::_stringTextures;
	size_t Texture::_textureCount = 0;

	Texture::Texture() : _textureData( NULL )
	{}

	Texture::~Texture()
	{
		// イテレータを準備
		// ファイルパスで検索
		std::unordered_map<std::string, TextureWP>::iterator texturesIterator = _textures.find(_filePath);
		// 該当ありだった場合
		if(texturesIterator != _textures.end())
			// mapから消す
			_textures.erase(texturesIterator);

		SAFE_RELEASE(_textureData);
#if _DEBUG
		OutputDebugString(L"Release Texture\n");
#endif
	}

	TextureSP Texture::CreateEmpty( const size_t width, const size_t height )
	{
			
		// テクスチャクラスのポインタを用意
		// 新しくテクスチャクラスを作製
		TextureSP object = TextureSP( new Texture() ) ;

		// widthとheightの大きいほうを基準とする
		int size = width > height ? width : height ;
		int textureSize = 1 ;
		while( textureSize < size )
		{
			textureSize *= 2 ;
		}

		if ( FAILED( D3DXCreateTexture( DXUTGetD3D9Device(), 
			textureSize, 
			textureSize, 
			0, 
			D3DUSAGE_RENDERTARGET, 
			D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, 
			&object->_textureData ) ) )
		{
			// テクスチャの作成に失敗
			// エラーに対応するコードをここに書く
			//CDebug::AddString("Error : Failed to Create Texture\n"); 
		}
		// テクスチャのサイズを記録
		object->_imageInfo.Width = textureSize ;
		object->_imageInfo.Height = textureSize ;
		// テクスチャの総数を更新
		_textureCount ++ ;
		// テクスチャNoをファイルパスの代わりにキーにしてマップに登録
		std::stringstream pathBuffer ;
		pathBuffer.str("");
		pathBuffer << _textureCount << ":" << width << height ;
		std::string path = pathBuffer.str().c_str() ;
		// ファイルパスを保持
		object->_filePath = path;
		// マップに登録
		_textures.insert( std::make_pair( path, TextureWP( object ) ) ) ;

		return object ;
	}


	TextureSP Texture::CreateToFont( int width, int height, int color1, int color2, const std::string font, const std::string string_ ) 
	{
		std::stringstream texturePath("") ;
		texturePath << font << string_ ;
		// 既にこのテクスチャーが存在する場合、返す
		std::unordered_map<std::string, TextureWP>::iterator texturesIterator = _textures.find( texturePath.str() ) ;
		if(texturesIterator != _textures.end())
			return texturesIterator->second.lock();

		//-------------------------------------------------------------------------------
		// フォントサイズはポリゴン依存
		int fontSize = ((width + height) >> 1);
		fontSize = (fontSize <= 32) ? 32 : fontSize;
		int length = 0;

		WCHAR fontBuffer[LF_FACESIZE] = {0};
		DXconvAnsiToWide(fontBuffer, font.c_str(), (font.length()+1));
		// フォント情報設定
		LOGFONT logFont = {fontSize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, 0};
		
		memcpy(logFont.lfFaceName, fontBuffer, sizeof(WCHAR)*LF_FACESIZE);

		HFONT hFont = CreateFontIndirect(&logFont);
		if(!(hFont))
			MessageBox(NULL, TEXT("Error : Failed to create string texture"), NULL, MB_OK | MB_ICONSTOP);

		WCHAR stringBuffer[256] = {0};
		DXconvAnsiToWide(stringBuffer, string_.c_str(), (string_.length() + 1));

		TCHAR *char_ = stringBuffer;
		length = (int)wcslen(char_);
		//-------------------------------------------------------------------------------
		// デバイスコンテキスト取得
		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
		//-----------------------------------------------------------------------------------------------
		LPDIRECT3DTEXTURE9 textureBuffer = NULL;
		TextureSP object = TextureSP(new Texture());
		//-----------------------------------------------------------------------------------------------
		for(int i = 0; i < 1; i++){
			//-------------------------------------------------------------------------------
			// 1 文字コード取得
			size_t code = 0;
#if _UNICODE
			// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
			code = (UINT)*char_;
#else
			// マルチバイト文字の場合、
			// 1バイト文字のコードは1バイト目のUINT変換、
			// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
			if( IsDBCSLeadByte( *char_) ){
				code = (BYTE)char_[0]<<8 | (BYTE)char_[1];
				char_ += 2;
			}else{
				code = char_[0];
				char_++;
			}
#endif	
			//-------------------------------------------------------------------------------
			// フォントビットマップ取得
			TEXTMETRIC textmetric;
			GetTextMetrics(hdc, &textmetric);
			GLYPHMETRICS glyphmetrics;
			CONST MAT2 matrix = {{0,1}, {0,0}, {0,0}, {0,1}};

			//DWORD size = GetGlyphOutline( hdc, code, GGO_BITMAP, &GM, 0, NULL, &matrix ) ;			
			//DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY2_BITMAP, &GM, 0, NULL, &matrix ) ;
			//DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &matrix ) ;
			DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyphmetrics, 0, NULL, &matrix);

			BYTE *ptr = new BYTE[size];

			//BYTE *ptr = ( BYTE* )T2Malloc( sizeof( BYTE ) * size ) ;
			//GetGlyphOutline( hdc, code, GGO_BITMAP, &GM, size, ptr, &matrix ) ;
			//GetGlyphOutline( hdc, code, GGO_GRAY2_BITMAP, &GM, size, ptr, &matrix ) ;
			//GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &matrix ) ;
			GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &glyphmetrics, size, ptr, &matrix);
			//---------------------------------------------------------------
			// テクスチャ作成
			D3DLOCKED_RECT lockedRect;

			if(FAILED(DXUTGetD3D9Device()->CreateTexture(glyphmetrics.gmCellIncX,
				textmetric.tmHeight,
				1,
				D3DUSAGE_DYNAMIC,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&textureBuffer,
				NULL))){
					MessageBox(NULL, TEXT(" !! Font Create Err 02 !! "), NULL, MB_OK | MB_ICONSTOP);
			}

			if(FAILED(textureBuffer->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD))){
				MessageBox(NULL, TEXT(" !! Font Create Err 03 !! "), NULL, MB_OK | MB_ICONSTOP);
			}
			//---------------------------------------------------------------
			// フォント情報の書き込み
			// offsetX, offsetY : 書き出し位置(左上)
			// bitmapWidth, bitmapHeight : フォントビットマップの幅高
			// Level : α値の段階 (GGO_GRAY4_BITMAPは17段階、GGO_GRAY8_BITMAPは65段階)
			int offsetX = glyphmetrics.gmptGlyphOrigin.x;
			int offsetY = textmetric.tmAscent - glyphmetrics.gmptGlyphOrigin.y;
			int bitmapWidth = glyphmetrics.gmBlackBoxX + (4-(glyphmetrics.gmBlackBoxX%4)) % 4;
			int bitmapHeight = glyphmetrics.gmBlackBoxY;
			int level = 65;
			int x, y;
			DWORD alpha, color;

			FillMemory(lockedRect.pBits , lockedRect.Pitch * textmetric.tmHeight, 0);

			int divisor = (offsetY + bitmapHeight);
			int initialRed		= (color1 & 0x00ff0000) >> 16;
			int initialGreen	= (color1 & 0x0000ff00) >> 8;
			int initialBlue		= (color1 & 0x000000ff);

			int finalRed	= (color2 & 0x00ff0000) >> 16;
			int finalGreen	= (color2 & 0x0000ff00) >> 8;
			int finalBlue	= (color2 & 0x000000ff);

			int subtractedRed	= initialRed	- finalRed;
			int subtractedGreen = initialGreen	- finalGreen;
			int subtractedBlue	= initialBlue	- finalBlue;

			float additionalRed		= ((float)subtractedRed		/ (float)divisor);
			float additionalGreen	= ((float)subtractedGreen	/ (float)divisor);
			float additionalBlue	= ((float)subtractedGreen	/ (float)divisor);

			int red, green, blue;

			for(y = offsetY; y < offsetY+bitmapHeight; y++){
				for(x = offsetX; x < offsetX+bitmapWidth; x++){
					//--------------------------------------------------------------
					// ノーマルフォント
					//if( T2FONT_ANTIALIAS & FLG ){
					//	Alpha = ( 255 * ptr[ x-offsetX + bitmapWidth * ( y-offsetY ) ] ) / ( Level-1 ) ;
					//}else{
					//	Alpha = ( 0 == ptr[ x-offsetX + bitmapWidth * ( y-offsetY ) ] )? 0 : 255 ;
					//}

					alpha = (DWORD)ptr[ x-offsetX + bitmapWidth * ( y-offsetY ) ] ;
					alpha = alpha * ( 256 / ( level - 1 ) ) ;
					if( alpha > 255 )
						alpha = 255 ;
					//Alpha = ( 0 == ptr[ x-offsetX + bitmapWidth * ( y-offsetY ) ] )? 0 : 255 ;

					red		= initialRed	- (int)( y * additionalRed ) ;
					green	= initialGreen	- (int)( y * additionalGreen ) ;
					blue	= initialBlue	- (int)( y * additionalBlue ) ;
					color = D3DCOLOR_ARGB( 0, red, green, blue ) | ( alpha << 24 ) ;
					memcpy( ( BYTE* )lockedRect.pBits + lockedRect.Pitch*y + 4*x, &color, sizeof(DWORD) ) ;
					
				}
			}

			textureBuffer->UnlockRect( 0 ) ;

			delete[] ptr;		

		}

		//D3DXVECTOR3 v1( -( width * 0.5f ),  ( height * 0.5f ), 0.0f ) ;
		//D3DXVECTOR3 v2(  ( width * 0.5f ),  ( height * 0.5f ), 0.0f ) ;
		//D3DXVECTOR3 v3(  ( width * 0.5f ), -( height * 0.5f ), 0.0f ) ;
		//D3DXVECTOR3 v4( -( width * 0.5f ), -( height * 0.5f ), 0.0f ) ;
		//CreateSprite( v1, v2, v3, v4, NULL ) ;

		//-------------------------------------------------------------------------------
		// デバイスコンテキストとフォントハンドルの開放
		SelectObject( hdc, oldFont ) ;
		DeleteObject( hFont ) ;
		ReleaseDC( NULL, hdc ) ;	

		//*ppFont = pWkFont ;
		//pWkFont->Init( pStr ) ;

		object->_textureData = textureBuffer ;
		object->_imageInfo.Width = fontSize ;

		_stringTextures.insert(make_pair(texturePath.str(), TextureWP(object)));

		return object ;

	}

	TextureSP Texture::CreateFromFile(const std::string &path, const DWORD filter)
	{
		TextureSP object;
		std::stringstream filePathBuffer;
		filePathBuffer.str("");
		filePathBuffer << path << "Filter : " << filter;
		std::string filePath = filePathBuffer.str().c_str();

		std::unordered_map<std::string, TextureWP>::iterator texturesIterator = _textures.find(filePath);
		// 該当無しだった場合
		if(texturesIterator == _textures.end())
		{
			object = TextureSP(new Texture());

			object->_filePath = filePath;
			// ファイルパスをワイド文字に変換する
			WCHAR pathBuffer[256] = {0} ;
			DXconvAnsiToWide( pathBuffer, path.c_str(), ( path.length() + 1 ) ) ;

			// テクスチャのロード
			D3DXCreateTextureFromFileEx(DXUTGetD3D9Device(),
				// ファイルパス
				pathBuffer,			
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				// フィルター
				filter,
				D3DX_DEFAULT,
				// 透明色の指定 全て0指定でカラーキーなし
				D3DXCOLOR(0,0,0,0),	
				// 画像情報の取得
				&object->_imageInfo,		
				NULL,
				// テクスチャの受け取り
				&object->_textureData			
				);

			// マップに登録
			_textures.insert(std::make_pair(filePath, TextureWP(object)));
		// 該当ありだった場合
		}
		else
			// 作成済みのテクスチャクラスのポインタを取得
			object = TextureSP((*texturesIterator).second);	

		return object;
	}

	void Texture::Destroy(std::string name) 
	{
		// イテレータを用意
		// イテレータをハッシュマップの先頭にセット
		std::unordered_map<std::string, TextureWP>::iterator texturesIterator = _textures.begin() ; 
		// 全部解放
		if( name == "all" ){
			while(texturesIterator != _textures.end())
			{
				(*texturesIterator).second.reset();
				_textures.erase(texturesIterator++);
			}
		}
		else
			// テクスチャを１つだけ解放
		{
			texturesIterator = _textures.find(name);
			if(texturesIterator != _textures.end())
			{
				(*texturesIterator).second.reset();
				_textures.erase(texturesIterator);
			}
		}
	}

}