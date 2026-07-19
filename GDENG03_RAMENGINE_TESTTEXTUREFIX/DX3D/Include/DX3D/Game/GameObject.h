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
#include <DX3D/Core/Common.h>
#include <DX3D/Core/Identifiable.h>
#include <DX3D/Game/Component.h>

#include <unordered_map>

namespace dx3d
{
	class GameObject : public Identifiable
	{
		dx3d_typeid(GameObject)
	public:
		explicit GameObject(const GameObjectDesc& desc);

		template <typename T>
		T* createOrGetComponent() requires IsRegistered<Component, T>
		{
			auto c = getComponent<T>();
			if (c) return c;
			UniquePtr<Component> cp = std::make_unique<T>(ComponentDesc{
								{m_logger},
								*this,
								m_world,
								m_gameContext
				});
			return static_cast<T*>(createComponentInternal(cp));
		}

		template <typename T>
		T* getComponent() requires IsRegistered<Component, T>
		{
			return static_cast<T*>(getComponentInternal(T::GetTypeId()));
		}

		TransformComponent& getTransform() noexcept;
		World& getWorld() noexcept;
		InputSystem& getInputSystem() noexcept;
		ResourceManager& getResourceManager() noexcept;
	protected:
		virtual void onCreate() {}
		virtual void onUpdate(f32 deltaTime) {}	
	private:
		Component* createComponentInternal(UniquePtr<Component>& component);
		Component* getComponentInternal(size_t id);
	private:
		std::unordered_map<size_t, UniquePtr<Component>> m_components{};
		TransformComponent* m_transform{};
		GameContext m_gameContext;
		World& m_world;	
		friend class World;
	};
}

