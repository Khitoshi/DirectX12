
#include <memory>
#include <iostream>
#include <Windows.h>

#include "System.h"
#include "Camera.h"
#include "Light.h"

#pragma warning(disable : 4996)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    std::unique_ptr<Camera> camera_3d;
    camera_3d = std::make_unique<Camera>();
    std::unique_ptr<System>system;
    system = std::make_unique<System>();
    system->InitGraphicSystem(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("DirectX12 ���색�C�u����"),*camera_3d);
    
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

    return 0;
}
