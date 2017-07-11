/*
 The MIT License (MIT)
 
 Copyright (c) 2017 Daniel Illescas Romero <https://github.com/illescasDaniel>
 
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
	
	class Any {
		
		std::shared_ptr<void*> value_ { nullptr };
		
	public:
		
		template<typename Type>
		Any(const Type& value) {
			this->value_ = std::make_shared<void*>(new Type{value});
		}
		
		Any(const char* value) {
			this->value_ = std::make_shared<void*>(new std::string{value});
		}
		
		template<typename Type>
		Type& as() const {
			return *static_cast<Type*>(*this->value_);
		}
		
		template <typename Type>
		inline operator Type() const {
			return this->as<Type>();
		}
		
		std::string operator=(const char* newValue) {
			value_ = std::make_shared<void*>(new std::string{newValue});
			return static_cast<std::string>(std::string(newValue));
		}
		
		template<typename Type>
		Type& operator=(const Type& newValue) {
			value_ = std::make_shared<void*>(new Type{newValue});
			return *reinterpret_cast<Type*>(*value_);
		}
	};
}
