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

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	template <typename Type>
	class Optional {
		
	private:
		Type* value_ { nullptr };
		
	public:
		
		CONSTEXPR Optional(const std::nullptr_t& value = nullptr) noexcept { this->value_ = value; }
		
		CONSTEXPR Optional(const Type& value) {
			this->value_ = new Type{value};
		}
		
		~Optional() {
			if (this->isNotNull()) {
				delete value_;
				value_ = nullptr;
			}
		}
		
		CONSTEXPR Type valueOr(const Type& other) const noexcept {
			return (this->isNotNull()) ? this->value() : other;
		}
		
		CONSTEXPR Type orEmpty() const noexcept {
			return (this->isNotNull()) ? this->value() : Type{};
		}
		
		CONSTEXPR bool isNull() const noexcept {
			return value_ == nullptr;
		}
		
		CONSTEXPR bool isNotNull() const noexcept {
			return value_ != nullptr;
		}
		
		CONSTEXPR Optional& operator=(const Optional& other) {
			if (this->isNull() and other.value_ != nullptr) {
				this->value_ = new Type{other.value()};
			}
			else {
				*this->value_ = other.value();
			}
			return *this;
		}
		
		CONSTEXPR Optional& operator=(const Type& value) {
			if (this->isNull()) {
				this->value_ = new Type{value};
			}
			else {
				*this->value_ = value;
			}
			return *this;
		}
		
		CONSTEXPR bool operator==(const Optional& other) const noexcept {
			return value_ == other.value_;
		}
		
		CONSTEXPR bool operator!=(const Optional& other) const noexcept {
			return value_ != other.value_;
		}
		
		CONSTEXPR Type& operator*() const noexcept {
			return *value_;
		}
		
		CONSTEXPR Type& value() const noexcept {
			return *value_;
		}
		
		CONSTEXPR operator bool() const noexcept {
			return this->isNotNull();
		}
		
		CONSTEXPR operator Type() const noexcept {
			return this->orEmpty();
		}
		
		CONSTEXPR friend std::ostream& operator<<(std::ostream& os, const Optional& optionalValue) noexcept {
			return os << optionalValue.orEmpty();
		}
	};
}

