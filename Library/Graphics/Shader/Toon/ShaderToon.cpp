#include "DXUT.h"
#include "ShaderToon.h"

#include "../../../Support/wsb_xml.h"
#include "../../Texture.h"
#include "../../Renderer/GraphicBehavior.h"
#include "../../GraphicsManager.h"

namespace KMT {

	ShaderToon::ShaderToon() : Shader("Resources/HLSL/Toon.xml") { }

	ShaderSP ShaderToon::Create()
	{
		std::string xmlPath("Resources/HLSL/Toon.xml");

		CWsbXmlSP xml = CWsbXml::LoadXmlFile(xmlPath);
		std::string path = xml->GetElement("path")->GetString();
		ShaderSP shader;
		std::unordered_map<std::string, ShaderSP>::iterator it = _shaders.find(path);

		// 存在したら第二要素を返す
		if(it != _shaders.end())
		{
			shader = (*it).second;
			return shader;
		}
		// 存在しなければ新しく生成
		std::string texturePath = xml->GetElement("ToonTex")->GetString();
		float directionX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float directionY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float directionZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();

		auto shaderToon = new ShaderToon();
		shaderToon->SetLightDirection(directionX, directionY, directionZ);
		shaderToon->_toonTexture = CTexture::CreateFromFile(texturePath, D3DX_DEFAULT);

		shader = ShaderSP(shaderToon);

		// ハッシュマップに挿入
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderToon::ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition)
	{
		// ライト計算用に回転行列を渡す
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// ライト設定
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
		// トゥーンテクスチャの設定
		_effect->SetTexture(*GetHandle("ToonTex"), _toonTexture->getpd3dTexture());		
	}

}