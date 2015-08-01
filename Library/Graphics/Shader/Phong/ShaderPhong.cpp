#include "DXUT.h"
#include "ShaderPhong.h"

#include "../../../Support/wsb_xml.h"
#include "../../GraphicsManager.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	CShaderPhong::CShaderPhong() : CShader("Resource/HLSL/Phong.xml") { }

	CShaderSP CShaderPhong::CreateShader()
	{
		std::string _xmlpath("Resource/HLSL/Phong.xml");

		CWsbXmlSP xml = CWsbXml::LoadXmlFile(_xmlpath);
		std::string sdr_path = xml->GetElement("path")->GetString();
		CShaderSP psdr;
		std::unordered_map<std::string, CShaderSP>::iterator it = Shaders.find(sdr_path);

		// 存在したら第二要素を返す
		if(it != Shaders.end())
		{
			psdr = (*it).second;
			return psdr;
		}
		// 存在しなければ新しく生成
		psdr = CShaderSP(new CShaderPhong());

		float dirX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float dirY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float dirZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();
		float ambient = xml->GetElement("Ambient")->GetFloat();
		float specular = xml->GetElement("Specular")->GetFloat();
		float specularpow = xml->GetElement("SpecularPower")->GetFloat();

		psdr->setAmbient(ambient);
		psdr->setLightDirection(dirX, dirY, dirZ);
		psdr->setSpecular(specular);
		psdr->setSpecularPower(specularpow);

		// ハッシュマップに挿入
		Shaders.insert(std::make_pair(sdr_path, psdr));

		return psdr;
	}

	void CShaderPhong::applyEffect(const CMatrix& _rotmtx, const CVector4& _campos)
	{
		// ライト計算用に回転行列を渡す
		pd3dEffect->SetMatrix(*getpHandle("ROT"), &_rotmtx);
		// 環境光設定
		pd3dEffect->SetVector(*getpHandle("Ambient"), &D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f));
		// スペキュラー範囲設定
		pd3dEffect->SetFloat(*getpHandle("Specular"), Specular);
		// スペキュラー強度設定
		pd3dEffect->SetFloat(*getpHandle("SpecularPower"), SpecularPower);
		// カメラ設定
		pd3dEffect->SetVector(*getpHandle("EyePos"), (D3DXVECTOR4*)&_campos);
		// ライト設定(平行光源)
		pd3dEffect->SetVector(*getpHandle("LightDir"), (D3DXVECTOR4*)&LightDirection);
	}

}