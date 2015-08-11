#include "DXUT.h"
#include "ShaderPhong.h"

#include "../../../Support/wsb_xml.h"
#include "../../GraphicsManager.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	ShaderPhong::ShaderPhong() : Shader("Resources/HLSL/Phong.xml") { }

	ShaderSP ShaderPhong::CreateShader()
	{
		std::string xmlPath("Resources/HLSL/Phong.xml");

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

		// ハッシュマップに挿入
		_shaders.insert(std::make_pair(path, shader));

		return shader;
	}

	void ShaderPhong::ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition)
	{
		// ライト計算用に回転行列を渡す
		_effect->SetMatrix(*GetHandle("ROT"), &rotation);
		// 環境光設定
		_effect->SetVector(*GetHandle("Ambient"), &D3DXVECTOR4(_ambient, _ambient, _ambient, 1.0f));
		// スペキュラー範囲設定
		_effect->SetFloat(*GetHandle("Specular"), _specular);
		// スペキュラー強度設定
		_effect->SetFloat(*GetHandle("SpecularPower"), _specularPower);
		// カメラ設定
		_effect->SetVector(*GetHandle("EyePos"), (D3DXVECTOR4*)&cameraPosition);
		// ライト設定(平行光源)
		_effect->SetVector(*GetHandle("LightDir"), (D3DXVECTOR4*)&_lightDirection);
	}

}