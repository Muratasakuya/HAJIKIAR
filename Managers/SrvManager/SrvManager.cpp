#include "SrvManager.h"

#include "DXCommon.h"



// SRVの最大数
const uint32_t SrvManager::kMaxSRVCount_ = 512;

// descriptorHeap_.Get() getter
ID3D12DescriptorHeap* SrvManager::GetDescriptorHeap() const {

	return descriptorHeap_.Get();
}

// CPUHandleの取得
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}

// GPUHandleの取得
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_.Get()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);

	return handleGPU;
}

// DescriptorHeapを作成する関数
ComPtr<ID3D12DescriptorHeap> SrvManager::MakeDescriptorHeap(ID3D12Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {

	HRESULT hr;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = heapType;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags =
		shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}



/*////////////////////////////////////////////////////////////////////////////////

*							SRV生成 2DTexture

////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;

	dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}



/*////////////////////////////////////////////////////////////////////////////////

*						  SRV生成 StructureBuffer

////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}



/*////////////////////////////////////////////////////////////////////////////////

*							使用インデックスの計算

////////////////////////////////////////////////////////////////////////////////*/
uint32_t SrvManager::Allocate() {

	if (!CanAllocate()) {
		throw std::runtime_error("Cannot allocate more SRVs, maximum count reached.");
	}

	// returnする番号を一旦記録しておく
	int index = useIndex_;
	// 次回のために1進める
	useIndex_++;

	return index + 1;
}



/*////////////////////////////////////////////////////////////////////////////////

*							割り当て可能かどうかのチェック

////////////////////////////////////////////////////////////////////////////////*/
bool SrvManager::CanAllocate() {

	return useIndex_ < kMaxSRVCount_;
}



/*////////////////////////////////////////////////////////////////////////////////

*								  描画前処理

////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::PreDraw() {

	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}



/*////////////////////////////////////////////////////////////////////////////////

*									初期化

////////////////////////////////////////////////////////////////////////////////*/
void SrvManager::Initialize(DXCommon* dxCommon) {

	assert(dxCommon);
	dxCommon_ = dxCommon;

	// 生成
	descriptorHeap_ =
		MakeDescriptorHeap(dxCommon_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);
	// デスクリプタ1個のサイズの取得
	descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}