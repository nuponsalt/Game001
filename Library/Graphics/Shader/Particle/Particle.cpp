#include "DXUT.h"
#include "Particle.h"

#include "../../../Support/wsb_xml.h"

namespace KMT {

	CShaderParticle::CShaderParticle() : CShader("Resource/HLSL/Particle.xml")
	{ }

	CShaderSP CShaderParticle::CreateShader()
	{
		std::string _xmlpath("Resource/HLSL/Particle.xml");

		CWsbXmlSP xml = CWsbXml::LoadXmlFile(_xmlpath);
		std::string sdr_path = xml->GetElement("path")->GetString();

		std::unordered_map<std::string, CShaderSP>::iterator it = Shaders.find(sdr_path);
		CShaderSP psdr;
		// 存在したら第二要素を返す
		if(it != Shaders.end())
		{
			psdr = (*it).second;
			return psdr;
		}
		// 存在しなければ新しくロード
		psdr = CShaderSP(new CShaderParticle());
		// ハッシュマップに挿入
		Shaders.insert(std::make_pair(sdr_path, psdr));
		return psdr;
	}

	void CShaderParticle::applyEffect(const CMatrix& _rotmtx, const CVector4& _campos)
	{
		return;
	}

}