#pragma once
namespace StzEngine::Graphics
{
	class GraphicsSystem final
	{
	public:
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static GraphicsSystem* Get();

		GraphicsSystem() = default;
		~GraphicsSystem();

		GraphicsSystem(const GraphicsSystem&) = delete;
		GraphicsSystem(const GraphicsSystem&&) = delete;
		GraphicsSystem& operator=(const GraphicsSystem&) = delete;
		GraphicsSystem& operator=(const GraphicsSystem&&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void EndRender();

		void ToggleFullscreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTargetView();
		void ResetViewport();


		// void SetClearColor();
		void SetVsync(bool vSync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;
		float BackBufferAspectRatio() const;

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetContext() const;
	private:
		static LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;

		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;

		DXGI_SWAP_CHAIN_DESC mSwapChainDeskP{};
		D3D11_VIEWPORT mViewport{};

		UINT mVsync = 1;

	};
}