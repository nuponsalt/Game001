#pragma once

namespace KMT {

	class GraphicsManager
	{
	private :
		
		// コンストラクタ
		GraphicsManager() {}

		// デストラクタ
		~GraphicsManager() {}
	public :
		
		// 画像処理に必要なデバイスのポインタ
		static IDirect3DDevice9* _device ;

		// バックブッファのサーフェイス
		static LPDIRECT3DSURFACE9 _backBufferSurface ;

		// バックバッファの深度バッファのサーフェイス
		static LPDIRECT3DSURFACE9 _backDepthSurface ;

		// 初期化
		static void Initialize( IDirect3DDevice9* device ) ;

		// 解放
		static void Destroy() ;

		static void SetRenderTargetToBackBuffer() ;
	} ;

}