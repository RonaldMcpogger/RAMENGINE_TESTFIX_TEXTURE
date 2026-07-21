

#pragma once
#include <DX3D/All.h>
#include <d3d11.h>
#include <wrl.h>

class MainGame : public dx3d::Game
{
public:
	explicit MainGame(const dx3d::GameDesc& desc);
protected:
	virtual void onCreate();
	virtual void onUpdate(dx3d::f32 deltaTime);

private:
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool m_showCredits{ false };
	bool m_showPicker{ false };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_creditsLogo{};
	dx3d::ui32 m_creditsLogoWidth{};
	dx3d::ui32 m_creditsLogoHeight{};
};

