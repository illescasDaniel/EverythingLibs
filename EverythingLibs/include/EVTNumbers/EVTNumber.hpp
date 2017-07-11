/*
 The MIT License (MIT)
 
 Copyright (c) 2017 Daniel Illescas Romero
 
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
 SOFTWARE.
 */

#pragma once

#include <memory>

namespace evt {
	
	namespace numbers {
		
		class Number {
			
			std::shared_ptr<void*> value_ { nullptr };
			
		public:
			
			template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
			Number(const Type& value) {
				this->value_ = std::make_shared<void*>(new Type{value});
			}
			
			template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
			Type& as() const {
				return *static_cast<Type*>(*this->value_);
			}
			
			template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
			inline operator Type() const {
				return this->as<Type>();
			}
			
			template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
			Type& operator=(const Type& newValue) {
				value_ = std::make_shared<void*>(new Type{newValue});
				return *reinterpret_cast<Type*>(*value_);
			}
			
			#define arithmeticOperation(name, operation) \
			template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type> \
			Type name(const Number& otherNumber) { \
				return this->as<Type>() operation otherNumber.as<Type>(); \
			}
			
			arithmeticOperation(add, +);
			arithmeticOperation(substract, -);
			arithmeticOperation(multiply, *);
			arithmeticOperation(divide, /);
			arithmeticOperation(modulus, %);
		};
	}
}