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
		
		// �V�F�[�_ �t�@�C���̃��[�h
		LPD3DXBUFFER compilationErrors = NULL;

		HRESULT hr = D3DXCreateEffectFromFile( 
					GraphicsManager::_device, 
					bufferName, 
					NULL, 
					NULL, 
					D3DXSHADER_DEBUG, 
					NULL, 
					&_effect, 
					&compilationErrors
					);

		// ���[�h�Ɏ��s�����ꍇ
		if(FAILED(hr))
		{
			if(compilationErrors)	
				DXUTOutputDebugStringA((LPCSTR)compilationErrors->GetBufferPointer());
			
			SAFE_RELEASE(compilationErrors);
			return;
		}
		// �n���h���̓ǂݍ���
		_technique = _effect->GetTechniqueByName(xml->GetElement("technique")->GetString().c_str());
		_wvp = _effect->GetParameterByName(NULL, xml->GetElement("wvp")->GetString().c_str());
		_color = _effect->GetParameterByName(NULL, xml->GetElement("color")->GetString().c_str());
		_sourceTexture = _effect->GetParameterByName(NULL, xml->GetElement("srctex")->GetString().c_str());

		// ���A���[�h
		for (int i = 0; i < xml->GetElementNum("handle"); i++)
		{
			// XML����Key��ǂݍ��݃��[�h����
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
		// �C�e���[�^��p��
		// �C�e���[�^���n�b�V���}�b�v�̐擪�ɃZ�b�g
		std::unordered_map< std::string, ShaderSP >::iterator it = _shaders.begin();

		// �S�����
		if(_name == "all")
		{
			while(it != _shaders.end())
			{
				(*it).second.reset();
				_shaders.erase(it++);
			}
		}
		// �e�N�X�`�����P�������
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

	void Shader::SetColor(const Vector4& color) const
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
		// �C�e���[�^��p��
		std::unordered_map<std::string, D3DXHANDLE>::iterator it = _handles.find(path);

		if(it != _handles.end())
			return &(*it).second;
		// ������Ȃ��ꍇ(�����ɗ����ꍇ�G���[����)
		return NULL;
	}

	//-----------------------------------------------------------------------------------------------------------
	// CShaderNormal
	//-----------------------------------------------------------------------------------------------------------
	ShaderNormal::ShaderNormal() : Shader("Resources/HLSL/Normal.xml")
	{ }

	ShaderSP ShaderNormal::Create()
	{
		std::string _xmlpath("Resources/HLSL/Normal.xml");
		
		CWsbXmlSP xml = CWsbXml::LoadXmlFile(_xmlpath);
		std::string path = xml->GetElement("path")->GetString();
		
		std::unordered_map<std::string, ShaderSP>::iterator it = _shaders.find(path);
		ShaderSP shader;
		// ���݂�������v�f��Ԃ�
		if(it != _shaders.end())
		{
			shader = (*it).second;
			return shader;
		}
		// ���݂��Ȃ���ΐV�������[�h
		shader = ShaderSP(new ShaderNormal());
		// �n�b�V���}�b�v�ɑ}��
		_shaders.insert(std::make_pair(path, shader));
		return shader;
	}

	void ShaderNormal::ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition)
	{
		return;
	}

}
