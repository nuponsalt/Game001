#pragma once

#pragma warning( disable:4018 )
#pragma warning( disable:4244 )

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT
{
	class GraphicalPlane;
	typedef std::shared_ptr<GraphicalPlane> GraphicalPlaneSP, GPlaneSP;

	typedef class GraphicalPlane : public GraphicBehavior, public Animation
	{
	public :
		// サイズ
		Vector3 _size;

		// デストラクタ
		~GraphicalPlane(){}
		// 生成
		// width : 横幅サイズ
		// height : 縦幅サイズ
		// texture : テクスチャー(空でないとダメな状態)
		static GraphicalPlaneSP Create(const int &width, const int &height, const TextureSP &texture = NULL) ;
		// テクスチャーのサイズに合わせた板ポリゴンの生成
		// path : テクスチャーのパス
		// divisionX, divisionY : 分割量
		// sizeX, sizeY : サイズ
		static GraphicalPlaneSP CreateFromTexture(const std::string &path, 
			const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0) ;
		static GraphicalPlaneSP CreateFromTexture(const TextureSP &texture, 
			const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0) ;
		// 描画
		void Render(const CCamera* camera);

	private :
		// テクスチャーのサイズ
		Vector3 _textureSize;
		// 前のアニメーション番号
		int _previousNumber;
		// 板ポリの生成カウント
		static size_t _createCount;

		// コンストラクタ
		GraphicalPlane();
		// 板ポリゴンの作成
		// width : 横幅サイズ
		// height : 縦幅サイズ
		// texture : テクスチャー
		bool GenerateBoard(const std::string& path, const int &width, const int &height, const TextureSP &texture = NULL) ;
		// テクスチャのサイズに合わせた板ポリを生成する
		// path : テクスチャのパス
		// divisionX, divisionY : 画像の分割情報
		// sizeX, sizeY : 板ポリの大きさ指定
		void LoadTexture (const std::string &path, const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0);
		void LoadTexture (const TextureSP& texture, const int &divisionX = 1,const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0);

	}GPlane;

}