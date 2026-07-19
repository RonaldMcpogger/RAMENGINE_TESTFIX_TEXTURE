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

#include<DX3D/Game/GameObject.h>
#include<DX3D/Game/Component.h>
#include<DX3D/Component/TransformComponent.h>
#include<DX3D/Game/World.h>

dx3d::GameObject::GameObject(const GameObjectDesc& desc) : Identifiable(desc.base), m_world(desc.world), m_gameContext(desc.gameContext)
{
	m_transform = createOrGetComponent<TransformComponent>();
}

dx3d::TransformComponent& dx3d::GameObject::getTransform() noexcept
{
	return *m_transform;
}

dx3d::World& dx3d::GameObject::getWorld() noexcept
{
	return m_world;
}

dx3d::InputSystem& dx3d::GameObject::getInputSystem() noexcept
{
	return m_gameContext.input;
}

dx3d::ResourceManager& dx3d::GameObject::getResourceManager() noexcept
{
	return m_gameContext.resourceManager;
}

dx3d::Component* dx3d::GameObject::createComponentInternal(UniquePtr<Component>& component)
{
	if (!component) return {};
	auto typeId = component->getTypeId();
	auto ptr = component.get();
	if (m_components.find(typeId) != m_components.end()) return {};
	m_components.emplace(typeId, std::move(component));
	m_world.addComponentInternal(*ptr);
	return ptr;
}

dx3d::Component* dx3d::GameObject::getComponentInternal(size_t id)
{
	auto it = m_components.find(id);
	if (it == m_components.end()) return {};
	return it->second.get();
}
