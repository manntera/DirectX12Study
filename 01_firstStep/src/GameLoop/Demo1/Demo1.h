#pragma once
//■--------------------■//
//■---【Demo1.h】 -----■//
//■--------------------■//
//■-Demo1を記述します。■//
//■--------------------■//

//■-------------------------■//
//■---【インクルード記述】--■//
//■-------------------------■//
#include<OglGameLoop.h>
#include<OglWindow.h>

#include "../../d3dx12.h"
#include <d3d12.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

namespace OGL
{
	class Demo1 :public GameScene
	{
	private:
		struct Vertex
		{
			XMFLOAT3	pos;
			XMFLOAT4	col;
		};
		const int						m_frameBufferCount = 2;

		ComPtr<ID3D12Device>				m_device;
		ComPtr<ID3D12CommandQueue>			m_commandQueue;
		ComPtr<IDXGISwapChain3>				m_swapChain;
		ComPtr<ID3D12DescriptorHeap>		m_rtvHeap;
		ComPtr<ID3D12Resource>*				m_backBuffer;
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
		ComPtr<ID3D12RootSignature>			m_rootSignature;
		ComPtr<ID3D12PipelineState>			m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList>	m_commandList; 
		ComPtr<ID3D12Resource>				m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW			m_vertrexBufferView;

		UINT							m_frameBufferIndex;
		UINT							m_rtvDescriptorSize;

	public:
		virtual ~Demo1() {};
		Demo1() {};
		void *operator new(size_t size) {
			return _mm_malloc(size, alignof(Demo1));
		}
	public:
		bool	Init(void);
		bool	Update(void);
		bool	Draw(void);
		bool	Uninit(void);
	};	//EndOf__class_Demo1
}	//EndOf__namespace_OGL