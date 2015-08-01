#include "DXUT.h"
#include "ShaderToon.h"

#include "../../../Support/wsb_xml.h"
#include "../../Texture.h"
#include "../../Renderer/GraphicBehavior.h"
#include "../../GraphicsManager.h"

namespace KMT {

	CShaderToon::CShaderToon() : CShader("Resource/HLSL/Toon.xml") { }

	CShaderSP CShaderToon::CreateShader()
	{
		std::string _xmlpath("Resource/HLSL/Toon.xml");

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
		std::string path = xml->GetElement("ToonTex")->GetString();
		float dirX = xml->GetElement("LightDirection")->GetElement("X")->GetFloat();
		float dirY = xml->GetElement("LightDirection")->GetElement("Y")->GetFloat();
		float dirZ = xml->GetElement("LightDirection")->GetElement("Z")->GetFloat();

		CShaderToon* tsdr = new CShaderToon();
		tsdr->setLightDirection(dirX, dirY, dirZ);
		tsdr->ToonTexture = CTexture::CreateFromFile(path, D3DX_DEFAULT);

		psdr = CShaderSP(tsdr);

		// ハッシュマップに挿入
		Shaders.insert(std::make_pair(sdr_path, psdr));

		return psdr;
	}

	void CShaderToon::applyEffect(const CMatrix& _rotmtx, const CVector4& _campos)
	{
		// ライト計算用に回転行列を渡す
		pd3dEffect->SetMatrix(*getpHandle("ROT"), &_rotmtx);
		// ライト設定
		pd3dEffect->SetVector(*getpHandle("LightDir"), (D3DXVECTOR4*)&LightDirection);
		// トゥーンテクスチャの設定
		pd3dEffect->SetTexture(*getpHandle("ToonTex"), ToonTexture->getpd3dTexture());		
	}

}