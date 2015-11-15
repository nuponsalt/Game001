//*********************************************************************
// �V�F�[�_�[�N���X
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once 

#pragma warning( disable:4238 )

#include <memory>
#include <string>
#include <unordered_map>

#include "../../Math/Vector.h"

namespace KMT {

	class GraphicBehavior;

	class Shader;
	typedef std::shared_ptr<Shader> ShaderSP;
	typedef std::weak_ptr<Shader> ShaderWP;

	class Shader
	{
	public :
		enum BEGINorEND
		{
			BEGIN = true,
			END = false
		};

		// �t�H�O�̎��
		enum FogType
		{
			LINEAR_DISTANCE_FOG,
			EXPONENTIAL_DISTANCE_FOG,
			HEIGHT_FOG
		};

		// �f�X�g���N�^
		~Shader();
		
		// �j��
		static void Destroy(std::string name = "all");

		//* �擾 *//
		// �G�t�F�N�g
		inline LPD3DXEFFECT GetEffect() { return _effect; }
		// �n���h��
		inline D3DXHANDLE* GetWVP() { return &_wvp; }	// ���[���h * �r���[ * �v���W�F�N�V����
		D3DXHANDLE* GetHandle(const std::string &path);	// �w�肵�Ď擾
		
		//* �V�F�[�_�[�ݒ� *//
		// �A�N�e�B�u�ȃe�N�j�b�N���V�F�[�_�[�ɐݒ�
		void SetTechnique() const;
		// ���[���h * �r���[ * �v���W�F�N�V���� �s����V�F�[�_�[�ɐݒ�
		void SetWVPMatrix(const CMatrix& wvp) const;
		// �J���[���V�F�[�_�[�ɐݒ�
		void SetColor(const Vector4& color) const;
		// �e�N�X�`�����V�F�[�_�[�ɐݒ�
		void SetTexture(const LPDIRECT3DTEXTURE9 texture) const;
		// �V�F�[�_�[�̎g�p�J�n
		void BeginShader() const;
		// �V�F�[�_�[�̎g�p�I��
		void EndShader() const;
		// �p�X�̎g�p
		void BeginPass(const bool addsBlend);
		// �p�X�̏I��
		void EndPass();
		// ���L�̃V�F�[�_�[�ݒ�(�������z�֐�)
		// rotation : ��]�s��
		// cameraPosition : �J�����̎��_�̍��W
		virtual void ApplyEffect(const CMatrix &rotation, const Vector4& cameraPosition) = 0;

		//* �p�����[�^�[�ݒ� *//
		// ���C�g�̍��W
		inline void SetLightDirection(Vector4 direction) { _lightDirection = direction; }
		inline void SetLightDirection(const float x, const float y, const float z) { _lightDirection = Vector4(x, y, z, 0.0f); }
		// ����
		inline void SetAmbient(const float ambient) { _ambient = ambient; }
		// �X�y�L�����[�͈�
		inline void SetSpecular(const float specular) { _specular = specular; }
		// �X�y�L�����[���x
		inline void SetSpecularPower(const float specularPower) { _specularPower = specularPower; }
		// �t�H�O�J���[
		inline void SetFogColor(const Vector4& fogColor) { _fogColor = fogColor; }
		// �t�H�O�̃p�����[�^�[�̐ݒ�
		inline void SetFogRange(const float param1, const float param2) { _fogRange = Vector4(param1, param2, 0, 0); }

	protected :
		// �g�p����V�F�[�_�[�t�@�C����
		std::string	_path;
		// ����
		float _ambient;
		// ���s����
		Vector4 _lightDirection;
		// �X�y�L�����[�͈̔�
		float _specular;
		// �X�y�L�����[�̋��x
		float _specularPower;
		// �t�H�O�J���[
		Vector4 _fogColor;
		// �t�H�O�͈̔�
		Vector4 _fogRange;

		// �V�F�[�_�[�̃^�C�v�̗񋓌^
		enum ShaderType_
		{
			SHADER_NORMAL,
			SHADER_LAMBERT,
			SHADER_HLAMBERT,
			SHADER_PHONG,
			SHADER_TOON,
			SHADER_TYPEMAX
		};
		// �V�F�[�_�[�̃^�C�v
		size_t _type;

		// �G�t�F�N�g�̐ݒ�Ɩ₢���킹�A����уe�N�j�b�N�̑I��
		// ��������|�C���^
		// �V�F�[�_�{��
		LPD3DXEFFECT _effect;
		// �e�N�j�b�N
		D3DXHANDLE _technique;
		// ���[���h x �r���[ x �v���W�F�N�V����
		D3DXHANDLE _wvp;
		// �f�B�t���[�Y�F
		D3DXHANDLE _color;
		// �e�N�X�`��
		D3DXHANDLE _sourceTexture;
		// �V�F�[�_�[�ɓn���n���h����o�^�����n�b�V���}�b�v
		std::unordered_map<std::string, D3DXHANDLE> _handles;
		// �V�F�[�_�[�Ǘ��n�b�V���}�b�v�z��
		static std::unordered_map<std::string, ShaderSP> _shaders;

		// �R���X�g���N�^
		Shader();
		Shader(std::string xmlPath);

	};
	
	class ShaderNormal;
	typedef std::shared_ptr<ShaderNormal> ShaderNormalSP;
	typedef std::weak_ptr<ShaderNormal> ShaderNormalWP;

	class ShaderNormal : public Shader
	{
	public :
		// �f�X�g���N�^
		~ShaderNormal() { }
		// �V�F�[�_�[����
		static ShaderSP Create();
		// ���f���ɑ΂���G�t�F�N�g�K�p
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// �R���X�g���N�^
		ShaderNormal();

	};

}