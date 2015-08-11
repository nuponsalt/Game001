#include "DXUT.h"
#include "Shader.h"
#include "../GraphicsManager.h"

#include "../../Support/wsb_xml.h"
#include "../../Extension.h"
#include "../../System/Debug.h"
#include "../Renderer/GraphicBehavior.h"

namespace KMT {

	// static Object
	std::unordered_map<std::string, ShaderSP>	Shader::_shaders;
	
	//-----------------------------------------------------------------------------------------------------------
	// Shader
	//-----------------------------------------------------------------------------------------------------------

	Shader::Shader() : _ambient(0), _specular(0), _specularPower(0), _type(0) { }
	
	Shader::Shader(std::string xmlPath)
	{	
		CWsbXmlSP xml = CWsbXml::LoadXmlFile(xmlPath);
		std::string path = xml->GetElement("path")->GetString();

		_path = path;

		WCHAR bufferName[256] = {0};
		DXconvAnsiToWide(bufferName, path.c_str(), (path.length()+1));
		
		// シェーダ ファイルのロード
		LPD3DXBUFFER compilationErrors = NULL;

		HRESULT hr = D3DXCreateEffectFromFile( 
					CGraphicsManager::pd3dDevice, 
					bufferName, 
					NULL, 
					NULL, 
					D3DXSHADER_DEBUG, 
					NULL, 
					&_effect, 
					&compilationErrors
					);

		// ロードに失敗した場合
		if(FAILED(hr))
		{
			if(compilationErrors)	
				DXUTOutputDebugStringA((LPCSTR)compilationErrors->GetBufferPointer());
			
			SAFE_RELEASE(compilationErrors);
			return;
		}
		// ハンドルの読み込み
		_technique = _effect->GetTechniqueByName(xml->GetElement("technique")->GetString().c_str());
		_wvp = _effect->GetParameterByName(NULL, xml->GetElement("wvp")->GetString().c_str());
		_color = _effect->GetParameterByName(NULL, xml->GetElement("color")->GetString().c_str());
		_sourceTexture = _effect->GetParameterByName(NULL, xml->GetElement("srctex")->GetString().c_str());

		// 他、ロード
		for (int i = 0; i < xml->GetElementNum("handle"); i++)
		{
			// XMLからKeyを読み込みロードする
			D3DXHANDLE shaderTemporary = _effect->GetParameterByName(NULL, xml->GetElement("handle", i)->GetString().c_str());
			_handles.insert(std::make_pair(xml->GetElement("handle", i)->GetString(), shaderTemporary));
		}
	}

	Shader::~Shader()
	{
		SAFE_RELEASE(_effect);
	}
	
	void Shader::Destroy(std::string _name)
	{
		// イテレータを用意
		// イテレータをハッシュマップの先頭にセット
		std::unordered_map< std::string, ShaderSP >::iterator it = _shaders.begin();

		// 全部解放
		if(_name == "all")
		{
			while(it != _shaders.end())
			{
				(*it).second.reset();
				_shaders.erase(it++);
			}
		}
		// テクスチャを１つだけ解放
		else
		{
			it = _shaders.find(_name);
			if(it != _shaders.end())
			{
				(*it).second.reset();
				_shaders.erase(it);
			}
		}
	}

	void Shader::SetTechnique() const
	{
		_effect->SetTechnique(_technique);
	}

	void Shader::SetWVPMatrix(const CMatrix& wvp) const
	{
		_effect->SetMatrix(_wvp, (D3DXMATRIX*)&wvp);
	}

	void Shader::SetColor(const CVector4& color) const
	{
		_effect->SetVector(_color, (D3DXVECTOR4*)&color);
	}

	void Shader::SetTexture(const LPDIRECT3DTEXTURE9 texture) const
	{
		_effect->SetTexture(_sourceTexture, texture);
	}

	void Shader::BeginShader() const
	{
		_effect->Begin(NULL, 0);
	}

	void Shader::EndShader() const
	{
		_effect->End();
	}

	void Shader::BeginPass(const bool addsBlend)
	{
		_effect->BeginPass(addsBlend);
	}

	void Shader::EndPass()
	{
		_effect->EndPass();
	}

	D3DXHANDLE* Shader::GetHandle(const std::string &path) 
	{
		// イテレータを用意
		std::unordered_map<std::string, D3DXHANDLE>::iterator it = _handles.find(path);

		if(it != _handles.end())
			return &(*it).second;
		// 見つからない場合(ここに来た場合エラー発生)
		return NULL;
	}

	//-----------------------------------------------------------------------------------------------------------
	// CShaderNormal
	//-----------------------------------------------------------------------------------------------------------
	ShaderNormal::ShaderNormal() : Shader("Resources/HLSL/Normal.xml")
	{ }

	ShaderSP ShaderNormal::CreateShader()
	{
		std::string _xmlpath("Resources/HLSL/Normal.xml");
		
		CWsbXmlSP xml = CWsbXml::LoadXmlFile(_xmlpath);
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
		shader = ShaderSP(new ShaderNormal());
		// ハッシュマップに挿入
		_shaders.insert(std::make_pair(path, shader));
		return shader;
	}

	void ShaderNormal::ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition)
	{
		return;
	}

}
