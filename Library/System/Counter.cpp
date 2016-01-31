#include "DXUT.h"

#include "Counter.h"

namespace KMT {

	Counter::Counter() : _position(), _interval(0) { }

	void Counter::LoadFromFile(const std::string& _path)
	{
		if(_image == NULL)
			_image = Sprite::CreateFromFile(_path, DIV_X_NUM, DIV_Y_NUM);
	}

	CounterSP Counter::CreateCounter()
	{
		CounterSP object(new Counter());
		// “Ç‚İ‚İ
		object->LoadFromFile("Resource/Texture/Number.png");
		object->GetImage()->SetPlayMode(PlayMode::SELECT_FRAME);

		return object;
	}

	CounterSP Counter::CreateCounter(const std::string &_path)
	{
		CounterSP object(new Counter());
		// “Ç‚İ‚İ
		object->LoadFromFile(_path);
		object->GetImage()->SetPlayMode(PlayMode::SELECT_FRAME);

		return object;
	}

	void Counter::Render(const AlignState &type, const int &number)
	{
		// ó‚¯æ‚Á‚½”’l‚Ì•¶š”‚ğæ“¾‚·‚é
		int place = 0;
		char buffer[MAX_NUM] = {0};

		place = sprintf_s(buffer, "%d", number);

		// ¶Šñ‚¹‚Ìê‡
		if(LEFT_ALIGN == type) 
		{
			for(int i = 0 ; i < place ; i++)
			{
				_image->Position = D3DXVECTOR3(_position.x + (i * _interval), _position.y, 0);
				_image->UpdateAnimation((buffer[ i ] - '0'));
				_image->Render();
			}
		}
		
		// ‰EŠñ‚¹‚Ìê‡
		if(RIGHT_ALIGN == type)
		{
			for( int i = 0 ; i < place ; ++i )
			{
				_image->Position = D3DXVECTOR3(_position.x - ((place - i) * _interval), _position.y, 0);
				_image->UpdateAnimation((buffer[ i ] - '0'));
				_image->Render();
			}
		}
	}

}