#include "DXUT.h"
#include "ShaderLambert.h"
#include "../../GraphicsManager.h"

#include "../../../Support/wsb_xml.h"
#include "../../../Extension.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	//------------------------------------------------------------------------------------------------------------------------------------
	// ShaderLambert
	//------------------------------------------------------------------------------------------------------------------------------------
	ShaderLambert::ShaderLambert() : Shader("Resources/HLSL/Lambert.xml") { }

	ShaderSP ShaderLambert::Create()
	{
		std::string xmlPath("Resources/HLSL/Lambert.xml");

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
		// ���݂��Ȃ���ΐV�������[�h
		shader = ShaderSP(new ShaderLambert());

		float directionX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float directionY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float directionZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();

		float ambient = xml->GetElement("Ambient")->GetFloat();

		shader->SetAmbient(ambient);
		shader->SetLightDirection(directionX, directionY, directionZ);
		shader->SetFogColor(Vector4(
			xml->GetElement("Fog")->GetElement("Color")->GetElement("R")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Color")->GetElement("G")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Color")->GetElement("B")->GetFloat(),
			1));
		shader->SetFogRange(
			xml->GetElement("Fog")->GetElement("Param1")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Param2")->GetFloat()
			);

		// �n�b�V���}�b�v�ɑ}��
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderLambert::ApplyEffect(const CMatrix &rotation, const Vector4& cameraPosition)
	{
		// ���C�g�v�Z�p�ɉ�]�s���n��
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// �����ݒ�
		_effect->SetVector(*GetHandle("Ambient"), &D3DXVECTOR4(_ambient, _ambient, _ambient, 1.0f));
		// ���C�g�ݒ�(���s����)
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
		// �t�H�O�ݒ�
		_effect->SetVector(*GetHandle("fogColor"), (D3DXVECTOR4*)&_fogColor);
		_effect->SetVector(*GetHandle("fogCoord"), (D3DXVECTOR4*)&_fogRange);
	}

}