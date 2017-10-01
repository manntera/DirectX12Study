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

//◇-------------------◇
//◇--	【Init】 ------◇
//◇--	機能：初期化 --◇
//◇-------------------◇
bool	Demo1::Init(void)
{
	ComPtr<IDXGIFactory4>	factory;

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

	//CreateDescriptorHeap
	{
		D3D12_DESCRIPTOR_HEAP_DESC	rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = m_frameBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (FAILED(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)))) {
			MessageBox(nullptr, "記述子ヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//CreateBackBuffer
	{
		D3D12_CPU_DESCRIPTOR_HANDLE	rtvHandle = {};
		rtvHandle.ptr = m_rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		m_backBuffer = new ComPtr<ID3D12Resource>[m_frameBufferCount];
		for (UINT i = 0; i < m_frameBufferCount; i++) {
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

		//CreateGraphicsPipelineState
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC	psoDesc = {};

			psoDesc.pRootSignature = m_rootSignature.Get();

			//CreateInputElement
			{
				D3D12_INPUT_ELEMENT_DESC	inputElementDescs[] = {
					{"POSITION",	0,DXGI_FORMAT_R32G32B32_FLOAT		,0,	0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
					{ "COLOR",		0,DXGI_FORMAT_R32G32B32A32_FLOAT	,0,	12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
				};
				psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };
			}
			//CreateVertexShader
			{
				ComPtr<ID3DBlob> vs;
				D3D12_SHADER_BYTECODE shaderByteCode;
				if (FAILED(D3DReadFileToBlob(L"cso/Demo1_VS.cso", vs.GetAddressOf()))) {
					MessageBox(nullptr, "頂点シェーダーの読み込みに失敗しました。", "Error", MB_OK);
					return false;
				}
			//	if (FAILED(D3DCompileFromFile(L"src/GameLoop/Demo1/HLSL/Demo1_VS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, vs.GetAddressOf(), nullptr))) return FALSE;
				shaderByteCode.pShaderBytecode = vs->GetBufferPointer();
				shaderByteCode.BytecodeLength = vs->GetBufferSize();
				psoDesc.VS = shaderByteCode;
			}
			//CreatePixelShader
			{
				ComPtr<ID3DBlob> ps;
				D3D12_SHADER_BYTECODE shaderByteCode;
				if (FAILED(D3DReadFileToBlob(L"cso/Demo1_PS.cso", ps.GetAddressOf()))) {
					MessageBox(nullptr, "ピクセルシェーダーの読み込みに失敗しました。", "Error", MB_OK);
					return false;
				}
//				if (FAILED(D3DCompileFromFile(L"src/GameLoop/Demo1/HLSL/Demo1_PS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, ps.GetAddressOf(), nullptr))) return FALSE;
				shaderByteCode.pShaderBytecode = ps->GetBufferPointer();
				shaderByteCode.BytecodeLength = ps->GetBufferSize();
				psoDesc.PS = shaderByteCode;
			}
			//CreateRasterizerState
			{
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
			}
			//CreateBlendState
			{
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
			}
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
	return true;
}
//◇---------------------◇
//◇--	【Uninit】 ------◇
//◇--	機能：終了処理 --◇
//◇---------------------◇
bool	Demo1::Uninit(void)
{
	return true;
}