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
#include <DX3D/Core/Core.h>
#include <DX3D/Game/Component.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Mat4x4.h>


namespace dx3d
{
	class TransformComponent final : public Component
	{
		dx3d_typeid(TransformComponent)
	public:
		explicit TransformComponent(const ComponentDesc& data);

		void setPosition(const Vec3& position);
		Vec3 getPosition() const noexcept;

		void setRotation(const Vec3& rotation);
		Vec3 getRotation() const noexcept;

		void setScale(const Vec3& scale);
		Vec3 getScale() const noexcept;

		Vec3 forward() ;
		Vec3 right() ;
		Vec3 up() ;

		Mat4x4 getAffineWorldMatrix() noexcept;
		Mat4x4 getRigidWorldMatrix() noexcept;

		void updateWorldMatrix() noexcept;
	private:
		void markAsDirty();
	private:
		Vec3 m_position{ 0,0,0 };
		Vec3 m_rotation{ 0,0,0 };
		Vec3 m_scale{ 1,1,1 };
		
		Mat4x4 m_rigidWorldMatrix{};   // rotation + translation only
		Mat4x4 m_affineWorldMatrix{};  // rotation + translation + scale

		bool m_dirty{};
	};
}
