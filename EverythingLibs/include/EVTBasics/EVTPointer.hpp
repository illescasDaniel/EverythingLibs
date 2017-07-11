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

#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <utility>
#include <algorithm>

namespace evt {
	
	template <typename Type>
	class Pointer {
		
		std::unique_ptr<Type> value_ptr;
		
	public:
		
		Pointer() {
			value_ptr = std::unique_ptr<Type>(new Type{});
		}
		
		Pointer(const Type& value) {
			value_ptr = std::unique_ptr<Type>(new Type{value});
		}
		
		Type& operator*() {
			return *value_ptr;
		}
		
		inline operator Type() const {
			return *value_ptr;
		}
		
		void operator=(Pointer& otherPtr) {
			value_ptr = std::move(otherPtr.value_ptr);
		}
	};
	
	template <typename Type>
	class Pointer<Type[]> {
		
		std::unique_ptr<Type[]> value_ptr;
		std::size_t capacity_ = 1;
		
	public:
		
		Pointer() {
			this->value_ptr = std::unique_ptr<Type[]>(new Type[capacity_]);
		}
		
		explicit Pointer(const std::size_t newCapacity) {
			this->capacity_ = newCapacity;
			this->value_ptr = std::unique_ptr<Type[]>(new Type[capacity_]);
		}
		
		Pointer(std::initializer_list<Type> values) {
			
			this->capacity_ = values.size();
			this->value_ptr = std::unique_ptr<Type[]>(new Type[capacity_]);
			
			size_t index = 0;
			for (const auto& value: values) {
				value_ptr[index] = value;
				index++;
			}
		}
		
		inline Type& operator[](const std::size_t index) {
			
			if (index >= capacity_) { throw std::out_of_range("Index out of range"); }
			if (value_ptr == nullptr) { throw std::bad_alloc(); }
			
			return value_ptr[index];
		}
		
		inline const Type& operator[](const std::size_t index) const {
			
			if (index >= capacity_) { throw std::out_of_range("Index out of range"); }
			if (value_ptr == nullptr) { throw std::bad_alloc(); }
			
			return value_ptr[index];
		}
		
		inline std::size_t capacity() const {
			return capacity_;
		}
		
		inline Type* begin() const {
			return &value_ptr[0];
		}
		
		inline Type* end() const {
			return &value_ptr[capacity_];
		}
		
		void operator=(Pointer<Type[]>& otherPtr) {
			
			this->capacity_ = otherPtr.capacity();
			this->value_ptr = std::move(otherPtr.value_ptr);
			
			otherPtr.capacity_ = 0;
		}
		
		void operator=(Pointer<Type[]>&& otherPtr) {
			
			this->capacity_ = otherPtr.capacity();
			this->value_ptr = std::move(otherPtr.value_ptr);
			
			otherPtr.capacity_ = 0;
		}
		
		template <typename Container>
		void moveValuesFrom(Container&& container) {
			std::move(std::begin(container), std::end(container), this->begin());
		}
		
		void moveValuesFrom(std::initializer_list<Type>&& container) {
			std::move(std::begin(container), std::end(container), this->begin());
		}
		
		template <typename Container>
		void copyValuesFrom(const Container& container) {
			std::copy(std::begin(container), std::end(container), this->begin());
		}
		
		void copyValuesFrom(const std::initializer_list<Type> container) {
			std::copy(std::begin(container), std::end(container), this->begin());
		}
	};
}
