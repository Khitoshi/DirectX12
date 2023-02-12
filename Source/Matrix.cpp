
#include "Quaternion.h"
#include "Matrix.h"

void Matrix::MakeRotationFromQuaternion(const Quaternion & q)
{
	DirectX::XMStoreFloat4x4(
		&mat,
		DirectX::XMMatrixRotationQuaternion(q)
	);
}