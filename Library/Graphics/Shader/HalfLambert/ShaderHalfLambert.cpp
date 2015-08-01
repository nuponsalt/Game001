#include "DXUT.h"
#include "ShaderHalfLambert.h"

#include "../../GraphicsManager.h"

#include "../../../Support/wsb_xml.h"
#include "../../../Extension.h"
#include "../../../System/Debug.h"
#include "../../Renderer/GraphicBehavior.h"

namespace KMT {

	//------------------------------------------------------------------------------------------------------------------------------------
	// CShaderHalfLambert
	//------------------------------------------------------------------------------------------------------------------------------------
	CShaderHalfLambert::CShaderHalfLambert() : CShader("Resource/HLSL/HalfLambert.xml")
	{}

	CShaderSP CShaderHalfLambert::CreateShader()
	{
		std::string _xmlpath("Resource/HLSL/HalfLambert.xml") ;
		
		CWsbXmlSP xml = CWsbXml::LoadXmlFile(_xmlpath);
		std::string sdr_path = xml->GetElement("path")->GetString();
		CShaderSP psdr ;
		std::unordered_map<std::string, CShaderSP>::iterator it = Shaders.find(sdr_path);
		
		// 存在したら第二要素を返す
		if(it != Shaders.end())
		{
			psdr = (*it).second ;
			return psdr ;
		}
		// 存在しなければ新しくロード
		psdr = CShaderSP(new CShaderHalfLambert());
		
		float dirX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float dirY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float dirZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();
		
		psdr->setLightDirection(dirX, dirY, dirZ);

		// ハッシュマップに挿入
		Shaders.insert(std::make_pair(sdr_path, psdr));

		return psdr;
	}

	void CShaderHalfLambert::applyEffect(const CMatrix &_rotmtx, const CVector4& _campos)
	{
		// ライト計算用に回転行列を渡す
		pd3dEffect->SetMatrix(*getpHandle("ROT"), &_rotmtx);
		// ライト設定(平行光源)
		pd3dEffect->SetVector(*getpHandle("LightDir"), (D3DXVECTOR4*)&LightDirection);
	}

}