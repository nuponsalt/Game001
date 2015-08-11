#include "DXUT.h"
#include "Particle.h"

#include "../../../Support/wsb_xml.h"

namespace KMT {

	CShaderParticle::CShaderParticle() : Shader("Resources/HLSL/Particle.xml")
	{ }

	ShaderSP CShaderParticle::CreateShader()
	{
		std::string xmlPath("Resources/HLSL/Particle.xml");

		CWsbXmlSP xml = CWsbXml::LoadXmlFile(xmlPath);
		std::string path = xml->GetElement("path")->GetString();

		std::unordered_map<std::string, ShaderSP>::iterator it = _shaders.find(path);
		ShaderSP shader;
		// 存在したら第二要素を返す
		if(it != _shaders.end())
		{
			shader = (*it).second;
			return shader;
		}
		// 存在しなければ新しくロード
		shader = ShaderSP(new CShaderParticle());
		// ハッシュマップに挿入
		_shaders.insert(std::make_pair(path, shader));
		return shader;
	}

	void CShaderParticle::ApplyEffect(const CMatrix& _rotmtx, const CVector4& _campos)
	{
		return;
	}

}