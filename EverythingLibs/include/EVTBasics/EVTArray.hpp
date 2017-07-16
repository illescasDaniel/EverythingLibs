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

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <random>
#include <functional>
#include "EVTOptional.hpp"
#include "EVTPointer.hpp"
#include "../EVTObject.hpp"

#if (__cplusplus > 201103L)
	#define CONSTEXPR constexpr
#else
	#define CONSTEXPR
#endif

namespace evt {
	
	namespace internalArrayPrintEVT {
		
		// Extra functions for the "toString()" method
		inline std::string to_string(const std::string& str) { return str; }
		inline std::string to_string(const char chr) { return std::string(1,chr); }
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value,bool>::type>
		inline std::string to_string(const Arithmetic& arithmeticValue) {
			
			if (std::is_same<Arithmetic, bool>::value) {
				return arithmeticValue ? "true" : "false";
			}
			
			return std::to_string(arithmeticValue);
		}
		
		inline std::string to_string(const EVTObject& evtObject) { return evtObject.toString(); }
		/* Place your custom "to_string()" function/s here for other classes, or just inherit from EVTObject. */
	}
	
	// MARK: - Array Class
	template <typename Type, std::size_t initialCapacity = 2>
	class Array: public EVTObject {
		
		// Types and macros
		typedef std::size_t SizeType;
		typedef evt::Pointer<Type[]> Pointer;
		typedef std::initializer_list<Type> InitializerList;
		
		// MARK: - Attributes
		
		Pointer values = {Pointer((initialCapacity > 2) ? initialCapacity : 2)};
		SizeType count_ {0};
		
		// MARK: - Private Functions
		
		CONSTEXPR double sizeOfArrayInMB(const double currentCapacity) const {
			return (sizeof(Type)*(currentCapacity)) / 1000000;
		}
		
		/// Assigns new memory, also updates the new capacity.
		CONSTEXPR void assignMemoryAndCapacityForSize(SizeType newSize, bool forceResize = false) {
			if (forceResize or values.capacity() < newSize) {
				values = Pointer(newSize);
			}
		}
		
		/// Resizes the array to a given size
		CONSTEXPR void resizeValuesToSize(const SizeType newSize, bool move = 0) {
			
			Pointer newValues (newSize);
			
			move ? newValues.moveValuesFrom(values) : newValues.copyValuesFrom(values);
			values = newValues;
		}
		
		/// Replaces the content of the array with other elements
		template <typename Container>
		CONSTEXPR void assignNewElements(const Container& elements) {
			count_ = std::distance(std::begin(elements), std::end(elements));
			assignMemoryAndCapacityForSize(count_);
			values.copyValuesFrom(elements);
		}
		
		template <typename Container>
		CONSTEXPR void assignNewElementsMOVE(Container&& elements) {
			count_ = std::distance(std::begin(elements), std::end(elements));
			assignMemoryAndCapacityForSize(count_);
			values.moveValuesFrom(elements);
		}
		
