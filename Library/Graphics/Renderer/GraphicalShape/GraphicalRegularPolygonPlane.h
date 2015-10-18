#pragma once

#pragma warning(disable:4018)
#pragma warning(disable:4244)

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT {

	enum FaceGenerateType
	{
		// �ʂ��Ƃ�3���_
		TRI_VTX,
		// �p��+1 = ���_��
		POLYNUM_VTX
	};

	class GraphicalRegularPolygonPlane;
	typedef std::shared_ptr<GraphicalRegularPolygonPlane> GraphicalRegularPolygonPlaneSP, RegPolyPlaneSP;
	
	class GraphicalRegularPolygonPlane : public GraphicBehavior
	{
	public :
		// �T�C�Y
		Vector3 _size ;

		// �f�X�g���N�^
		~GraphicalRegularPolygonPlane(){}
		// ����
		// radius : �����T�C�Y
		// faceNumber : �c���T�C�Y
		// type : �ʐ����^�C�v
		// texture : �e�N�X�`���[
		static RegPolyPlaneSP Create(const int &radius, const int &faceNumber, const FaceGenerateType &type, const CTextureSP &texture = NULL);
	
	private :
		Vector3 _textureSize ;

		// �R���X�g���N�^
		GraphicalRegularPolygonPlane();
		// �����p�`�|���S���̍쐬(���O�p�`�ȏ�)
		// radius : ���a
		// faceNumber : �ʐ�(3�Ő��O�p�`,����������قǉ~�ɋ߂��Ȃ�)
		// texture : �e�N�X�`���[
		bool GenerateRegularPolygonBoard(const int &radius, const int &faceNumber, const FaceGenerateType &type, const CTextureSP &texture = NULL);
	};

}