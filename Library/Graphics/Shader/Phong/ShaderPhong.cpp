#include "DXUT.h"
#include "ShaderPhong.h"

#include "../../../Support/wsb_xml.h"
#include "../../GraphicsManager.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	ShaderPhong::ShaderPhong() : Shader("Resources/HLSL/Phong.xml") { }

	ShaderSP ShaderPhong::Create()
	{
		std::string xmlPath("Resources/HLSL/Phong.xml");

		CWsbXmlSP xml = CWsbXml::LoadXmlFile(xmlPath);
		std::string path = xml->GetElement("path")->GetString();
		ShaderSP shader;
		std::unordered_map<std::string, ShaderSP>::iterator it = _shaders.find(path);

		// ���݂�������v�f��Ԃ�
		if(it != _shaders.end())
		{
			shader = (*it).second;
			return shader;
		}
		// ���݂��Ȃ���ΐV��������
		shader = ShaderSP(new ShaderPhong());

		float directionX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float directionY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float directionZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();
		float ambient = xml->GetElement("Ambient")->GetFloat();
		float specular = xml->GetElement("Specular")->GetFloat();
		float specularpow = xml->GetElement("SpecularPower")->GetFloat();

		shader->SetAmbient(ambient);
		shader->SetLightDirection(directionX, directionY, directionZ);
		shader->SetSpecular(specular);
		shader->SetSpecularPower(specularpow);

		// �n�b�V���}�b�v�ɑ}��
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderPhong::ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition)
	{
		// ���C�g�v�Z�p�ɉ�]�s���n��
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// �����ݒ�
		_effect->SetVector(*GetHandle("Ambient"), &D3DXVECTOR4(_ambient, _ambient, _ambient, 1.0f));
		// �X�y�L�����[�͈͐ݒ�
		_effect->SetFloat(*GetHandle("Specular"), _specular);
		// �X�y�L�����[���x�ݒ�
		_effect->SetFloat(*GetHandle("SpecularPower"), _specularPower);
		// �J�����ݒ�
		_effect->SetVector(*GetHandle("EyePos"), (D3DXVECTOR4*)&cameraPosition);
		// ���C�g�ݒ�(���s����)
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
	}

}