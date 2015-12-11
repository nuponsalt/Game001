#include "DXUT.h"
#include "Animation.h"

namespace KMT 
{
	Animation::Animation() : _divisionWidth(0), _divisionHeight(0), _imageSize(), _centerPosition(), _speed(1), _number(0), _count(0), _currentPlayMode(SELECT_FRAME) { }

	Animation::~Animation()
	{
		_texture.reset();
	}

	void Animation::LoadTextureAndAnimation(const std::string &path, const int &divisionX, const int &divisionY, const DWORD &filter)
	{
		// テクスチャーの読み込み
		_texture = Texture::CreateFromFile(path, filter);
		// 分割情報反映
		_divisionWidth = divisionX;
		_divisionHeight = divisionY;
		// 分割後の1つのサイズを取得
		D3DXIMAGE_INFO info  = _texture->GetImageInfo();
		_imageSize = Vector3(((float)info.Width / _divisionWidth), ((float)info.Height / _divisionHeight), 0);
		
		// 分割回数を記憶
		int count = 0;
		int sum = divisionX * divisionY;
		// 画像をy個縦に分割
		for(int y = 0 ; y < divisionY ; y++){
			// 画像をx個横に分割
			for(int x = 0 ; x < divisionX ; x++){
				// 分割回数が分割総数を超えていたら分割処理を実行しない
				if(count >= sum)	continue;
				// 分割範囲の計算
				RECT rect = { 
					LONG(_imageSize.x * x), 
					LONG(_imageSize.y * y), 
					LONG(_imageSize.x * (x + 1)), 
					LONG(_imageSize.y * (y + 1)) 
				};
				// 分割情報を格納
				_rects.push_back(rect);
				count++;
			}
		}

		// 2D の中心
		const float FHALF = 0.5f;
		// 一枚分の幅
		float x = _imageSize.x / (float)_divisionWidth;
		float y = _imageSize.y / (float)_divisionHeight;
		_centerPosition = Vector3((x * FHALF), (y * FHALF), 0.0f);
	}

	const bool Animation::UpdateAnimation(const int &frame)
	{
		int frameSize = _rects.size();

		switch(_currentPlayMode){
			// フレーム指定
		case SELECT_FRAME :
			if(frame < frameSize)
				_number = frame;
			else
				_number = frameSize - 1;
			return false;

			// 一度だけ再生
		case PLAY_ONCE :
			if(frame == _count)
			{
				_count = 0;
				_number++;
			}
			else
				_count++;

			if(_number >= frameSize)
			{
				_number = frameSize - 1;
				return true;
			}
			break;

			// ループ再生
		case PLAY_LOOP :
			if(frame == _count)
			{
				_count = 0;
				_number++;
			}
			else
				_count;

			if(_number >= frameSize)
				_number = 0;
			
			break;

		default :
			break;
		}

		return false;
	}

}