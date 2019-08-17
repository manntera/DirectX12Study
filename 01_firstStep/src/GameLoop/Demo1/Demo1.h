#pragma once
//��--------------------��//
//��---�yDemo1.h�z -----��//
//��--------------------��//
//��-Demo1���L�q���܂��B��//
//��--------------------��//

//��-------------------------��//
//��---�y�C���N���[�h�L�q�z--��//
//��-------------------------��//
#include<OglGameLoop.h>
#include<OglWindow.h>

#include "../../d3dx12.h"
#include <d3d12.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "../../MathDefine.h"
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

namespace OGL
{
	class Demo1 :public GameScene
	{
	private:
		struct Vertex
		{
			float3	pos;
			float4	col;
		//	float2	texUV;
		};
		const int						m_frameBufferCount = 2;
		D3D12_VIEWPORT						m_viweport;
		D3D12_RECT							m_scissorRect;
		ComPtr<ID3D12Device>				m_device;
		ComPtr<ID3D12CommandQueue>			m_commandQueue;
		ComPtr<IDXGISwapChain3>				m_swapChain;
		ComPtr<ID3D12DescriptorHeap>		m_dhRtv;
		ComPtr<ID3D12Resource>*				m_backBuffer;
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
		ComPtr<ID3D12RootSignature>			m_rootSignature;
		ComPtr<ID3D12PipelineState>			m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;
		
		ComPtr<ID3D12DescriptorHeap>		m_dhCbvSrvUav;
		ComPtr<ID3D12Resource>				m_constantBuffer;
		XMMATRIX							m_matrix;

		ComPtr<ID3D12Resource>				m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW			m_vertexBufferView;

		ComPtr<ID3D12Resource>				m_indexBuffer;
		D3D12_INDEX_BUFFER_VIEW				m_indexBufferView;

		// �����I�u�W�F�N�g
		HANDLE				m_fenceEvent;
		ComPtr<ID3D12Fence>	m_fence;
		UINT64				m_fenceValue;

		UINT							m_frameBufferIndex;
		UINT							m_rtvDescriptorSize;

		bool WaitForPreviousFrame();
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