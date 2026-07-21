
#include "MainGame.h"
#include "Objects/Player.h"

#include <DX3D/Graphics/GraphicsDevice.h>
#include <imgui.h>

#include <Windows.h>
#include <wincodec.h>
#include <algorithm>
#include <vector>

namespace
{
	// Loads common image formats supported by Windows Imaging Component (PNG, JPG,
	// BMP, and more) into a D3D11 texture that ImGui can display.
	bool loadTextureFromFile(const wchar_t* filePath, ID3D11Device* device,
		ID3D11ShaderResourceView** textureView, dx3d::ui32* width, dx3d::ui32* height)
	{
		const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		const bool shouldUninitializeCom = SUCCEEDED(comResult);
		Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
		Microsoft::WRL::ComPtr<IWICFormatConverter> converter;

		HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&factory));
		if (SUCCEEDED(result))
			result = factory->CreateDecoderFromFilename(filePath, nullptr, GENERIC_READ,
				WICDecodeMetadataCacheOnLoad, &decoder);
		if (SUCCEEDED(result)) result = decoder->GetFrame(0, &frame);
		if (SUCCEEDED(result)) result = factory->CreateFormatConverter(&converter);
		if (SUCCEEDED(result))
			result = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
				WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

		UINT imageWidth = 0, imageHeight = 0;
		if (SUCCEEDED(result)) result = converter->GetSize(&imageWidth, &imageHeight);
		std::vector<unsigned char> pixels;
		if (SUCCEEDED(result))
		{
			pixels.resize(static_cast<size_t>(imageWidth) * imageHeight * 4);
			result = converter->CopyPixels(nullptr, imageWidth * 4,
				static_cast<UINT>(pixels.size()), pixels.data());
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		if (SUCCEEDED(result))
		{
			D3D11_TEXTURE2D_DESC desc{};
			desc.Width = imageWidth;
			desc.Height = imageHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			D3D11_SUBRESOURCE_DATA data{ pixels.data(), imageWidth * 4, 0 };
			result = device->CreateTexture2D(&desc, &data, &texture);
		}
		if (SUCCEEDED(result)) result = device->CreateShaderResourceView(texture.Get(), nullptr, textureView);
		if (SUCCEEDED(result))
		{
			*width = imageWidth;
			*height = imageHeight;
		}
		if (shouldUninitializeCom) CoUninitialize();
		return SUCCEEDED(result);
	}
}

MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();
	auto& world = getWorld();
	auto woodTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/wood.jpg");
	auto floorTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/floor.jpg");

	loadTextureFromFile(L"Assets/Images/malinapls.png", getGraphicsDevice().getD3DDevice(),
		m_creditsLogo.GetAddressOf(), &m_creditsLogoWidth, &m_creditsLogoHeight);
	{
		auto basicMat = getResourceManager().createResourceFromFile<dx3d::MaterialResource>(L"Game/Assets/Shaders/Basic.hlsl");
		if (basicMat)
		{
			auto matData = dx3d::Vec3(1, 1, 1);
			basicMat->setData(std::as_bytes(std::span{ &matData, 1 }));
			basicMat->setTexture(0, floorTex);
		}

		auto floor = world.createGameObject<dx3d::GameObject>();
		floor->createOrGetComponent<dx3d::CubeComponent>();
		auto comp = floor->createOrGetComponent<dx3d::CubeComponent>();
		comp->setMaterial(basicMat);
		floor->getTransform().setScale({ 6.8f, 0.1f, 6.8f });
		floor->getTransform().setPosition({ 0, 0, 0 });
		
	}

	srand((unsigned int)time(NULL));

	for (auto y = -2; y < 3; y++)
	{
		for (auto x = -2; x < 3; x++)
		{
			auto basicMat = getResourceManager().createResourceFromFile<dx3d::MaterialResource>(L"Game/Assets/Shaders/Basic.hlsl");
			if (basicMat)
			{
				auto matData = dx3d::Vec3(1, 1, 1);
				basicMat->setData(std::as_bytes(std::span{ &matData, 1 }));
				basicMat->setTexture(0, woodTex);
			}

			auto cube = world.createGameObject<dx3d::GameObject>();
			auto comp = cube->createOrGetComponent<dx3d::CubeComponent>();
			comp->setMaterial(basicMat);
			auto roty = (rand() % 628) / 100.0f;
			cube->getTransform().setScale({ 0.5,0.5,0.5 });
			cube->getTransform().setPosition({ x * 1.4f, 0.25f + 0.05f, y * 1.4f });
			cube->getTransform().setRotation({ 0,roty,0 });
		}
	}

	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0, 1, -2});

	getInputSystem().setCursorLocked(false);
	getInputSystem().setCursorVisible(true);

}


void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { PostQuitMessage(0); }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("ColorPicker"))
				m_showPicker = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Credits"))
				m_showCredits = true;
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (m_showCredits)
	{
		ImGui::Begin("Credits", &m_showCredits);
		ImGui::TextWrapped("RAMEGNINE");
		if (m_creditsLogo)
		{
			const float availableWidth = ImGui::GetContentRegionAvail().x;
			const float displayWidth = (std::min)(availableWidth, static_cast<float>(m_creditsLogoWidth) / 2);
			const float displayHeight = displayWidth * m_creditsLogoHeight / m_creditsLogoWidth;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (availableWidth - displayWidth) * 0.5f);
			ImGui::Image(reinterpret_cast<ImTextureID>(m_creditsLogo.Get()), ImVec2(displayWidth, displayHeight));
		}
		else
		{
			ImGui::TextDisabled("Credits logo not found");
		}

		ImGui::Separator();
		ImGui::TextWrapped("RAMEGNINE is a 3D engine created by Rameses P. Amar.");
		ImGui::Separator();
		ImGui::BulletText("Created by Rameses P. Amar");
		ImGui::BulletText("Based on PardCode C++ Game Engine Tutorial Series");
		ImGui::End();
	}

	if (m_showPicker)
	{
		ImGui::Begin("Editor", &m_showPicker);
		if (ImGui::BeginTabBar("EditorTab"))
		{
			if (ImGui::BeginTabItem("ColorWheel"))
			{
				ImGui::ColorPicker4("Color Picker", color);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}
