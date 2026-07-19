/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2026, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "Texture.h"

dx3d::Texture::Texture(const TextureDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc)
{
	if (!desc.size.width) DX3DLogThrowInvalidArg("Width must be non-zero.");
	if (!desc.size.height) DX3DLogThrowInvalidArg("Height must be non-zero.");
	if (!desc.pixels) DX3DLogThrowInvalidArg("Pixels must be a valid array.");

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = desc.size.width;
	texDesc.Height = desc.size.height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.pixels;
	initData.SysMemPitch = desc.size.width * 4;

	DX3DGraphicsLogThrowOnFail(m_device.CreateTexture2D(&texDesc, &initData, &m_texture),
		"CreateTexture2D failed.");

	D3D11_SHADER_RESOURCE_VIEW_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDesc.Texture2D.MipLevels = 1;
	resDesc.Texture2D.MostDetailedMip = 0;

	DX3DGraphicsLogThrowOnFail(m_device.CreateShaderResourceView(m_texture.Get(), &resDesc,
		&m_srv), "CreateShaderResourceView failed.");
}
