#pragma once

#pragma warning(disable : 4482)

#include "../../Math/Transformation.h"
#include "../Material.h"
#include "../Mesh.h"
#include "../Texture.h"

#include <vector>
#include <memory>

namespace KMT {

	//* �O���錾 *//
	class CCamera;
	class Shader;
	typedef std::shared_ptr<Shader> ShaderSP;

	class GraphicBehavior;
	typedef std::shared_ptr<GraphicBehavior> GraphicBehaviorSP;
	typedef std::weak_ptr<GraphicBehavior> GraphicBehaviorWP;

	class GraphicBehavior : public Transformation, public CMaterial
	{
	public :
		// �\�[�g�p�̒l
		float _cameraDistance;
		// �`��t���O
		bool _renders;
		// �r���{�[�h�t���O
		bool _isBillBoard;

		// �R���X�g���N�^
		GraphicBehavior() : _cameraDistance(0), _renders(true), _isBillBoard(false), _cullingState(D3DCULL_CCW){ }
		// �f�X�g���N�^
		virtual ~GraphicBehavior() { }

		//* �擾 *//
		// ���b�V��
		inline CMeshSP GetMesh() { return _mesh; }
		// �e�N�X�`���x�N�^�[�z��
		inline std::vector<CTextureSP> GetTextures() { return _textures; }
		// �e�N�X�`���x�N�^�[�z��(���ڃC���f�b�N�X�w��)
		inline CTextureSP GetTexturesElement(size_t index) { return _textures[index]; }
		// �g�U���ːF(�f�B�t���[�Y�F)
		inline Vector4 GetDiffuseColors(size_t index) { return _diffuseColors[index]; }

		//* �ݒ� *//
		// �V�F�[�_�[
		inline void SetShader(ShaderSP shader) { _shader = shader; }
		// �J�����O���[�h
		inline void SetCullingState(const _D3DCULL cullingState) { _cullingState = cullingState; }
		// �e�N�X�`��
		inline void SetTexture(const CTextureSP& texture) { _textures.push_back(texture); }
		
		//* std::list��sort�g�p�֐� *
		static inline bool Compare(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->_cameraDistance < riLeft.lock()->_cameraDistance);
		}
		static inline bool CompareBack(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->_cameraDistance < riLeft.lock()->_cameraDistance);
		}
		static inline bool Compare2D(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->Position.z > riLeft.lock()->Position.z);
		}

		// �`��
		virtual void Render(const CCamera* camera) = 0;

	private :
	protected :
		// �t�@�C���p�X
		std::string _path;
		// ���b�V��
		CMeshSP _mesh;
		// �V�F�[�_�[
		ShaderSP _shader;
		// �`�掞�̃J�����O���[�h�ۑ��ϐ�
		_D3DCULL _cullingState;
		
		// �e�N�X�`���[�z��(std::vector)
		std::vector<CTextureSP> _textures;
		// ���_�J���[�z��(std::vector)
		std::vector<Vector4> _diffuseColors;

	};

}