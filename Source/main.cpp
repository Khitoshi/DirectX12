
#include <memory>
#include <iostream>
#include <Windows.h>
#include "Model.h"

#include "System.h"
#include "Camera.h"
#include "Light.h"

#pragma warning(disable : 4996)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    //�J����
    std::unique_ptr<Camera> camera_3d;
    camera_3d = std::make_unique<Camera>();

    //graphic�G���W��
    //std::unique_ptr<GraphicsEngine> graphicsEngine;
    //graphicsEngine = std::make_unique<GraphicsEngine>();
    // 
    //DirectXTK �G���W��
    //std::unique_ptr<tkEngine> engine;
    //engine = std::make_unique<tkEngine>();
    tkEngine* engine;
    engine = new tkEngine();
    std::unique_ptr<System>system;
    system = std::make_unique<System>();

    system->InitWindow(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nCmdShow,
        TEXT("DirectX12 ���색�C�u����")
        );

    //graphicsEngine = std::make_unique<GraphicsEngine>(system->GetHWnd(), system->GetFrameBufferWidth(), system->GetFrameBufferHeight());

    engine->Init(
        system->GetHWnd(),
        system->GetFrameBufferWidth(),
        system->GetFrameBufferHeight(),
        //*graphicsEngine,
        *camera_3d
    );


    //3D�p�@�J����
    //�J�����̈ʒu��ݒ�
    camera_3d->GetPosition().Set(0.0f, 0.0f, 100.0f);
    //�J�����̃^�[�Q�b�g��ݒ�
    camera_3d->GetTarget().Set(0.0f, 0.0f, 0.0f);



    //���C�g
    std::unique_ptr<Light> light;
    light = std::make_unique<Light>();
    //���C�g�͉E���瓖�����Ă���
    light->GetDirectionLight().direction.Set(1.0f, -1.0f, -1.0f);
    light->GetDirectionLight().direction.Normalize();
    //���C�g�̐F�͔�
    light->GetDirectionLight().color.Set(0.5f, 0.5f, 0.5f);
    //���_�̈ʒu��ݒ肷��
    light->GetEyePosition().Set(camera_3d->GetPosition());

    // ���f��������������
    // ���f�������������邽�߂̏����\�z����
    ModelInitData modelInitData;
    modelInitData.model_File_Path_ = "./Assets/Model/teapot.tkm";

    // �g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
    modelInitData.shader_File_Path = "./Assets/Shader/sample.fx";

    // �f�B���N�V�������C�g�̏���萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^���邽�߂�
    // ���f���̏��������Ƃ��ēn��
    modelInitData.expand_Constant_Buffer = &light;
    modelInitData.expand_Constant_Buffer_Size = sizeof(light);

    Model* model;
    model = new Model();
    model->Init(engine, engine->GetGraphicsEngine(), modelInitData);



    //�ȉ��X�V�R�[�h
    auto& renderContext = engine->GetGraphicsEngine()->GetRenderContext();

    // ��������Q�[�����[�v
    while (system->DispatchWindowMessage())
    {
        //�����_�����O�J�n
        engine->BeginFrame(engine->GetGraphicsEngine(),*camera_3d.get());

        //��������G�������R�[�h���L�q
        //Quaternion qRot;
        //qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
        //qRot.Apply(light.directionLight.direction);
        //qRot.SetRotationDegX(g_pad[0]->GetLStickYF());
        //qRot.Apply(light.directionLight.direction);

        model->Draw(engine->GetGraphicsEngine(), renderContext, *camera_3d);

        engine->EndFrame();
    }
    //delete model;
    return 0;
}
