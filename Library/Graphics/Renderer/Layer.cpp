#include "DXUT.h"
#include "Layer.h"
#include "../GraphicsManager.h"

#include "GraphicalShape/GraphicalPlane.h"

namespace KMT {

	Layer::Layer() : _textureSurface(NULL), _depthSurface(NULL) { }

	Layer::~Layer()
	{
		// �T�[�t�F�C�X�̊J��
		SAFE_RELEASE(_textureSurface);
		// �X�e���V���p�b�t�@
		SAFE_RELEASE(_depthSurface);
		// �|���̉��
		_board.reset();
		// �J�����̉��
		_camera3D.reset();
		_camera2D.reset();

		// �����_�����O���X�g�����
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			_renderLists[i]->clear();
		}
		for(size_t i = 0; i < _renderLists.size(); i++)
		{
			SAFE_DELETE(_renderLists[i]);
		}
		_renderLists.clear();
	}

	LayerSP Layer::Create(size_t width, size_t height)
	{
		LayerSP object = LayerSP(new Layer);

		// �e�N�X�`���̐���
		object->_texture = CTexture::CreateEmpty(width, height);

		// �T�[�t�F�C�X�̎擾
		if (FAILED(object->_texture->getpd3dTexture()->GetSurfaceLevel(0, &object->_textureSurface)))
		{
			// �T�[�t�F�C�X�擾���s
			// �G���[�ɑΉ�����R�[�h
		}

		// �X�e���V���o�b�t�@�̍쐬
		if (FAILED(GraphicsManager::_device->CreateDepthStencilSurface( 
			object->_texture->getd3dImageInfo().Width,
			object->_texture->getd3dImageInfo().Height,
			D3DFMT_D16, 
			D3DMULTISAMPLE_NONE, 
			0, 
			TRUE, 
			&object->_depthSurface, 
			NULL)))
		{
			// �X�e���V���o�b�t�@�̍쐬�Ɏ��s
			// �G���[�ɑΉ�����R�[�h�������ɏ���
		}

		object->_camera3D = CCamera::Create();
		object->_camera3D->setEye(0, 0, -10.0f);

		object->_camera2D = CCamera::Create();
		float angle = object->_camera2D->getAngle();
		object->_camera2D->setEye(0, 0, -(SCREEN_HEIGHT / (2.0f * tan(angle / 2.0f))));
		// �|������
		object->_board = GraphicalPlane::Create(width, height);
		float aspect = (float)width / height;
		object->_camera3D->setAspect(aspect);
		object->_camera2D->setAspect(aspect);
		// �|���Ƀe�N�X�`�����Z�b�g
		if(object->_board->GetTextures().size() < 1)
			object->_board->SetTexture(object->_texture);
		// �����_�����O���X�g�̍쐬
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			std::list<GraphicBehaviorWP> *list = new std::list<GraphicBehaviorWP>();
			object->_renderLists.push_back(list);
		}

		return object;
	}

	void Layer::Render()
	{
		// �X�e���V���o�b�t�@���Z�b�g
		GraphicsManager::_device->SetDepthStencilSurface(_depthSurface);

		// �����_�����O�^�[�Q�b�g���Z�b�g
		GraphicsManager::_device->SetRenderTarget(0, _textureSurface);

		// �T�[�t�F�C�X���N���A
		GraphicsManager::_device->Clear(0,
			NULL, 
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 100, 149, 237),
			1.0f,
			0);

		// �J�����̍X�V
		_camera3D->updateView();
		_camera2D->updateView();

		//std::unordered_map<size_t, std::list<GraphicBehaviorWP>*>::iterator mapIterator;

		// 2D�\�[�g
		_renderLists[RENDER_BACK2D]->sort(GraphicBehavior::Compare2D);
		_renderLists[RENDER_FRONT2D]->sort(GraphicBehavior::Compare2D);

		// ���X�g�A�N�Z�X�p�C�e���[�^
		std::list<GraphicBehaviorWP>::iterator listIterator;
		// 3D�\�[�g
		for(int i = 0; i < 2; i++)
		{
			listIterator = _renderLists[RENDER_NORMAL + i]->begin();

			while(listIterator != _renderLists[RENDER_NORMAL + i]->end())
			{
				// ���łɃf���[�g����Ă����ꍇ���X�g���珜�O���ăX�L�b�v
				if((*listIterator).lock() == NULL)
				{
					listIterator = _renderLists[RENDER_NORMAL + i]->erase(listIterator);
					continue;
				}

				// �J��������̋������v�Z
				Vector3 cameraPosition = _camera3D->getEye();
				Vector3 length = (*listIterator).lock()->Position - cameraPosition;
				(*listIterator).lock()->_cameraDistance = length.Length();

				++listIterator;
			}
		}

		// �\�[�g
		_renderLists[RENDER_NORMAL]->sort(GraphicBehavior::Compare);
		_renderLists[RENDER_ALPHA]->sort(GraphicBehavior::CompareBack);

		// �����_�����O�[���X�g�̒��g�����ׂĕ`��
		for(int i = RENDER_BACK2D; i < RENDER_STATE_MAX; i++)
		{
			if(i != RENDER_ALPHA)
			{
				// Z�o�b�t�@�̃N���A
				GraphicsManager::_device->Clear(0, 
					NULL, 
					D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB(255, 0, 191, 255),
					1.0f,
					0);
			}

			// Z�o�b�t�@�؂�ւ�
			(i == RENDER_NORMAL || i == RENDER_ALPHA) 
				? GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, TRUE)
				: GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, FALSE);

			listIterator = _renderLists[i]->begin();
			
			while(listIterator != _renderLists[i]->end())
			{
				// ���łɃf���[�g����Ă����ꍇ���X�g���珜�O���ăX�L�b�v
				if((*listIterator).lock() == NULL)
				{
					listIterator = _renderLists[i]->erase(listIterator);
					continue ;
				}
				// �J�����I��
				CCameraSP camera = (i == RENDER_BACK2D || i == RENDER_FRONT2D) ? _camera2D : _camera3D;
				// �`��
				(*listIterator).lock()->Render(camera.get());
				// ���̃C�e���[�^��
				listIterator ++;
			}
		}
	}
}