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
	class ThinPointer {
		
		Type* valuePtr {nullptr};
		
		CONSTEXPR void freePointer() {
			if (valuePtr != nullptr) {
				delete valuePtr;
				valuePtr = nullptr;
			}
		}
		
	public:
		
		CONSTEXPR ThinPointer(){ }
		
		CONSTEXPR ThinPointer(const Type& value) {
			valuePtr = new Type{value};
		}
		
		CONSTEXPR ThinPointer(ThinPointer& otherPtr) {
			this->operator=(otherPtr);
		}
		
		CONSTEXPR Type& operator*() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return *valuePtr;
		}
		
		CONSTEXPR operator Type() const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return *valuePtr;
		}
		
		CONSTEXPR void operator=(ThinPointer& otherPtr) {
			this->freePointer();
			valuePtr = new Type{*otherPtr};
			otherPtr.freePointer();
		}
		
		CONSTEXPR bool isNull() const {
			return valuePtr == nullptr;
		}
		
		CONSTEXPR bool isNotNull() const {
			return valuePtr != nullptr;
		}
		
		~ThinPointer() {
			this->freePointer();
		}
	};
	
	template <typename Type>
	class Pointer {
		
		std::unique_ptr<Type> valuePtr {nullptr};
		
	public:
		
		CONSTEXPR Pointer() {
			valuePtr = std::unique_ptr<Type>(new Type{});
		}
		
		CONSTEXPR Pointer(const Type& value) {
			valuePtr = std::unique_ptr<Type>(new Type{value});
		}
		
		CONSTEXPR Pointer(Pointer& otherPtr) {
			this->operator=(otherPtr);
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
		
		CONSTEXPR Pointer& operator=(Pointer& otherPtr) {
			valuePtr = std::move(otherPtr.valuePtr);
			return *this;
		}
	};
	
	template <typename Type>
	class Pointer<Type[]> {
		
		std::unique_ptr<Type[]> valuePtr {nullptr};
		std::size_t capacity_ {1};
		
	public:
		
		CONSTEXPR Pointer() {
			this->valuePtr = std::unique_ptr<Type[]>(new Type[capacity_]{});
		}
		
		CONSTEXPR Pointer(const std::size_t capacity) {
			this->capacity_ = capacity;
			this->valuePtr = std::unique_ptr<Type[]>(new Type[capacity_]{});
		}
		
		CONSTEXPR Pointer(std::initializer_list<Type> values) {
			
			this->capacity_ = values.size();
			this->valuePtr = std::unique_ptr<Type[]>(new Type[capacity_]{});
			
			size_t index = 0;
			for (const auto& value: values) {
				valuePtr[index] = value;
				index++;
			}
		}
		
		CONSTEXPR Pointer(Pointer<Type[]>& otherPtr) {
			this->operator=(otherPtr);
		}
		
		CONSTEXPR Pointer(Pointer<Type[]>&& otherPtr) {
			this->operator=(otherPtr);
		}
		
		CONSTEXPR Type& operator[](const std::size_t index) const {
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			return valuePtr[index];
		}
		
		CONSTEXPR Type& at(const std::size_t index) const {
			
			if (index >= capacity_) { throw std::out_of_range("Index out of range"); }
			if (valuePtr == nullptr) { throw std::bad_alloc(); }
			
			return valuePtr[index];
		}
		
		CONSTEXPR std::size_t capacity() const {
			return capacity_;
		}
		
		CONSTEXPR Type* begin() const {
			return &valuePtr[0];
		}
		
		CONSTEXPR Type* end() const {
			return &valuePtr[capacity_];
		}
		
		CONSTEXPR Pointer& operator=(Pointer<Type[]>& otherPtr) {
			
			this->capacity_ = otherPtr.capacity();
			this->valuePtr = std::move(otherPtr.valuePtr);
			
			otherPtr.capacity_ = 0;
			
			return *this;
		}
		
		CONSTEXPR Pointer& operator=(Pointer<Type[]>&& otherPtr) {
			
			this->capacity_ = otherPtr.capacity();
			this->valuePtr = std::move(otherPtr.valuePtr);
			
			otherPtr.capacity_ = 0;
			
			return *this;
		}
		
		CONSTEXPR bool operator==(Pointer<Type[]>& otherPtr) {
			return std::equal(&valuePtr[0], &valuePtr[capacity_], otherPtr.begin());
		}
		
		template <typename Container>
		CONSTEXPR void moveValuesFrom(Container&& container, std::size_t position = 0) {
			std::move(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void moveValuesFrom(std::initializer_list<Type>&& container, std::size_t position = 0) {
			std::move(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		template <typename Container>
		CONSTEXPR void copyValuesFrom(const Container& container, std::size_t position = 0) {
			std::copy(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR void copyValuesFrom(const std::initializer_list<Type> container, std::size_t position = 0) {
			std::copy(std::begin(container), std::end(container), &(this->at(position)));
		}
		
		CONSTEXPR bool isNull() const {
			return valuePtr == nullptr;
		}
		
		CONSTEXPR bool isNotNull() const {
			return valuePtr != nullptr;
		}
	};
}