		template <typename Container>
		Array& appendNewElements(const Container& newElements) {
			
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (values.capacity() >= (count_ + countOfContainer)) {
				values.copyValuesFrom(newElements, count_);
			}
			else if (countOfContainer > 0) {
				
				SizeType newCapacity = countOfContainer + count_;
				
				Pointer newValues (newCapacity);
				
				std::copy(&values[0], &values[count_], &newValues[0]);
				newValues.copyValuesFrom(newElements, count_);
				
				values = newValues;
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& appendNewElementsMOVE(Container&& newElements) {
			
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			
			if (values.capacity() >= (count_ + countOfContainer)) {
				values.moveValuesFrom(newElements, count_);
			}
			else if (countOfContainer > 0) {
				
				SizeType newCapacity = countOfContainer + count_;
				
				Pointer newValues (newCapacity);
				
				std::move(&values[0], &values[count_], &newValues[0]);
				newValues.moveValuesFrom(newElements, count_);
				
				values = newValues;
			}
			
			count_ += countOfContainer;
			
			return *this;
		}
		
		template <typename Container>
		Array& removeElementsFromContainer(const Container& newElements, bool onlyFirstOcurrence = false) {
			
			SizeType elementsFound = 0;
			SizeType countOfContainer = std::distance(std::begin(newElements), std::end(newElements));
			std::unique_ptr<SizeType[]> elementsPosition(new SizeType[countOfContainer]);
			
			auto newElement = std::begin(newElements);
			
			do {
				elementsFound = 0;
				newElement = std::begin(newElements);
				
				for (SizeType index = 0; index < count_; ++index) {
					
					if (values[index] == *newElement) {
						elementsPosition[elementsFound] = index;
						elementsFound += 1;
						++newElement;
					}
					else if (elementsFound != newElements.size()) {
						elementsFound = 0;
						newElement = std::begin(newElements);
					} else { break; }
				}
				
				if (elementsFound == newElements.size()) {
					for (SizeType i = 0; i < newElements.size(); ++i) {
						this->removeAt(elementsPosition[0]);
					}
				}
			} while((elementsFound == newElements.size()) && !onlyFirstOcurrence);
			
			return *this;
		}
		
		CONSTEXPR void checkIfEmpty() const {
			if (count_ == 0) {
				throw std::length_error("Array is empty (lenght == 0)");
			}
		}
		
		CONSTEXPR void checkIfOutOfRange(const SizeType index) const {
			if (index >= count_) {
				throw std::out_of_range("Index out of range");
			}
		}
		
	public:
		
		// MARK: Constructors
		
		CONSTEXPR Array() { }
		CONSTEXPR Array(InitializerList elements) { assignNewElements(elements); }
		CONSTEXPR Array(const Array& otherArray) { (*this) = otherArray; }
		CONSTEXPR Array(Array&& otherArray) { (*this) = otherArray; }
		
		template<typename Container>
		CONSTEXPR Array(const Container& elements) { assignNewElements(elements); }
		
		template <typename Container, typename = typename std::enable_if<!std::is_same<Container, Type>::type>>
		CONSTEXPR Array(Container&& elements) { assignNewElementsMOVE(elements); }
		
		virtual ~Array() {
			values.~Pointer();
		}
		
		// MARK: Capacity
		
		CONSTEXPR SizeType size() const  { return count_; }
		CONSTEXPR SizeType count() const { return count_; }
		CONSTEXPR SizeType capacity() const { return values.capacity(); }
		
		CONSTEXPR bool isEmpty() const { return (count_ == 0); }
		
		// MARK: Manage elements
		
		CONSTEXPR void insertAt(const Type* position, const Type& newElement) {
			
			if (position == &values[count_]) {
				this->append(newElement);
			}
			else if (position == &values[0]) {
				this->insert(newElement, 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(newElement, position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		CONSTEXPR void insertAt(const Type* position, Type&& newElement) {
			
			if (position == &values[count_]) {
				this->append(newElement);
			}
			else if (position == &values[0]) {
				this->insert(newElement, 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(newElement, position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		CONSTEXPR void insert(const Type& newElement, const SizeType index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (values.capacity() == count_) {
				
				SizeType newCapacity = (sizeOfArrayInMB(values.capacity()) < 500) ? (values.capacity() << 2) : (values.capacity() << 1);
				
				Pointer newValues (newCapacity);
				
				std::copy(&values[0], &values[index], &newValues[0]);
				newValues.copyValuesFrom(values, index+1);
				
				values = newValues;
			}
			else {
				std::copy(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = newElement;
			count_ += 1;
		}
		
		CONSTEXPR void insert(Type&& newElement, const SizeType index) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (values.capacity() == count_) {
				
				SizeType newCapacity = (sizeOfArrayInMB(values.capacity()) < 500) ? (values.capacity() << 2) : (values.capacity() << 1);
				
				Pointer newValues (newCapacity);
				
				std::move(&values[0], &values[index], &newValues[0]);
				newValues.moveValuesFrom(values, index+1);
				
				values = newValues;
			}
			else {
				std::move(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = newElement;
			count_ += 1;
		}
		
		/// Replaces the contents with copies of those in the range
		CONSTEXPR void assignRange(SizeType first, SizeType last) {
			
			if (first > last) {
				checkIfOutOfRange(first);
				SizeType auxFirst = last;
				last = first;
				first = auxFirst;
			}
			checkIfOutOfRange(last);
			
			std::copy(&values[first], &values[last+1], &values[0]);
			
			SizeType newCount = last-first + 1;
			
			if (newCount < (count_ / 4.0)) {
				resize(newCount);
			} else {
				count_ = newCount;
			}
		}
		
		CONSTEXPR void append(const Type& newElement) {
			
			if (values.capacity() == count_) {
				resizeValuesToSize((sizeOfArrayInMB(values.capacity()) < 500) ? (values.capacity() << 2) : (values.capacity() << 1));
			}
			values[count_] = newElement;
			count_ += 1;
		}
		
		CONSTEXPR void append(Type&& newElement) {
			
			if (values.capacity() == count_) {
				resizeValuesToSize((sizeOfArrayInMB(values.capacity()) < 500) ? (values.capacity() << 2) : (values.capacity() << 1), 1);
			}
			values[count_] = newElement;
			count_ += 1;
		}
		
		CONSTEXPR void append(InitializerList newElements) { appendNewElements(newElements); }
		CONSTEXPR void appendElements(InitializerList newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		CONSTEXPR void appendElements(const Container& newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		CONSTEXPR void appendElements(Container&& newElements) { appendNewElementsMOVE(newElements); }
		
		/// Only reserves new memory if the new size if bigger than the array capacity
		CONSTEXPR void reserve(const SizeType newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
				return;
			}
			
			if (newSize < count_) {
				count_ = newSize;
			}
			if (newSize > values.capacity()) {
				resizeValuesToSize(newSize);
			}
		}
		
		/// Only reserves new memory if the new size if smaller than the array count
		CONSTEXPR void resize(const SizeType newSize) {
			
			if (newSize == 0 && count_ > 0) {
				removeAll();
				return;
			}
			
			if (newSize < count_) {
				count_ = newSize;
			}
			resizeValuesToSize(newSize);
		}
		
		/// Requests the removal of unused capacity. The new capacity will be the number of elements (count)
		CONSTEXPR bool shrink() {
			
			if (values.capacity() > count_) {
				resizeValuesToSize(count_);
				return true;
			}
			return false;
		}
		
		/// Removes all elements in array, count will be 0, capacity won't be affected
		CONSTEXPR  void clear() {
			removeAll(true);
		}
		
		/// Removes all elements in array, capacity will be 1 if desired
		CONSTEXPR void removeAll(const bool keepCapacity = false) {
			
			if (!keepCapacity) {
				assignMemoryAndCapacityForSize(2, true);
			}
			count_ = 0;
		}
		
		CONSTEXPR void removeAt(const SizeType index, const bool shrinkIfEmpty = true) {
			
			if (index == count_ - 1) {
				removeLast();
				return;
			}
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			checkIfOutOfRange(index);
			
			std::copy(&values[index + 1], &values[count_], &values[index]);
			
			if (count_ - 1 < (values.capacity() / 4.0)) {
				resize(count_ - 1);
			}
			else {
				count_ -= 1;
			}
		}
		
		CONSTEXPR void removeLast(const bool shrinkIfEmpty = true) {
			
			if (count_ == 2 && shrinkIfEmpty) {
				shrink();
			}
			
			checkIfEmpty();
			
			if (count_ - 1 < (values.capacity() / 4.0)) {
				resize(count_ - 1);
			}
			else {
				count_ -= 1;
			}
		}
		
		CONSTEXPR void removeFirst(const bool shrinkIfEmpty = true) {
			removeAt(0, shrinkIfEmpty);
		}
		
		CONSTEXPR void removeSubrange(const SizeType startPosition, const SizeType endPosition, bool lessEqual = true) {
			for (SizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		CONSTEXPR void removeSubrange(std::initializer_list<int> position, bool lessEqual = true) {
			
			SizeType startPosition = *std::begin(position);
			SizeType endPosition = *(std::end(position)-1);
			
			for (SizeType i = startPosition; lessEqual ? (i <= endPosition) : (i < endPosition); ++i) {
				this->removeAt(startPosition);
			}
		}
		
		CONSTEXPR void swap(Array& otherArray) {
			
			Pointer auxValues = this->values;
			SizeType auxCount = this->count_;
			
			this->values = otherArray.values;
			this->count_ = otherArray.count_;
			
			otherArray.values = auxValues;
			otherArray.count_ = auxCount;
		}
		
		template <typename Container>
		CONSTEXPR Array& removeElements(const Container& newElements, bool onlyFirstOcurrence = false) {
			return removeElementsFromContainer(newElements, onlyFirstOcurrence);
		}
		
		CONSTEXPR Array& removeElements(InitializerList newElements, bool onlyFirstOcurrence = false) {
			return removeElementsFromContainer(newElements, onlyFirstOcurrence);
		}
		
		template <typename Container>
		CONSTEXPR void swap(Container& container) {
			Array otherArray(container);
			swap(otherArray);
			container = Array::to<Container>(otherArray);
		}
		
		CONSTEXPR bool contains(const Type& element) const {
			for (const auto& elm: (*this)) {
				if (element == elm) { return true; }
			}
			return false;
		}
		
		/// Returns the index of the first ocurrence of the element. Last position if the element isn't found
		CONSTEXPR SizeType find(const Type& element) const {
			return (std::find(&values[0], &values[count_], element) - &values[0]);
		}
		
		/// Returns an Array of positions with all the ocurrences of the element
		Array<SizeType> findAll(const Type& element) const {
			
			Array<SizeType> positions;
			SizeType position = 0;
			
			for (const auto& value: (*this)) {
				if (element == value) {
					positions.append(position);
				}
				position += 1;
			}
			
			return positions;
		}
		
		std::string toString() const override {
			
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			SizeType position = 0;
			
			for (const Type& value: *this) {
				output += [&] {
					
					#if (__cplusplus >= 201406L)
					
						if CONSTEXPR (std::is_same<Type, std::string>::value) {
							return ("\"" + evt::internalArrayPrintEVT::to_string(value) + "\"");
						} else if CONSTEXPR  (std::is_same<Type, char>::value) {
							return ("\'" + evt::internalArrayPrintEVT::to_string(value) + "\'");
						} else if CONSTEXPR (std::is_arithmetic<Type>::value || std::is_same<Type, EVTObject>::value || std::is_base_of<EVTObject, Type>::value) {
							return evt::internalArrayPrintEVT::to_string(value);
						}
						return std::string("Object");
					
					#else
						if (typeid(value) == typeid(std::string)) {
							return ("\"" + evt::internalArrayPrintEVT::to_string(value) + "\"");
						} else if (typeid(value) == typeid(char)) {
							return ("\'" + evt::internalArrayPrintEVT::to_string(value) + "\'");
						}
						return evt::internalArrayPrintEVT::to_string(value);
					
					#endif
				}();
				
				if (position+1 < count_) {
					output += ", ";
				}
				
				position += 1;
			}
			
			output += "]";
			
			return output;
		}
		
		/// Converts Array to other types
		template <typename Container>
		CONSTEXPR static Container to(const Array& elements) {
			Container cont(elements.count());
			std::copy(std::begin(elements), std::end(elements), std::begin(cont));
			return cont;
		}
		
		template <typename Container>
		CONSTEXPR static Container to(Array&& elements) {
			Container cont(elements.count());
			std::move(std::begin(elements), std::end(elements), std::begin(cont));
			return cont;
		}
		
		template <typename Container>
		CONSTEXPR bool equal(const Container& elements) const {
			return (*this == elements);
		}
		
		CONSTEXPR bool equal(InitializerList elements) const {
			return this->operator==(elements);
		}
		
		CONSTEXPR Array filter(const std::function<bool(const Type&)>& filterFunction) const {
			Array filteredArray;
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					filteredArray.append(element);
				}
			}
			return filteredArray;
		}
		
		CONSTEXPR Optional<Type> first(const std::function<bool(const Type&)>& filterFunction) const {
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					return element;
				}
			}
			return nullptr;
		}
		
		CONSTEXPR Optional<Type> last(const std::function<bool(const Type&)>& filterFunction) const {
			
			Optional<Type> optElement;
			
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					optElement = element;
				}
			}
			return optElement;
		}
		
		CONSTEXPR Optional<Type> at(const SizeType index) const {
			if (index >= count_) {
				return nullptr;
			} else {
				return values[index];
			}
		}
		
		// MARK: Operators overload
		
		CONSTEXPR Type& operator[](const SizeType index) {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		CONSTEXPR const Type& operator[](const SizeType index) const {
			checkIfOutOfRange(index);
			return values[index];
		}
		
		/// Removes a subrange of elements
		template <typename Container>
		CONSTEXPR Array& operator-=(const Container& newElements) {
			return removeElements(newElements);
		}
		
		CONSTEXPR Array& operator-=(InitializerList newElements) {
			return removeElements(newElements);
		}
		
		/// Returns a subrange of elements
		template <typename Container>
		CONSTEXPR Array operator-(const Container& newElements) const {
			Array otherArray(*this);
			otherArray -= newElements;
			return otherArray;
		}
		
		template <typename Container>
		CONSTEXPR Array operator+(const Container& newElements) const {
			Array otherArray(*this);
			otherArray += newElements;
			return otherArray;
		}
		
		template <typename Container>
		CONSTEXPR Array& operator+=(const Container& newElements) {
			return appendNewElements(newElements);
		}
		
		template <typename Container>
		CONSTEXPR Array& operator+=(Container&& newElements) {
			return appendNewElementsMOVE(newElements);
		}
		
		CONSTEXPR Array& operator+=(InitializerList newElements) {
			return appendNewElements(newElements);
		}
		
		template <typename Container>
		CONSTEXPR bool operator==(const Container& elements) const {
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			if (count_ != countOfContainer) { return false; }
			return std::equal(&values[0], &values[count_], std::begin(elements));
		}
		
		template <typename Container>
		CONSTEXPR bool operator!=(const Container& elements) const {
			return !( (*this) == elements );
		}
		
		/// Returns true if the contents of the array are lexicographically less than the contents of the container
		template <typename Container>
		CONSTEXPR bool operator<(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement < *containerElement;
				}
			}
			
			return count_ < countOfContainer;
		}
		
		template <typename Container>
		CONSTEXPR bool operator<=(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement < *containerElement;
				}
			}
			
			return count_ <= countOfContainer;
		}
		
		/// Returns true if the contents of the array are lexicographically greater than the contents of the container
		template <typename Container>
		CONSTEXPR bool operator>(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement > *containerElement;
				}
			}
			
			return count_ > countOfContainer;
		}
		
		template <typename Container>
		CONSTEXPR bool operator>=(const Container& elements) {
			
			SizeType countOfContainer = std::distance(std::begin(elements), std::end(elements));
			SizeType smallerSize = (count_ < countOfContainer) ? count_ : countOfContainer;
			
			auto arrayElement = std::begin(*this);
			auto containerElement = std::begin(elements);
			
			for (SizeType i = 0; i < smallerSize; ++i, ++arrayElement, ++containerElement) {
				if (*arrayElement != *containerElement) {
					return *arrayElement > *containerElement;
				}
			}
			
			return count_ >= countOfContainer;
		}
		
		Array& operator=(const Array& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count_;
				
				assignMemoryAndCapacityForSize(otherArray.capacity());
				values.copyValuesFrom(otherArray);
			}
			
			return *this;
		}
		
		CONSTEXPR Array& operator=(Array&& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count_;
				
				assignMemoryAndCapacityForSize(otherArray.capacity());
				values.moveValuesFrom(otherArray);
			}
			
			return *this;
		}
		
		CONSTEXPR void moveFrom(Array&& otherArray) {
			
			if (this != &otherArray) {
				values = otherArray.values;
				count_ = otherArray.count_;
				
				otherArray.count_ = 0;
			}
		}
		
		// MARK: Shuffle
		
		void shuffle() {
			if (this->isEmpty()) { return; }
			
			#ifdef __APPLE__
				std::mt19937_64 rng(arc4random());
			#else
				std::random_device rd;
				std::mt19937_64 rng(rd());
			#endif
			
			std::shuffle(&values[0], &values[count_], rng);
		}
		
		CONSTEXPR Array shuffled() const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.shuffle();
			
			return otherArray;
		}
		
		// MARK: Sort
		
		CONSTEXPR void sort(const std::function<bool(Type&,Type&)>& compareFunction = std::less_equal<Type>()) {
			std::sort(&values[0], &values[count_], compareFunction);
		}
		
		CONSTEXPR Array sorted(const std::function<bool(Type&,Type&)>& compareFunction = std::less_equal<Type>()) const {
			
			if (this->isEmpty()) {
				return *this;
			}
			
			Array otherArray(*this);
			otherArray.sort(compareFunction);
			
			return otherArray;
		}
		
		// MARK: Positions
		
		CONSTEXPR Type* begin() const {
			return values.begin();
		}
		
		CONSTEXPR Type* end() const {
			return &values[count_];
		}
		
		CONSTEXPR Type& first() {
			checkIfEmpty();
			return values[0];
		}
		
		CONSTEXPR Type& last() {
			checkIfEmpty();
			return values[count_-1];
		}
		
		CONSTEXPR const Type& first() const {
			checkIfEmpty();
			return values[0];
		}
		
		CONSTEXPR const Type& last() const {
			checkIfEmpty();
			return values[count_-1];
		}
	};
}
