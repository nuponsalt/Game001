#include "DXUT.h"
#include "Animation.h"

namespace KMT 
{
	CAnimation::CAnimation() : DivideWidth(0), DivideHeight(0), ImageSize(), CenterPosition(), Speed(1), Number(0), Count(0), CurrentPlayMode(SELECT_FRAME) { }

	CAnimation::~CAnimation()
	{
		Texture.reset();
	}

	void CAnimation::LoadTextureAndAnimation(const std::string &_path, const int &_x_num, const int &_y_num, const DWORD &dwFilter)
	{
		// テクスチャーの読み込み
		Texture = CTexture::CreateFromFile(_path, dwFilter);
		// 分割情報反映
		DivideWidth = _x_num;
		DivideHeight = _y_num;
		// 分割後の1つのサイズを取得
		D3DXIMAGE_INFO info  = Texture->getd3dImageInfo();
		ImageSize = CVector3(((float)info.Width / DivideWidth), ((float)info.Height / DivideHeight), 0);
		
		// 分割回数を記憶
		int num = 0;
		int _all_div = _x_num * _y_num;
		// 画像をy個縦に分割
		for(int y = 0 ; y < _y_num ; y++){
			// 画像をx個横に分割
			for(int x = 0 ; x < _x_num ; x++){
				// 分割回数が分割総数を超えていたら分割処理を実行しない
				if(num >= _all_div)	continue;
				// 分割範囲の計算
				RECT rect = { 
					LONG(ImageSize.x * x), 
					LONG(ImageSize.y * y), 
					LONG(ImageSize.x * (x + 1)), 
					LONG(ImageSize.y * (y + 1)) 
				};
				// 分割情報を格納
				Rects.push_back(rect);
				num++;
			}
		}
		// 2D の中心
		const float FHALF = 0.5f;
		float _imgsize_divX = ImageSize.x / (float)DivideWidth;
		float _imgsize_divY = ImageSize.y / (float)DivideHeight;
		CenterPosition = CVector3((_imgsize_divX * FHALF ), (_imgsize_divY * FHALF), 0.0f );
	}

	const bool CAnimation::UpdateAnimation(const int &frame)
	{
		int FrameSize = Rects.size();

		switch(CurrentPlayMode){
			// フレーム指定
		case SELECT_FRAME :
			if(frame < FrameSize)
				Number = frame;
			else
				Number = FrameSize - 1;
			return false;

			// 一度だけ再生
		case PLAY_ONCE :
			if(frame == Count)
			{
				Count = 0;
				Number++;
			}
			else
				Count++;

			if(Number >= FrameSize)
			{
				Number = FrameSize - 1;
				return true;
			}
			break;

			// ループ再生
		case PLAY_LOOP :
			if(frame == Count)
			{
				Count = 0;
				Number++;
			}
			else
				Count;

			if(Number >= FrameSize)
				Number = 0;
			
			break;

		default :
			break;
		}

		return false;
	}

}