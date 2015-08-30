#pragma once

namespace KMT {

	class CGraphicsManager
	{
	private :
		
		// コンストラクタ
		CGraphicsManager() {}

		// デストラクタ
		~CGraphicsManager() {}
	public :
		
		// 画像処理に必要なデバイスのポインタ
		static IDirect3DDevice9* pd3dDevice ;

		// バックブッファのサーフェイス
		static LPDIRECT3DSURFACE9 pd3dBackBufferSurface ;

		// バックバッファの深度バッファのサーフェイス
		static LPDIRECT3DSURFACE9 pd3dBackDepthSurface ;

		// 初期化
		static void Initialize( IDirect3DDevice9* _pd3dDevice ) ;

		// 解放
		static void Destroy() ;

		static void setRenderTargetToBackBuffer() ;
	} ;

}