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

#include <DX3D/Graphics/GraphicsPipelineLayout.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/GraphicsUtils.h>
#include <d3dcompiler.h>
#include <ranges>

dx3d::GraphicsPipelineLayout::GraphicsPipelineLayout(const GraphicsPipelineLayoutDesc& desc, const GraphicsResourceDesc& gDesc) : 
	GraphicsResource(gDesc), 
	m_vsBinary(desc.vsBinary), 
	m_psBinary(desc.psBinary)
{
	if (!desc.vsBinary) DX3DLogThrowInvalidArg("No shader binary provided.");
	if (desc.vsBinary->getType() != ShaderType::VertexShader)
		DX3DLogThrowInvalidArg("The 'vsBinary' member is not a valid vertex shader binary.");
	if (!desc.psBinary) DX3DLogThrowInvalidArg("No shader binary provided.");
	if (desc.psBinary->getType() != ShaderType::PixelShader)
		DX3DLogThrowInvalidArg("The 'psBinary' member is not a valid pixel shader binary.");

	processShaderBinary(*m_vsBinary);
	processShaderBinary(*m_psBinary);
}

dx3d::BinaryData dx3d::GraphicsPipelineLayout::getVSBinaryData() const noexcept
{
	return m_vsBinary->getData();
}

dx3d::BinaryData dx3d::GraphicsPipelineLayout::getPSBinaryData() const noexcept
{
	return m_psBinary->getData();
}


dx3d::BinaryData dx3d::GraphicsPipelineLayout::getInputElementsData() const noexcept
{
	return
	{
		m_elements,
		m_numElements
	};
}

dx3d::ui32 dx3d::GraphicsPipelineLayout::getMaxTextureSlots() const noexcept
{
	return m_maxTextureSlots;
}

dx3d::ui32 dx3d::GraphicsPipelineLayout::getMaxSamplerSlots() const noexcept
{
	return m_maxSamplerSlots;
}

dx3d::ui32 dx3d::GraphicsPipelineLayout::getMaxConstantBufferSlots() const noexcept
{
	return m_maxBufferSlots;
}

void dx3d::GraphicsPipelineLayout::processShaderBinary(ShaderBinary& binary)
{
	auto data = binary.getData();
	auto& reflection = m_reflections[static_cast<ui32>(binary.getType())];

	DX3DGraphicsLogThrowOnFail(D3DReflect(
		data.data,
		data.dataSize,
		IID_PPV_ARGS(&reflection)),
		"D3DReflect failed.");

	D3D11_SHADER_DESC shaderDesc{};
	DX3DGraphicsLogThrowOnFail(reflection->GetDesc(&shaderDesc),
		"ID3D11ShaderReflection::GetDesc failed.");

	if (binary.getType() == ShaderType::VertexShader)
	{
		m_numElements = shaderDesc.InputParameters;
		D3D11_SIGNATURE_PARAMETER_DESC params[D3D11_STANDARD_VERTEX_ELEMENT_COUNT]{};
		for (auto i : std::views::iota(0u, m_numElements))
		{
			DX3DGraphicsLogThrowOnFail(reflection->GetInputParameterDesc(i, &params[i]),
				"ID3D11ShaderReflection::GetInputParameterDesc failed.");
		}
		for (auto i : std::views::iota(0u, m_numElements))
		{
			auto param = params[i];
			m_elements[i] = {
				param.SemanticName,
				param.SemanticIndex,
				GraphicsUtils::GetDXGIFormatFromMask(param.ComponentType,param.Mask),
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			};
		}
	}

	{
		D3D11_SHADER_INPUT_BIND_DESC desc{};
		for (auto i : std::views::iota(0u, shaderDesc.BoundResources))
		{
			DX3DGraphicsLogThrowOnFail(reflection->GetResourceBindingDesc(i, &desc),
				"ID3D11ShaderReflection::GetInputParameterDesc failed.");
			if (desc.Type == D3D_SIT_CBUFFER)
				m_maxBufferSlots = std::max(m_maxBufferSlots, desc.BindPoint + 1);
			if (desc.Type == D3D_SIT_TEXTURE)
				m_maxTextureSlots = std::max(m_maxTextureSlots, desc.BindPoint + 1);
			if (desc.Type == D3D_SIT_SAMPLER)
				m_maxSamplerSlots = std::max(m_maxSamplerSlots, desc.BindPoint + 1);
		}
	}
}
