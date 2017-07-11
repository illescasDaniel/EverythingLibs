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

#include <cstddef>
#include <ostream>

namespace evt {
	
	template <typename Type>
	class Optional {
		
	private:
		Type* value_ { nullptr };
		
	public:
		
		Optional(const std::nullptr_t& value = nullptr) { this->value_ = value; }
		
		Optional(const Type& value) {
			this->value_ = new Type{value};
		}
		
		~Optional() {
			if (this->isNotNull()) {
				delete value_;
				value_ = nullptr;
			}
		}
		
		inline Type valueOr(const Type& other) const {
			return (this->isNotNull()) ? this->value() : other;
		}
		
		inline Type orEmpty() const {
			return (this->isNotNull()) ? this->value() : Type{};
		}
		
		inline bool isNull() const {
			return value_ == nullptr;
		}
		
		inline bool isNotNull() const {
			return value_ != nullptr;
		}
		
		Optional& operator=(const Optional& other) {
			if (this->isNull()) {
				this->value_ = new Type{other.value()};
			}
			else {
				*this->value_ = other.value();
			}
			return *this;
		}
		
		Optional& operator=(const Type& value) {
			if (this->isNull()) {
				this->value_ = new Type{value};
			}
			else {
				*this->value_ = value;
			}
			return *this;
		}
		
		inline bool operator==(const Optional& other) const {
			return value_ == other.value_;
		}
		
		inline bool operator!=(const Optional& other) const {
			return value_ != other.value_;
		}
		
		inline Type& operator*() const {
			return *value_;
		}
		
		inline Type& value() const {
			return *value_;
		}
		
		inline operator bool() const {
			return this->isNotNull();
		}
		
		inline operator Type() const {
			return this->orEmpty();
		}
		
		friend std::ostream& operator<<(std::ostream& os, const Optional& optionalValue) {
			return os << optionalValue.orEmpty();
		}
	};
}

