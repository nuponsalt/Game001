//*********************************************************************
// ���W�E��]�E�g�k�̊Ǘ��N���X(�����N���C�u�����Ƃ��Ă̋@�\�����\��)
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once
#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace KMT {

	class Vector3;
	class CMatrix;
	class CQuaternion;

	class Transformation;
	typedef std::shared_ptr<Transformation> CTransformationSP;
	typedef std::weak_ptr<Transformation> CTransformationWP;

	// ��]�^�C�v
	enum ROTATE_TYPE
	{
			XYZ,
			MATRIX,
			QUATERNION
	};

	class Transformation
	{
	public :
		//* ���W *//
		Vector3 Position;
		//* ��] *//
		// �x�N�g��
		Vector3 vRotation;
		// �s��
		CMatrix mRotationX, mRotationY, mRotationZ, mRotationWorld;
		// �N�H�[�^�j�I��
		CQuaternion qRotation;
		//* �g�k *//
		Vector3 Scale ;

		// ���݂̉�]�^�C�v
		ROTATE_TYPE CurrentRotateType;

		// �R���X�g���N�^
		Transformation();
		// �f�X�g���N�^
		virtual ~Transformation() { }
	
};

}