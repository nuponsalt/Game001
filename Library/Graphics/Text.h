//*********************************************
// �e�L�X�g�\���N���X
// Last Update : 2013.12.5
// Yuta Komatsu
//*********************************************
#pragma once

#include <string>
#include <memory>
#include "../Math/Vector.h"

namespace KMT {
	// �O���錾
	class DebugText;
	typedef std::shared_ptr<DebugText> DebugTextSP;
	typedef std::weak_ptr<DebugText> DebugTextWP;
	
	//**�f�o�b�O��p�e�L�X�g�`��N���X**//
	class DebugText
	{
	public:
		// �f�X�g���N�^
		~DebugText();

		// �t�H���g�̐���
		// nSizeX : �����T�C�Y
		// wFont : �g�p�t�H���g
		static DebugTextSP CreateText(size_t size, const LPCWCHAR fontName);
		// ������̕`��
		// �����@�`�悷�镶����
		void drawText(const int _Left, const int _Top, const DWORD _bitColor, LPCWSTR _Text, ...);

	private:
		// �t�H���g
		LPD3DXFONT _font;
		// �F���(�r�b�g)
		DWORD _color;

		// �R���X�g���N�^
		DebugText();	
	};

}