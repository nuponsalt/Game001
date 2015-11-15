//*********************************************************************
// �e�L�X�g�\���N���X
// Last Update : 2013.12.5
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Text.h"
#include "GraphicsManager.h"

namespace KMT {

	DebugText::DebugText() : _color(0xFFFFFFFF) { }
	
	DebugText::~DebugText() 
	{
		SAFE_RELEASE(_font);
	}

	DebugTextSP DebugText::CreateText(size_t size, const LPCWCHAR fontName) 
	{
		DebugTextSP text  = DebugTextSP(new DebugText);
		// �t�H���g�̏���ݒ�
		D3DXFONT_DESC fontDesc;
		// �t�H���g�̍���
		fontDesc.Height = size;
		// �t�H���g�̕�
		fontDesc.Width = size / 2;
		fontDesc.Weight = 0;
		fontDesc.MipLevels = 0;
		fontDesc.Italic = FALSE;
		fontDesc.CharSet = SHIFTJIS_CHARSET;
		fontDesc.OutputPrecision = OUT_TT_ONLY_PRECIS;
		fontDesc.Quality = DEFAULT_QUALITY;
		fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
		lstrcpy(fontDesc.FaceName, fontName);
		// �t�H���g����
		D3DXCreateFontIndirect(GraphicsManager::_device, &fontDesc, &text->_font);
		// ���ʂ̃I�u�W�F�N�g���߂�
		return text;
	}

	void DebugText::drawText(const int left, const int top, const DWORD color, LPCWSTR text, ...)
	{
		// �`��ʒu�̐ݒ�
		RECT _rect = { left, top, 0, 0 };
		// �J���[�ۑ�
		_color = color;
		// �ϒ��������當����ɕϊ�
		va_list va;
		va_start(va, text);
		WCHAR buffer[255] = { 0 };
		vswprintf_s(buffer, text, va);
		va_end(va);

		HRESULT hr;
		if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
		{
			// �e�L�X�g�`��
			// text : �`�悷�镶���� (WCHAR �w��)
			// _rect : �`��͈͎w��
			// nColor : �F�w��
			_font->DrawText(NULL, buffer,	-1, &_rect,	DT_LEFT | DT_NOCLIP, color);
			// �`��I��
			V(GraphicsManager::_device->EndScene());
		}
	}

}