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
#include <utility>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	template <typename Type>
	class RawPointer {
		
		Type* valuePtr {new Type{}};
		
		CONSTEXPR void freePointer() {
			if (valuePtr != nullptr) {
				delete valuePtr;
				valuePtr = nullptr;
			}
		}
		
		CONSTEXPR void copyAssignFrom(const RawPointer& otherPtr) {
			if (this != &otherPtr && otherPtr.isNotNull()) {
				this->freePointer();
				valuePtr = new Type{*otherPtr.valuePtr};
			}
		}
		
	public:
		
		CONSTEXPR RawPointer(){ }
		
		CONSTEXPR RawPointer(const Type& value) {
			valuePtr = new Type{value};
		}
		
		CONSTEXPR RawPointer(RawPointer& otherPtr) {
			copyAssignFrom(otherPtr);
		}
		
		CONSTEXPR RawPointer(RawPointer&& otherPtr) {
			this->operator=(std::move(otherPtr));
		}
		
		CONSTEXPR RawPointer operator=(const RawPointer& otherPtr) {
			copyAssignFrom(otherPtr);
			return *this;
		}
		
		CONSTEXPR RawPointer operator=(RawPointer&& otherPtr) {
			
			if (this != &otherPtr && otherPtr.isNotNull()) {
				this->freePointer();
				valuePtr = new Type{*otherPtr.valuePtr};
				otherPtr.freePointer();
			}
			
			return *this;
		}
		
		CONSTEXPR Type& operator*() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return *valuePtr;
		}
		
		CONSTEXPR operator Type() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return *valuePtr;
		}
		
		CONSTEXPR bool isNull() const {
			return valuePtr == nullptr;
		}
		
		CONSTEXPR bool isNotNull() const {
			return valuePtr != nullptr;
		}
		
		~RawPointer() {
			this->freePointer();
		}
	};
	
	
	template <typename Type>
	class RawPointer<Type[]> {
		
		std::size_t capacity_ {1};
		Type* valuePtr {new Type[capacity_]{}};
		
		void freePointer() {
			if (valuePtr != nullptr) {
				delete[] valuePtr;
				valuePtr = nullptr;
			}
		}
		
	private:
		CONSTEXPR void assignMemoryForCapacity(std::size_t capacity) {
			if (capacity > 0) {
				this->freePointer();
				this->capacity_ = capacity;
				this->valuePtr = new Type[capacity_]{};
			}
		}
		
		CONSTEXPR void copyPointerValuesFrom(const RawPointer<Type[]>& otherPtr) {
			if (otherPtr.capacity() != 0 && this != &otherPtr) {
				assignMemoryForCapacity(otherPtr.capacity_);
				std::copy(std::begin(otherPtr), std::end(otherPtr), &valuePtr[0]);
			}
		}
		
	public:
		
		CONSTEXPR RawPointer() {}
		
		CONSTEXPR RawPointer(const std::size_t capacity) {
			assignMemoryForCapacity(capacity);
		}
		
		CONSTEXPR RawPointer(std::initializer_list<Type> values) {
			
			assignMemoryForCapacity(values.size());
			
			size_t index = 0;
			for (const auto& value: values) {
				valuePtr[index] = value;
				index++;
			}
		}
		
		CONSTEXPR RawPointer(RawPointer<Type[]>&& otherPtr) {
			this->operator=(std::move(otherPtr));
		}
		
		CONSTEXPR RawPointer(const RawPointer<Type[]>& otherPtr) {
			copyPointerValuesFrom(otherPtr);
		}
		
		CONSTEXPR RawPointer<Type[]>& operator=(const RawPointer<Type[]>& otherPtr) {
			copyPointerValuesFrom(otherPtr);
			return *this;
		}
		
		CONSTEXPR RawPointer<Type[]>& operator=(RawPointer<Type[]>&& otherPtr)  {
			
			if (otherPtr.capacity() != 0 && this != &otherPtr) {
				assignMemoryForCapacity(otherPtr.capacity_);
				std::move(std::begin(otherPtr), std::end(otherPtr), &valuePtr[0]);
				otherPtr.freePointer();
			}
			return *this;
		}

		CONSTEXPR Type& operator[](const std::size_t index) const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return valuePtr[index];
		}
		
		CONSTEXPR Type& at(const std::size_t index) const {
			
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			if (index >= capacity_) { throw std::out_of_range("Index out of range"); }
			
			return valuePtr[index];
		}
		
		CONSTEXPR std::size_t capacity() const noexcept {
			return capacity_;
		}
		
		CONSTEXPR std::size_t size() const noexcept {
			return capacity_;
		}
		
		CONSTEXPR Type* begin() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return &valuePtr[0];
		}
		
		CONSTEXPR Type* end() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return &valuePtr[capacity_];
		}
		
		CONSTEXPR bool operator==(RawPointer<Type[]>& otherPtr) {
			return std::equal(&valuePtr[0], &valuePtr[capacity_], otherPtr.begin());
		}
		
		template <typename Container>
		CONSTEXPR void moveValuesFrom(Container&& container, std::size_t position = 0) {
			std::move(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void moveValuesFrom(std::initializer_list<Type>&& container, std::size_t position = 0) {
			std::move(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void moveValuesFrom(RawPointer<Type[]>&& container, std::size_t position = 0) {
			std::move(container.begin(), container.end(), &(this->at(position)));
		}
		
		template <typename Container>
		CONSTEXPR void copyValuesFrom(const Container& container, std::size_t position = 0) {
			std::copy(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void copyValuesFrom(const std::initializer_list<Type> container, std::size_t position = 0) {
			std::copy(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void copyValuesFrom(const RawPointer<Type[]>& container, std::size_t position = 0) {
			std::copy(container.begin(), container.end(), &(this->at(position)));
		}
		
		CONSTEXPR bool isNull() const noexcept {
			return valuePtr == nullptr;
		}
		
		CONSTEXPR bool isNotNull() const noexcept {
			return valuePtr != nullptr;
		}
		
		~RawPointer() {
			freePointer();
		}
	};
}
