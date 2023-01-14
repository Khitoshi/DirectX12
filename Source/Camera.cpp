#include "Camera.h"

Camera::Camera():
    position_(XMFLOAT3(0,0,0)),
    target_(XMFLOAT3(0, 0, 0))
{
}

Camera::~Camera()
{
}
