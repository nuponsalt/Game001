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

	//-----------------------------------------------------------
	// CTexture

	// static parameters
	std::unordered_map<std::string, CTextureWP> CTexture::Textures;
	std::unordered_map<std::string, CTextureWP> CTexture::stringTextures;
	size_t CTexture::TexCount = 0;

	CTexture::CTexture() : pd3dTexture( NULL )
	{}

	CTexture::~CTexture()
	{
		// イテレータを準備
		// ファイルパスで検索
		std::unordered_map<std::string, CTextureWP>::iterator it = Textures.find(FilePath);
		// 該当ありだった場合
		if(it != Textures.end())
			// mapから消す
			Textures.erase(it);

		SAFE_RELEASE(pd3dTexture);
#if _DEBUG
		OutputDebugString(L"Release Texture\n");
#endif
	}

	CTextureSP CTexture::CreateEmpty( const size_t nWidth, const size_t nHeight )
	{
			
		// テクスチャクラスのポインタを用意
		// 新しくテクスチャクラスを作製
		CTextureSP ptexture = CTextureSP( new CTexture() ) ;

		// widthとheightの大きいほうを基準とする
		int size = nWidth > nHeight ? nWidth : nHeight ;
		int tex_size = 1 ;
		while( tex_size < size )
		{
			tex_size *= 2 ;
		}

		if ( FAILED( D3DXCreateTexture( DXUTGetD3D9Device(), 
			tex_size, 
			tex_size, 
			0, 
			D3DUSAGE_RENDERTARGET, 
			D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, 
			&ptexture->pd3dTexture ) ) )
		{
			// テクスチャの作成に失敗
			// エラーに対応するコードをここに書く
			//CDebug::AddString("Error : Failed to Create Texture\n"); 
		}
		// テクスチャのサイズを記録
		ptexture->d3dImageInfo.Width = tex_size ;
		ptexture->d3dImageInfo.Height = tex_size ;
		// テクスチャの総数を更新
		TexCount ++ ;
		// テクスチャNoをファイルパスの代わりにキーにしてマップに登録
		std::stringstream ss ;
		ss.str("");
		ss << TexCount << ":" << nWidth << nHeight ;
		std::string str = ss.str().c_str() ;
		// ファイルパスを保持
		ptexture->FilePath = str;
		// マップに登録
		Textures.insert( std::make_pair( str, CTextureWP( ptexture ) ) ) ;

		return ptexture ;
	}


	CTextureSP CTexture::CreateToFont( int nWidth, int nHeight, int nFromRGB, int nToRGB, const std::string cFont, const std::string cChar ) 
	{
		std::stringstream ss("") ;
		ss << cFont << cChar ;
		// 既にこのテクスチャーが存在する場合、返す
		std::unordered_map<std::string, CTextureWP>::iterator it = Textures.find( ss.str() ) ;
		if(it != Textures.end())
			return it->second.lock();
		//-------------------------------------------------------------------------------
		// フォントサイズはポリゴン依存
		int fontsize = ((nWidth + nHeight) >> 1);
		fontsize = (fontsize <= 32) ? 32 : fontsize;
		int Len = 0;

		WCHAR font_buff[LF_FACESIZE] = {0};
		DXconvAnsiToWide(font_buff, cFont.c_str(), (cFont.length()+1));
		// フォント情報設定
		LOGFONT logfont = {fontsize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, 0};
		
		memcpy(logfont.lfFaceName, font_buff, sizeof(WCHAR)*LF_FACESIZE);

		HFONT hFont = CreateFontIndirect(&logfont);
		if(!(hFont))
			MessageBox(NULL, TEXT("Error : Failed to create string texture"), NULL, MB_OK | MB_ICONSTOP);

		WCHAR char_buff[256] = {0};
		DXconvAnsiToWide(char_buff, cChar.c_str(), (cChar.length() + 1));

		TCHAR *c = char_buff;
		Len = (int)wcslen(c);
		//-------------------------------------------------------------------------------
		// デバイスコンテキスト取得
		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
		//-----------------------------------------------------------------------------------------------
		LPDIRECT3DTEXTURE9 pWkTex = NULL;
		CTextureSP ptexture = CTextureSP(new CTexture());
		//-----------------------------------------------------------------------------------------------
		for(int i = 0; i < 1; i++){
			//-------------------------------------------------------------------------------
			// 1 文字コード取得
			size_t code = 0;
#if _UNICODE
			// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
			code = (UINT)*c;
#else
			// マルチバイト文字の場合、
			// 1バイト文字のコードは1バイト目のUINT変換、
			// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
			if( IsDBCSLeadByte( *c ) ){
				code = (BYTE)c[0]<<8 | (BYTE)c[1];
				c += 2;
			}else{
				code = c[0];
				c++;
			}
#endif	
			//-------------------------------------------------------------------------------
			// フォントビットマップ取得
			TEXTMETRIC TM;
			GetTextMetrics(hdc, &TM);
			GLYPHMETRICS GM;
			CONST MAT2 Mat = {{0,1}, {0,0}, {0,0}, {0,1}};

			//DWORD size = GetGlyphOutline( hdc, code, GGO_BITMAP, &GM, 0, NULL, &Mat ) ;			
			//DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY2_BITMAP, &GM, 0, NULL, &Mat ) ;
			//DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat ) ;
			DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);

			BYTE *ptr = new BYTE[size];

			//BYTE *ptr = ( BYTE* )T2Malloc( sizeof( BYTE ) * size ) ;
			//GetGlyphOutline( hdc, code, GGO_BITMAP, &GM, size, ptr, &Mat ) ;
			//GetGlyphOutline( hdc, code, GGO_GRAY2_BITMAP, &GM, size, ptr, &Mat ) ;
			//GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat ) ;
			GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, size, ptr, &Mat);
			//---------------------------------------------------------------
			// テクスチャ作成
			D3DLOCKED_RECT LockedRect;

			if(FAILED(DXUTGetD3D9Device()->CreateTexture(GM.gmCellIncX,
				TM.tmHeight,
				1,
				D3DUSAGE_DYNAMIC,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&pWkTex,
				NULL))){
					MessageBox(NULL, TEXT(" !! Font Create Err 02 !! "), NULL, MB_OK | MB_ICONSTOP);
			}

			if(FAILED(pWkTex->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD))){
				MessageBox(NULL, TEXT(" !! Font Create Err 03 !! "), NULL, MB_OK | MB_ICONSTOP);
			}
			//---------------------------------------------------------------
			// フォント情報の書き込み
			// iOfs_x, iOfs_y : 書き出し位置(左上)
			// iBmp_w, iBmp_h : フォントビットマップの幅高
			// Level : α値の段階 (GGO_GRAY4_BITMAPは17段階、GGO_GRAY8_BITMAPは65段階)
			int iOfs_x = GM.gmptGlyphOrigin.x;
			int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
			int iBmp_w = GM.gmBlackBoxX + (4-(GM.gmBlackBoxX%4)) % 4;
			int iBmp_h = GM.gmBlackBoxY;
			int Level = 65;
			int x, y;
			DWORD Alpha, Color;

			FillMemory(LockedRect.pBits , LockedRect.Pitch * TM.tmHeight, 0);

			for(y = iOfs_y; y < iOfs_y+iBmp_h; y++){
				for(x = iOfs_x; x < iOfs_x+iBmp_w; x++){
					//--------------------------------------------------------------
					// ノーマルフォント
					//if( T2FONT_ANTIALIAS & FLG ){
					//	Alpha = ( 255 * ptr[ x-iOfs_x + iBmp_w * ( y-iOfs_y ) ] ) / ( Level-1 ) ;
					//}else{
					//	Alpha = ( 0 == ptr[ x-iOfs_x + iBmp_w * ( y-iOfs_y ) ] )? 0 : 255 ;
					//}

					Alpha = (DWORD)ptr[ x-iOfs_x + iBmp_w * ( y-iOfs_y ) ] ;
					Alpha = Alpha * ( 256 / ( Level - 1 ) ) ;
					if( Alpha > 255 )
						Alpha = 255 ;
					//Alpha = ( 0 == ptr[ x-iOfs_x + iBmp_w * ( y-iOfs_y ) ] )? 0 : 255 ;
					{
						int wkY = ( iOfs_y + iBmp_h ) ;
						int wkStR = ( nFromRGB & 0x00ff0000 )>>16 ;
						int wkStG = ( nFromRGB & 0x0000ff00 )>>8 ;
						int wkStB = ( nFromRGB & 0x000000ff ) ;

						int wkEnR = ( nToRGB & 0x00ff0000 )>>16 ;
						int wkEnG = ( nToRGB & 0x0000ff00 )>>8 ;
						int wkEnB = ( nToRGB & 0x000000ff ) ;

						int wkSubR = wkStR - wkEnR ;
						int wkSubG = wkStG - wkEnG ;
						int wkSubB = wkStB - wkEnB ;

						float wkAddR = ( (float)wkSubR / (float)wkY ) ;
						float wkAddG = ( (float)wkSubG / (float)wkY ) ;
						float wkAddB = ( (float)wkSubB / (float)wkY ) ;

						int wkR = wkStR - (int)( y * wkAddR ) ;
						int wkG = wkStG - (int)( y * wkAddG ) ;
						int wkB = wkStB - (int)( y * wkAddB ) ;
						Color = D3DCOLOR_ARGB( 0, wkR, wkG, wkB ) | ( Alpha << 24 ) ;
						memcpy( ( BYTE* )LockedRect.pBits + LockedRect.Pitch*y + 4*x, &Color, sizeof(DWORD) ) ;
					}
				}
			}

			pWkTex->UnlockRect( 0 ) ;

			delete[] ptr;		

		}

		D3DXVECTOR3 v1( -( nWidth * 0.5f ),  ( nHeight * 0.5f ), 0.0f ) ;
		D3DXVECTOR3 v2(  ( nWidth * 0.5f ),  ( nHeight * 0.5f ), 0.0f ) ;
		D3DXVECTOR3 v3(  ( nWidth * 0.5f ), -( nHeight * 0.5f ), 0.0f ) ;
		D3DXVECTOR3 v4( -( nWidth * 0.5f ), -( nHeight * 0.5f ), 0.0f ) ;
		//CreateSprite( v1, v2, v3, v4, NULL ) ;

		//-------------------------------------------------------------------------------
		// デバイスコンテキストとフォントハンドルの開放
		SelectObject( hdc, oldFont ) ;
		DeleteObject( hFont ) ;
		ReleaseDC( NULL, hdc ) ;	

		//*ppFont = pWkFont ;
		//pWkFont->Init( pStr ) ;

		ptexture->pd3dTexture = pWkTex ;
		ptexture->d3dImageInfo.Width = fontsize ;

		stringTextures.insert(make_pair(ss.str(), CTextureWP(ptexture)));

		return ptexture ;

	}

	CTextureSP CTexture::CreateFromFile(const std::string &_path, const DWORD dwFilter)
	{
		CTextureSP ptexture;
		std::stringstream ss;
		ss.str("");
		ss << _path << "Filter : " << dwFilter;
		std::string str = ss.str().c_str();

		std::unordered_map<std::string, CTextureWP>::iterator it = Textures.find(str);
		// 該当無しだった場合
		if(it == Textures.end())
		{
			ptexture = CTextureSP(new CTexture());

			ptexture->FilePath = str;
			// ファイルパスをワイド文字に変換する
			WCHAR buff_path[256] = {0} ;
			DXconvAnsiToWide( buff_path, _path.c_str(), ( _path.length() + 1 ) ) ;

			// テクスチャのロード
			D3DXCreateTextureFromFileEx(DXUTGetD3D9Device(),
				// ファイルパス
				buff_path,			
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				// フィルター
				dwFilter,
				D3DX_DEFAULT,
				// 透明色の指定 全て0指定でカラーキーなし
				D3DXCOLOR(0,0,0,0),	
				// 画像情報の取得
				&ptexture->d3dImageInfo,		
				NULL,
				// テクスチャの受け取り
				&ptexture->pd3dTexture			
				);

			// マップに登録
			Textures.insert(std::make_pair(str, CTextureWP(ptexture)));
		// 該当ありだった場合
		}
		else
			// 作成済みのテクスチャクラスのポインタを取得
			ptexture = CTextureSP((*it).second);	

		return ptexture;
	}

	void CTexture::Destroy(std::string _name) 
	{
		// イテレータを用意
		// イテレータをハッシュマップの先頭にセット
		std::unordered_map<std::string, CTextureWP>::iterator it = Textures.begin() ; 
		// 全部解放
		if( _name == "all" ){
			while(it != Textures.end())
			{
				(*it).second.reset();
				Textures.erase(it++);
			}
		}
		else
			// テクスチャを１つだけ解放
		{
			it = Textures.find(_name);
			if(it != Textures.end())
			{
				(*it).second.reset();
				Textures.erase(it);
			}
		}
	}

}