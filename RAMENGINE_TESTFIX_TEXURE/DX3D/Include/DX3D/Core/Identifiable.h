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
#include <type_traits> 


#define dx3d_typeid(Class) \
public:\
using type_id_tag = Class;\
static size_t GetTypeId()\
{\
	static const auto id = typeid(Class).hash_code();\
	return id;\
}\
size_t getTypeId() const noexcept override\
{\
	return GetTypeId();\
}


namespace dx3d
{
	class Identifiable : public Base
	{
	public:
		explicit Identifiable(const BaseDesc& desc) :
			Base(desc)
		{
		}
		virtual size_t getTypeId() const noexcept = 0;
	};
	
	template <typename T>
	concept HasTypeId = std::is_same_v<typename T::type_id_tag, T>;

	template <typename Base, typename Derived>
	concept IsRegistered = std::is_base_of_v<Base, Derived> && HasTypeId<Derived>;
}
