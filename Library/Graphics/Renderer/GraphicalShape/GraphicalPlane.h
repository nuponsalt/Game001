#pragma once

#pragma warning( disable:4018 )
#pragma warning( disable:4244 )

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT
{
	class CGraphicalPlane;
	typedef std::shared_ptr<CGraphicalPlane> CGraphicalPlaneSP, CGPlaneSP;

	typedef class CGraphicalPlane : public CGraphicBehavior, public CAnimation
	{
	public :
		// サイズ
		CVector3 Size;

		// デストラクタ
		~CGraphicalPlane(){}
		// 生成
		// _width : 横幅サイズ
		// _height : 縦幅サイズ
		// _texture : テクスチャー(空でないとダメな状態)
		static CGraphicalPlaneSP Create(const int &_width, const int &_height, const CTextureSP &_texture = NULL) ;
		// テクスチャーのサイズに合わせた板ポリゴンの生成
		// _path : テクスチャーのパス
		// x_num, y_num : 分割量
		// size_x, size_y : サイズ
		static CGraphicalPlaneSP CreateFromTexture(const std::string &_path, 
			const int &x_num = 1, const int &y_num = 1, const int &size_x = 0, const int &size_y = 0) ;
		static CGraphicalPlaneSP CreateFromTexture(const CTextureSP &_tex, 
			const int &x_num = 1, const int &y_num = 1, const int &size_x = 0, const int &size_y = 0) ;
		// 描画
		void Render(const CCamera* camera);

	private :
		// テクスチャーのサイズ
		CVector3 TextureSize;
		// 前のアニメーション番号
		int PreviousNumber;
		// 板ポリの生成カウント
		static size_t CreateCount;

		// コンストラクタ
		CGraphicalPlane();
		// 板ポリゴンの作成
		// _width : 横幅サイズ
		// _height : 縦幅サイズ
		// _texture : テクスチャー
		bool GenerateBoard(const std::string& _path, const int &_width, const int &_height, const CTextureSP &_texture = NULL) ;
		// テクスチャのサイズに合わせた板ポリを生成する
		// _path : テクスチャのパス
		// x_num, y_num : 画像の分割情報
		// size_x, size_y : 板ポリの大きさ指定
		void LoadTexture (const std::string &_path, const int &x_num = 1, const int &y_num = 1, const int &size_x = 0, const int &size_y = 0);
		void LoadTexture (const CTextureSP& _tex, const int& x_num = 1,const int &y_num = 1, const int &size_x = 0, const int &size_y = 0);

	}CGPlane;

}