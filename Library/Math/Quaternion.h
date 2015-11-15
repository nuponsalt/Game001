//*********************************************************************
// �N�H�[�^�j�I���N���X
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT {

	class Quaternion
	{
	public :
		float x, y, z, w;
		
		// �R���X�g���N�^
		Quaternion() : x(0), y(0), z(0), w(1) { }
		// �w�肵���v�f�Ő���
		Quaternion(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) { }
		// �w�肵�����Ɗo��̉�]��\���N�H�[�^�j�I���𐶐�
		// �w��p�x�̓��W�A��
		Quaternion(const Vector3& axis, float angle);
		// �w�肵�����Ɗp�x�̉�]��\���N�H�[�^�j�I���𐶐�
		// ��L�R���X�g���N�^�Ƌ@�\�͓���
		friend Quaternion GetQuaternionAroundAxis(const Vector3& axis, float angle)
		{
			return Quaternion(axis, angle);
		}

		// �w�莲�̉�]��\���N�H�[�^�j�I���𐶐�
		// ���̒������p�x�ɂȂ�
		Quaternion(const Vector3& axis);

	};

	// �N�H�[�^�j�I��
	class CQuaternion : public D3DXQUATERNION {
	public:

		// �R���X�g���N�^
		// �P�ʃN�H�[�^�j�I���𐶐�
		CQuaternion() : D3DXQUATERNION(0, 0, 0, 1) {}

		// �w�肵���v�f�Ő���
		CQuaternion(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) {}

		// �w�肵�����Ɗp�x�̉�]��\���N�H�[�^�j�I���𐶐�
		// �w��p�x�̓��W�A���l
		CQuaternion(const Vector3& axis, float angle);

		// �w�肵�����Ɗp�x�̉�]��\���N�H�[�^�j�I���𐶐�
		// ��L�R���X�g���N�^�Ƌ@�\�͓���
		friend CQuaternion RotationAroundAxis(const Vector3& axis, float angle) {
			return CQuaternion(axis, angle);
		}

		// �w�莲�̉�]��\���N�H�[�^�j�I���𐶐�
		// ���̒������p�x�ɂȂ�
		CQuaternion(const Vector3& axis);

		// �w��x�N�g���Ԃ̉�]��\���N�H�[�^�j�I���𐶐�
		// �x�N�g���͐��K�����Ă����K�v�Ȃ�
		CQuaternion(Vector3 vfrom, Vector3 vto);

		// �w��x�N�g���Ԃ̉�]��\���N�H�[�^�j�I���𐶐�
		// �x�N�g���͐��K�����Ă����K�v�Ȃ�
		// ��L�R���X�g���N�^�Ƌ@�\�͓���
		friend CQuaternion RotationBetweenVectors(const Vector3& vfrom, const Vector3& vto) {
			return CQuaternion(vfrom, vto);
		}

		// �w��x�N�g���Ԃ̉�]��\���N�H�[�^�j�I���𐶐�
		// �x�N�g���͐��K�����Ă����K�v�Ȃ�
		// �w�肵�����𒆐S�ɉ�]����̂ŁA�J�����ȂǂœV�n�𔽓]���������Ȃ��ꍇ�ȂǂɎg�p
		CQuaternion(Vector3 vfrom, Vector3 vto, Vector3 axis);

		// �w��x�N�g���Ԃ̉�]��\���N�H�[�^�j�I���𐶐�
		// �x�N�g���͐��K�����Ă����K�v�Ȃ�
		// �w�肵�����𒆐S�ɉ�]����̂ŁA�J�����ȂǂœV�n�𔽓]���������Ȃ��ꍇ�ȂǂɎg�p
		// �@�\�͊Y������R���X�g���N�^�Ɠ���
		friend CQuaternion RotationBetweenVectorsAroundAxis(const Vector3& vfrom, const Vector3& vto, const Vector3& axis) {
			return CQuaternion(vfrom, vto, axis);
		}

		// ���ʐ��`���
		// f = 0�̂Ƃ�v1, f = 1�̂Ƃ�v2
		CQuaternion(const CQuaternion& v1, const CQuaternion& v2, float f);

		// D3DXQUATERNION����̕ϊ�
		CQuaternion(const D3DXQUATERNION& q);

		// D3DXQUATERNION�ւ̕ϊ�
		operator D3DXQUATERNION() const;

		// ���
		CQuaternion& operator= (const CQuaternion& q);

		// ���K��
		// �����̃N�H�[�^�j�I���ɑ��
		CQuaternion& Normalize();

		// ���K��
		// ���ʂ̃N�H�[�^�j�I���𐶐�
		friend CQuaternion Normalize(CQuaternion q) {
			return q.Normalize();
		}

		// �t�N�H�[�^�j�I��
		// �����̃N�H�[�^�j�I���ɑ��
		CQuaternion& Inverse();

		// �t�N�H�[�^�j�I��
		// ���ʂ̃N�H�[�^�j�I���𐶐�
		friend CQuaternion Inverse(CQuaternion q) {
			return q.Inverse();
		}

		CQuaternion& Delta(CQuaternion q);

		friend CQuaternion Delta(CQuaternion q1, CQuaternion q2) {
			return q1.Delta(q2);
		}

	};

}