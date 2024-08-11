#include "ImGuiManager.h"

#include "DXCommon.h"
#include "WinApp.h"
#include "SrvManager.h"
#include "NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*							  フレームの開始
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Begin() {

#ifdef _DEBUG

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*							 内部コマンドの生成
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::End() {

#ifdef _DEBUG

	ImGui::Render();
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*								 終了処理
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Finalize() {

#ifdef _DEBUG
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*								  描画
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Draw() {
#ifdef _DEBUG

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*								  初期化
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Initialize(WinApp* winApp, DXCommon* dxCommon, SrvManager* srvManager) {

	assert(winApp);
	assert(dxCommon);
	assert(srvManager);

	winApp_ = winApp;
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

#ifdef _DEBUG
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(),
		dxCommon_->GetSwapChainDesc().BufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvManager_->GetDescriptorHeap(),
		srvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		srvManager_->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	);
#endif
}