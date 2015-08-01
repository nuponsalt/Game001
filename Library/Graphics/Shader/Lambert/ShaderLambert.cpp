#include "DXUT.h"
#include "ShaderLambert.h"
#include "../../GraphicsManager.h"

#include "../../../Support/wsb_xml.h"
#include "../../../Extension.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	//------------------------------------------------------------------------------------------------------------------------------------
	// CShaderLambert
	//------------------------------------------------------------------------------------------------------------------------------------
	CShaderLambert::CShaderLambert() : CShader("Resource/HLSL/Lambert.xml") { }

	CShaderSP CShaderLambert::CreateShader()
	{
		std::string _xmlpath("Resource/HLSL/Lambert.xml");

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
		// 存在しなければ新しくロード
		psdr = CShaderSP(new CShaderLambert());

		float dirX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float dirY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float dirZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();

		float ambient = xml->GetElement("Ambient")->GetFloat();

		psdr->setAmbient(ambient);
		psdr->setLightDirection(dirX, dirY, dirZ);
		psdr->setFogColor(CVector4(
			xml->GetElement("Fog")->GetElement("Color")->GetElement("R")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Color")->GetElement("G")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Color")->GetElement("B")->GetFloat(),
			1));
		psdr->setfogCoord(
			xml->GetElement("Fog")->GetElement("Param1")->GetFloat(), 
			xml->GetElement("Fog")->GetElement("Param2")->GetFloat()
			);

		// ハッシュマップに挿入
		Shaders.insert(std::make_pair(sdr_path, psdr));

		return psdr;
	}

	void CShaderLambert::applyEffect(const CMatrix &_rotmtx, const CVector4& _campos)
	{
		// ライト計算用に回転行列を渡す
		pd3dEffect->SetMatrix(*getpHandle("ROT"), &_rotmtx);
		// 環境光設定
		pd3dEffect->SetVector(*getpHandle("Ambient"), &D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f));
		// ライト設定(平行光源)
		pd3dEffect->SetVector(*getpHandle("LightDir"), (D3DXVECTOR4*)&LightDirection);
		// フォグ設定
		pd3dEffect->SetVector(*getpHandle("fogColor"), (D3DXVECTOR4*)&fogColor);
		pd3dEffect->SetVector(*getpHandle("fogCoord"), (D3DXVECTOR4*)&fogCoord);

	}

}