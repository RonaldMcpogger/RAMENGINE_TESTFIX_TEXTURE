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

#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/MaterialResource.h>
#include <DX3D/Resource/TextureResource.h>
#include <filesystem>

dx3d::ResourceManager::ResourceManager(const ResourceManagerDesc& desc) : Base(desc.base), m_context(desc.context)
{
}

dx3d::RefPtr<dx3d::Resource> dx3d::ResourceManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	std::filesystem::path resourcePath{ file_path };
	auto ext = resourcePath.extension();

	auto it = m_resources.find(file_path);
	if (it != m_resources.end())
	{
		auto mat = std::dynamic_pointer_cast<MaterialResource>(it->second);
		if (mat)
			return std::make_shared<MaterialResource>(*mat, MaterialResourceDesc{ getResourceDesc(file_path), m_context.graphicsDevice });
		return it->second;
	}

	if (!std::filesystem::exists(resourcePath))
	{
		DX3DLogError("File {} doesn't exist.", resourcePath.string().c_str());
		return nullptr;
	}

	RefPtr<Resource> resPtr{};
	try
	{
		if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
			resPtr = std::make_shared<MaterialResource>(MaterialResourceDesc{ getResourceDesc(file_path), m_context.graphicsDevice });
		if (!ext.compare(L".jpg") || !ext.compare(L".png"))
			resPtr = std::make_shared<TextureResource>(TextureResourceDesc{ getResourceDesc(file_path), m_context.graphicsDevice });
	}
	catch (...)
	{
		DX3DLogError("Failed to load resource {}", resourcePath.string().c_str());
	}

	if (resPtr)
	{
		m_resources.emplace(file_path, resPtr);
		return resPtr;
	}

	return nullptr;
}

dx3d::ResourceDesc dx3d::ResourceManager::getResourceDesc(const wchar_t* file_path)
{
	return ResourceDesc{ { m_logger }, file_path, *this };
}

