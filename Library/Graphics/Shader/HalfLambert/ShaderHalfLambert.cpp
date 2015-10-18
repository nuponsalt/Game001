#include "DXUT.h"
#include "ShaderHalfLambert.h"

#include "../../GraphicsManager.h"

#include "../../../Support/wsb_xml.h"
#include "../../../Extension.h"
#include "../../../System/Debug.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	//------------------------------------------------------------------------------------------------------------------------------------
	// ShaderHalfLambert
	//------------------------------------------------------------------------------------------------------------------------------------
	ShaderHalfLambert::ShaderHalfLambert() : Shader("Resources/HLSL/HalfLambert.xml")
	{}

	ShaderSP ShaderHalfLambert::Create()
	{
		std::string xmlPath("Resources/HLSL/HalfLambert.xml") ;
		
		CWsbXmlSP xml = CWsbXml::LoadXmlFile(xmlPath);
		std::string path = xml->GetElement("path")->GetString();
		ShaderSP shader ;
		std::unordered_map<std::string, ShaderSP>::iterator it = _shaders.find(path);
		
		// ���݂�������v�f��Ԃ�
		if(it != _shaders.end())
		{
			shader = (*it).second ;
			return shader ;
		}
		// ���݂��Ȃ���ΐV�������[�h
		shader = ShaderSP(new ShaderHalfLambert());
		
		float directionX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float directionY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float directionZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();
		
		shader->SetLightDirection(directionX, directionY, directionZ);

		// �n�b�V���}�b�v�ɑ}��
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderHalfLambert::ApplyEffect(const CMatrix &rotation, const Vector4& cameraPosition)
	{
		// ���C�g�v�Z�p�ɉ�]�s���n��
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// ���C�g�ݒ�(���s����)
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
	}

}