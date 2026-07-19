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
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Identifiable.h>
#include <unordered_map>
#include <vector>


namespace dx3d
{
	class World final : public Base
	{
	public:
		explicit World(const WorldDesc& desc);

		template <typename T>
		T* createGameObject() requires IsRegistered<GameObject, T>
		{
			UniquePtr<GameObject> e = std::make_unique<T>(GameObjectDesc{ 
				{m_logger},
				m_gameContext,
				*this 
				});
			return static_cast<T*>(createGameObjectInternal(e));
		}

		template <typename T> requires IsRegistered<Component, T>
		T* const* getComponents(ui32& numComponents) const noexcept
		{
			return reinterpret_cast<T* const*>(getComponentsInternal(T::GetTypeId(), &numComponents));
		}

		void update(f32 deltaTime);
	private:
		GameObject* createGameObjectInternal(UniquePtr<GameObject>& object);
		void addComponentInternal(Component& component);
		void addDirtyTransformInternal(TransformComponent& component);

		Component* const* getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept;
	private:
		enum class EventType
		{
			Create = 0
		};
		struct GameObjectEvent
		{
			GameObject* object{};
			size_t pendingObjectIndex{};
			EventType eventType{};
		};

	private:	
		GameContext m_gameContext;

		std::unordered_map<size_t, std::vector<UniquePtr<GameObject>>> m_objects{};
		std::unordered_map<size_t, std::vector<Component*>> m_components{};

		std::vector<TransformComponent*> m_dirtyTransforms{};

		std::vector<UniquePtr<GameObject>> m_pendingObjects;
		std::vector<UniquePtr<GameObject>> m_pendingObjectsSwapBuffer;

		std::vector<GameObjectEvent> m_events{};
		std::vector<GameObjectEvent> m_eventsSwapBuffer{};


		friend class GameObject;
		friend class TransformComponent;
	};
}

