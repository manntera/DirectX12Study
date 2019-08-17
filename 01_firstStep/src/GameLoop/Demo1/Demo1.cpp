#pragma once
//■--------------------■//
//■---【Demo1.h】 -----■//
//■--------------------■//
//■-Demo1を記述します。■//
//■--------------------■//

//■-------------------------■//
//■---【インクルード記述】--■//
//■-------------------------■//
#include"Demo1.h"

using namespace OGL;
using namespace DirectX;

//◇-------------------◇
//◇--	【Init】 ------◇
//◇--	機能：初期化 --◇
//◇-------------------◇
bool	Demo1::Init(void)
{
	ComPtr<IDXGIFactory4>	factory;
#if defined(_DEBUG)
	// DirectX12のデバッグレイヤーを有効にする
	{
		ComPtr<ID3D12Debug>	debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();
		}
	}
#endif

	//CreateDevice
	{
		//DirectX12をサポートするハードウェアを取得
		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) return false;

		ComPtr<IDXGIAdapter1>	hardwareAdapter = nullptr;
		ComPtr<IDXGIAdapter1>	tempAdapter;

		for (int i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &tempAdapter); i++) {
			DXGI_ADAPTER_DESC1	desc;
			tempAdapter->GetDesc1(&desc);
			if (desc.Flags& DXGI_ADAPTER_FLAG_SOFTWARE) {
				continue;
			}
			if (SUCCEEDED(D3D12CreateDevice(tempAdapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) {
				break;
			}
		}
		hardwareAdapter = tempAdapter.Detach();
		if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_device.GetAddressOf())))) {
			MessageBox(nullptr, "デバイスの作成に失敗しました", "Error", MB_OK);
			return false;
		}
	}

	//CreateCommandQueue
	{
		D3D12_COMMAND_QUEUE_DESC	queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		if (FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)))) {
			MessageBox(nullptr, "コマンドキューの作成に失敗しました", "Error", MB_OK);
			return false;
		}
	}
	//CreateSwapChain
	{
		DXGI_SWAP_CHAIN_DESC1	swapChainDesc = {};
		swapChainDesc.BufferCount = m_frameBufferCount;
		swapChainDesc.Width = Window::GetInstance()->GetWidth();
		swapChainDesc.Height = Window::GetInstance()->GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1>	swapChain;
		HWND hWnd = *Window::GetInstance()->GetHwnd();
		if (FAILED(factory->CreateSwapChainForHwnd(m_commandQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &swapChain))) {
			MessageBox(nullptr, "スワップチェインの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		if (FAILED(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER))) {
			MessageBox(nullptr, "スワップチェインの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		if (FAILED(swapChain.As(&m_swapChain))) {
			MessageBox(nullptr, "スワップチェインの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		m_frameBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	}

	//CreateViewport
	{
		m_viweport = { 0.0f,0.0f,
			(float)Window::GetInstance()->GetWidth(),
			(float)Window::GetInstance()->GetHeight() };

	}

	//CreateScissorRect
	{
		m_scissorRect = { 0,0,
			(LONG)Window::GetInstance()->GetWidth(),
			(LONG)Window::GetInstance()->GetHeight() };
	}

	//CreateDescriptorHeap
	{
		D3D12_DESCRIPTOR_HEAP_DESC	rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = m_frameBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (FAILED(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_dhRtv)))) {
			MessageBox(nullptr, "記述子ヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//CreateBackBuffer
	{
		D3D12_CPU_DESCRIPTOR_HANDLE	rtvHandle = {};
		rtvHandle.ptr = m_dhRtv->GetCPUDescriptorHandleForHeapStart().ptr;
		m_backBuffer = new ComPtr<ID3D12Resource>[m_frameBufferCount];

		for (int i = 0; i < m_frameBufferCount; i++) {
			if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffer[i])))) {
				MessageBox(nullptr, "バックッバッファの作成に失敗しました。", "Error", MB_OK);
				return false;
			}
			m_device->CreateRenderTargetView(m_backBuffer[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
	}

	//CreateCommandAllocator
	{
		if (FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocator)))) {
			MessageBox(nullptr, "コマンドアロケーターの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}
	//CreateRootSignature
	{
		D3D12_ROOT_SIGNATURE_DESC	rootSignatureDesc;
		rootSignatureDesc.NumParameters = 0;
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob>	signature;
		ComPtr<ID3DBlob>	error;
		if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error))) {
			MessageBox(nullptr, "ルートシグネチャの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		if (FAILED(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)))) {
			MessageBox(nullptr, "ルートシグネチャの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	//CreateGraphicsPipelineState
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC	psoDesc = {};

		//SetRootSignature
		psoDesc.pRootSignature = m_rootSignature.Get();

		//CreateInputElement
		D3D12_INPUT_ELEMENT_DESC	inputElementDescs[] = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT	,0,	0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"COLOR",	0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,	sizeof(float3),	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			//		{"TEX_UV",	0,DXGI_FORMAT_R32G32_FLOAT,0,	sizeof(float3) + sizeof(float4),	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
		};
		psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };

		//CreateVertexShader
		ComPtr<ID3DBlob> vs;
		if (FAILED(D3DReadFileToBlob(L"cso/Demo1_VS.cso", vs.GetAddressOf()))) {
			MessageBox(nullptr, "頂点シェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs.Get());

		//CreatePixelShader
		ComPtr<ID3DBlob> ps;
		if (FAILED(D3DReadFileToBlob(L"cso/Demo1_PS.cso", ps.GetAddressOf()))) {
			MessageBox(nullptr, "ピクセルシェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps.Get());

		//CreateRasterizerState
		D3D12_RASTERIZER_DESC	rsDesc = {};
		rsDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rsDesc.CullMode = D3D12_CULL_MODE_BACK;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rsDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rsDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rsDesc.DepthClipEnable = true;
		rsDesc.MultisampleEnable = false;
		rsDesc.AntialiasedLineEnable = false;
		rsDesc.ForcedSampleCount = 0;
		rsDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		psoDesc.RasterizerState = rsDesc;

		//CreateBlendState
		D3D12_BLEND_DESC	bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			D3D12_RENDER_TARGET_BLEND_DESC* rtbDesc = &bsDesc.RenderTarget[i];
			rtbDesc->BlendEnable = false;
			rtbDesc->LogicOpEnable = false;
			rtbDesc->SrcBlend = D3D12_BLEND_ONE;
			rtbDesc->DestBlend = D3D12_BLEND_ZERO;
			rtbDesc->BlendOp = D3D12_BLEND_OP_ADD;
			rtbDesc->SrcBlendAlpha = D3D12_BLEND_ONE;
			rtbDesc->DestBlendAlpha = D3D12_BLEND_ZERO;
			rtbDesc->BlendOpAlpha = D3D12_BLEND_OP_ADD;
			rtbDesc->LogicOp = D3D12_LOGIC_OP_NOOP;
			rtbDesc->RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		psoDesc.BlendState = bsDesc;

		psoDesc.DepthStencilState.DepthEnable = false;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		if (FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)))) {
			MessageBox(nullptr, "パイプラインステートの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}
	//CreateCommandList
	{
		if (FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)))) {
			MessageBox(nullptr, "コマンドリストの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		if (FAILED(m_commandList->Close())) {
			MessageBox(nullptr, "コマンドリストの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}
	//CreateVertexBuffer
	{
		//CreateGeometryData
		Vertex vertex[]{
			{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 1.0f, -1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } }
		};
		////CreateHeapProperties
		//D3D12_HEAP_PROPERTIES heapProperties = {};
		//{
		//	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		//	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		//	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		//	heapProperties.CreationNodeMask = 1;
		//	heapProperties.VisibleNodeMask = 1;
		//}
		////CreateResourceDesc
		//D3D12_RESOURCE_DESC resourcedesc = {};
		//{
		//	resourcedesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//	resourcedesc.Alignment = 0;
		//	resourcedesc.Width = sizeof(vertex);
		//	resourcedesc.Height = 1;
		//	resourcedesc.DepthOrArraySize = 1;
		//	resourcedesc.MipLevels = 1;
		//	resourcedesc.Format = DXGI_FORMAT_UNKNOWN;
		//	resourcedesc.SampleDesc.Count = 1;
		//	resourcedesc.SampleDesc.Quality = 0;
		//	resourcedesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//	resourcedesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		//}
		//CreateResourceForVertexBuffer
//		if (FAILED(m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourcedesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer)))) {
		
		//CreateResourceForVertexBuffer
		if (FAILED(m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex)),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer)))) {
			MessageBox(nullptr, "頂点バッファ用のリソースの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		//CopyToVertexBuffer
		{
			UINT8*	dataBegin;
			CD3DX12_RANGE readRange(0, 0);
			if (FAILED(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&dataBegin)))) {
				MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
				return false;
			}
			memcpy(dataBegin, vertex, sizeof(vertex));
			m_vertexBuffer->Unmap(0, nullptr);
		}
		//InitializeVertexBufferView
		{
			m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			m_vertexBufferView.StrideInBytes = sizeof(Vertex);
			m_vertexBufferView.SizeInBytes = sizeof(vertex);
		}
	}
	//CreateIndexBuffer
	{
		//CreateIndexData
		WORD index[] = { 0,1,2,0,2,3 };
		//CreateResourceForVertexBuffer
		if (FAILED(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(index)),
			D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&m_indexBuffer)))) {
			MessageBox(nullptr, "インデックスバッファ用のリソースの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		//CopyToIndexBuffer
		{
			UINT8*	databegin;
			CD3DX12_RANGE readRange(0, 0);
			if (FAILED(m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&databegin)))) {
				MessageBox(nullptr, "インデックスバッファの作成に失敗しました。", "Error", MB_OK);
				return false;
			}
			memcpy(databegin, index, sizeof(index));
			m_indexBuffer->Unmap(0, nullptr);
		}
		//InitializeIndexBufferView
		{
			m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
			m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
			m_indexBufferView.SizeInBytes = sizeof(index);
		}
	}
	//CreateDescriptorHeapForCBV_SRV_UAV
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (FAILED(m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_dhCbvSrvUav)))) {
			MessageBox(nullptr, "デスクリプタヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}
	//CreateConstantBuffer
	{
		if (FAILED(m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(float4x4)),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_constantBuffer)))) {
			MessageBox(nullptr, "コンスタントバッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}


		if (FAILED(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_matrix)))) {
			MessageBox(nullptr, "定数バッファ作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}
	//SetConstantBuffer
	{
		m_matrix = XMMatrixSet(10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	//SetDescriptor
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC csvDesc = {};
		csvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
		csvDesc.SizeInBytes = sizeof(float4x4);
		m_device->CreateConstantBufferView(&csvDesc, m_dhCbvSrvUav->GetCPUDescriptorHandleForHeapStart());
	}
	//CreateFence
	{
		if (FAILED(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)))) {
			MessageBox(nullptr, "フェンスの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		m_fenceValue = 1;
		m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
		if (m_fenceEvent == nullptr) {
			if (FAILED(HRESULT_FROM_WIN32(GetLastError()))) {
				MessageBox(nullptr, "フェンスの作成に失敗しました。", "Error", MB_OK);
				return false;
			}
		}
		if (!WaitForPreviousFrame()) {
			MessageBox(nullptr, "フェンスの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}


	return true;
}

//◇---------------------◇
//◇--	【Update】 ------◇
//◇--	機能：更新 ------◇
//◇---------------------◇
bool	Demo1::Update(void)
{

	return true;
}
//◇-------------------◇
//◇--	【Init】 ------◇
//◇--	機能：描画 ----◇
//◇-------------------◇
bool	Demo1::Draw(void)
{
	if (FAILED(m_commandAllocator->Reset())) {
		return false;
	}
	if (FAILED(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()))) {
		return false;
	}
	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viweport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	//バックバッファをレンダリングターゲットに
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier = {};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = m_backBuffer[m_frameBufferIndex].Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_commandList->ResourceBarrier(1, &resourceBarrier);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hRtv = m_dhRtv->GetCPUDescriptorHandleForHeapStart();
	hRtv.ptr += (m_frameBufferIndex*m_rtvDescriptorSize);
	m_commandList->OMSetRenderTargets(1, &hRtv, FALSE, nullptr);

	//バックバッファへの描画
	float clearCol[] = { 0.0f,0.2f,0.4f ,1.0f };
	m_commandList->ClearRenderTargetView(hRtv, clearCol, 0, nullptr);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->IASetIndexBuffer(&m_indexBufferView);
	m_commandList->DrawIndexedInstanced(6, 1,0, 0, 0);

	//バックバッファを表示
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier = {};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = m_backBuffer[m_frameBufferIndex].Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_commandList->ResourceBarrier(1, &resourceBarrier);
	}
	if (FAILED(m_commandList->Close())) {
		return false;
	}
	ID3D12CommandList* commandList[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

	//フレームの出力
	if (FAILED(m_swapChain->Present(1, 0))) {
		return false;
	}
	WaitForPreviousFrame();
	return true;
}
//◇---------------------◇
//◇--	【Uninit】 ------◇
//◇--	機能：終了処理 --◇
//◇---------------------◇
bool	Demo1::Uninit(void)
{
	WaitForPreviousFrame();
	CloseHandle(m_fenceEvent);
	delete[](m_backBuffer);
	return true;
}

bool Demo1::WaitForPreviousFrame()
{
	const UINT64	fence = m_fenceValue;
	if (FAILED(m_commandQueue->Signal(m_fence.Get(), fence))) return FALSE;
	m_fenceValue++;

	// 前のフレームが終了するまで待機
	if (m_fence->GetCompletedValue() < fence) {
		if (FAILED(m_fence->SetEventOnCompletion(fence, m_fenceEvent))) return FALSE;
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	m_frameBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	return TRUE;
}