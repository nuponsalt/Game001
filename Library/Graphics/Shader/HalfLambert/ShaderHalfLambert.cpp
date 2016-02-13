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
		
		// 存在したら第二要素を返す
		if(it != _shaders.end())
		{
			shader = (*it).second ;
			return shader ;
		}
		// 存在しなければ新しくロード
		shader = ShaderSP(new ShaderHalfLambert());
		
		float directionX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float directionY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float directionZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();
		
		shader->SetLightDirection(directionX, directionY, directionZ);

		// ハッシュマップに挿入
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderHalfLambert::ApplyEffect(const Matrix &rotation, const Vector4& cameraPosition)
	{
		// ライト計算用に回転行列を渡す
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// ライト設定(平行光源)
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
	}

}