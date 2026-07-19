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

#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <d3dcompiler.h>

namespace dx3d
{
	class GraphicsPipelineLayout final : public GraphicsResource
	{
	public:
		GraphicsPipelineLayout(const GraphicsPipelineLayoutDesc& desc, const GraphicsResourceDesc& gDesc);
		BinaryData getVSBinaryData() const noexcept;
		BinaryData getPSBinaryData() const noexcept;
		BinaryData getInputElementsData() const noexcept;
		ui32 getMaxTextureSlots() const noexcept;
		ui32 getMaxSamplerSlots() const noexcept;
		ui32 getMaxConstantBufferSlots() const noexcept;

	private:
		void processShaderBinary(ShaderBinary& binary);
	private:
		RefPtr<ShaderBinary> m_vsBinary{};
		RefPtr<ShaderBinary> m_psBinary{};
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_reflections[2]{};

		D3D11_INPUT_ELEMENT_DESC m_elements[D3D11_STANDARD_VERTEX_ELEMENT_COUNT]{};
		ui32 m_numElements{};
		ui32 m_maxTextureSlots{};
		ui32 m_maxSamplerSlots{};
		ui32 m_maxBufferSlots{};
	};
}

