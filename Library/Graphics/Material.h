//*********************************************
// �J���[�Ǘ��N���X
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include "../Math/Vector.h"

namespace KMT {

	class Material
	{
	public :		
		// �J���[(�r�b�g�l)
		int _color;
		// �J���[(4D�x�N�g��)
		Vector4 _colorRGBA;
		// �R���X�g���N�^
		Material() : _colorRGBA(1, 1, 1, 1), _addsBlend(false) { }
		// �f�X�g���N�^
		virtual ~Material() { }
		// �擾 : ���Z�t���O
		inline bool GetAddsBlend() { return _addsBlend; }
		// �ݒ� : ���Z�t���O
		inline void SetAddsBlend(bool adds) { _addsBlend = adds; }
		// �擾 : �J���[
		inline Vector4 GetColorRGBA() const { return _colorRGBA; }
		// �ݒ� : �J���[
		inline void SetColorRGBA(const float &r, const float &g, const float &b, const float &a) { _colorRGBA = Vector4(r, g, b, a); }
		inline void SetColorRGBA(const Vector4 &rgba) { _colorRGBA = rgba; }

	protected :
		// ���Z�����邩�A�ۂ�
		bool _addsBlend;

	};

}