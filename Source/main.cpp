
#include <memory>
#include <iostream>
#include <Windows.h>

#include "System.h"
#include "Camera.h"
#include "Light.h"

using namespace std;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    std::unique_ptr<System>system;
    system = make_unique<System>();
    system->InitGraphicSystem(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("DirectX12 ���색�C�u����"));
    
    //3D�p�@�J����
    std::unique_ptr<Camera> camera3d;
    camera3d = make_unique<Camera>();
    //�J�����̈ʒu��ݒ�
    camera3d->SetPosition(0.0f, 0.0f, 100.0f);
    //�J�����̃^�[�Q�b�g��ݒ�
    camera3d->SetTarget(0.0f, 0.0f, 0.0f);

    //���C�g
    std::unique_ptr<Light> light;
    light = make_unique<Light>();
    //���C�g�͉E���瓖�����Ă���
    light->SetDirectionLightDirection(1.0f, -1.0f, -1.0f);
    light->direction_Light_.direction.Normalize();
    //���C�g�̐F�͔�
    light->SetDirectionLightColor(0.5f, 0.5f, 0.5f);
    //���_�̈ʒu��ݒ肷��
    light->eye_Position_.Set(camera3d->GetPosition());



    return 0;
}
