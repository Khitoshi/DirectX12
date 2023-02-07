#include "imguiSystem.h"

#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "../GraphicsEngine.h"
#include "../RenderContext.h"
#include <Windows.h>



imguiSystem::imguiSystem():
	imgui_Heap_()
{
}

imguiSystem::~imguiSystem()
{
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT imguiSystem::WindowProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}


//imgui�p �f�B�X�N���v�^�q�[�v�@����
void imguiSystem::CreateDescriptorHeapForImgui(ID3D12Device5* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//����
	HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->imgui_Heap_));

	if (FAILED(hr))
	{
		//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s����
		MessageBox(nullptr, L"imgui�p�f�B�X�N���v�^�q�[�v�̐����Ɏ��s", L"�G���[", MB_OK);

		//�ُ�I��
		std::abort();
	}

}

//imgui ������
void imguiSystem::Init(GraphicsEngine* graphicsEngine,HWND hWnd)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);

	auto device = graphicsEngine->GetDevice();

	CreateDescriptorHeapForImgui(device);
	ImGui_ImplDX12_Init(
		device,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		this->imgui_Heap_.Get(),
		this->imgui_Heap_->GetCPUDescriptorHandleForHeapStart(),
		this->imgui_Heap_->GetGPUDescriptorHandleForHeapStart()
	);
}

//�X�V
void imguiSystem::Update()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//�`��
void imguiSystem::Drow(GraphicsEngine* graphicsEngine)
{
	ImGui::Render();
	auto cmd = graphicsEngine->GetCommandList();
	//graphicsEngine->GetRenderContext().SetDescriptorHeap(this->imgui_Heap_.Get());
	cmd->SetDescriptorHeaps(1, this->imgui_Heap_.GetAddressOf());

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmd);
}
