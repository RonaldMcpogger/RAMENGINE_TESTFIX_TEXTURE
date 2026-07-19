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
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Rect.h>
#include <array>

namespace dx3d
{
	class InputSystem final: public Base
	{
	public:
		explicit InputSystem(const InputSystemDesc& desc);

		bool isKeyDown(KeyCode key) const;
		bool isKeyPressed(KeyCode key) const;
		bool isKeyReleased(KeyCode key) const;

		Vec2 getMousePosition() const noexcept;
		Vec2 getMouseDelta() const noexcept;

		void setCursorVisible(bool visible);
        void setCursorLocked(bool locked);
        void setCursorLockArea(const Rect& rect);

		void update();
	private:
		short getInternalKeyCode(const KeyCode& key);
		void centerCursor();
	private:
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)> m_currentKeys{};
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)> m_previousKeys{};

		Vec2 m_mousePosition{};
		Vec2 m_previousMousePosition{};
		Vec2 m_mouseDelta{};

		Rect m_lockArea{};

		bool m_cursorVisible{ true };
		bool m_cursorLocked{ false };
	};

}
