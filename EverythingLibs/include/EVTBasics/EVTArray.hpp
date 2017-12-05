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
#include "EVTRawPointer.hpp"
#include "../EVTProtocols.hpp"

#if (__cplusplus > 201103L)
	#define CONSTEXPR constexpr
#else
	#define CONSTEXPR
#endif

namespace evt {
	
	namespace ArrayPrint {
		
		// Extra functions for the "toString()" method
		inline std::string to_string(const std::string& str) noexcept { return str; }
		inline std::string to_string(const char chr) { return std::string(1,chr); }
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value,bool>::type>
		inline std::string to_string(const Arithmetic& arithmeticValue) {
			if (std::is_same<Arithmetic, bool>::value) {
				return arithmeticValue ? "true" : "false";
			}
			return std::to_string(arithmeticValue);
		}
		
		inline std::string to_string(const protocols::CustomStringConvertible& object) { return object.toString(); }
		/* Place your custom "to_string()" function/s here for other classes, or just inherit from EVTObject. */
	}
	
	// MARK: - Array Class
	template <typename Type>
	class Array {
		
		// Types and macros
		typedef std::size_t SizeType;
		typedef evt::RawPointer<Type[]> Pointer;
		typedef std::initializer_list<Type> InitializerList;
		
		// MARK: - Attributes
		
		Pointer values;
		SizeType count_ {0};
		
		// MARK: - Private Functions
		
		/// Assigns new memory, also updates the new capacity.
		CONSTEXPR void assignMemoryAndCapacityForSize(SizeType newSize, bool forceResize = false) {
			if (forceResize or values.capacity() < newSize) {
				values = std::move(Pointer(newSize));
			}
		}
		
		/// Resizes the array to a given size
		CONSTEXPR void resizeValuesToSize(const SizeType newSize, bool move = 0) {
			
			Pointer newValues (newSize);
			
			move ? newValues.moveValuesFrom(values) : newValues.copyValuesFrom(values);
			values = std::move(newValues);
		}
		
		template <typename MagicContainer>
		CONSTEXPR void assignNewMagicElements(MagicContainer&& elements, const std::size_t initialCapacity = 2) {
			count_ = std::distance(std::begin(elements), std::end(elements));
			assignMemoryAndCapacityForSize((count_ > initialCapacity) ? count_ : initialCapacity);
			assignNewElements(std::forward<MagicContainer>(elements));
		}
		/// Replaces the content of the array with other elements
		template <typename Container>
		CONSTEXPR void assignNewElements(const Container& elements) {
			values.copyValuesFrom(elements);
		}
		
		template <typename Container>
		CONSTEXPR void assignNewElements(Container&& elements) {
			values.moveValuesFrom(std::move(elements));
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
				
				values = std::move(newValues);
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
				
				values = std::move(newValues);
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
		
		void assignArrayWithOptionalInitialCapacity(const Array& otherArray, const size_t initialCapacity = 2) {
			if (this != &otherArray) {
				count_ = otherArray.count();
				std::size_t capacity = (otherArray.count() > initialCapacity) ? otherArray.count() : initialCapacity;
				assignMemoryAndCapacityForSize(capacity);
				values.copyValuesFrom(otherArray);
			}
		}
		
		void assignArrayWithOptionalInitialCapacity(Array&& otherArray, const size_t initialCapacity = 2) {
			if (this != &otherArray) {
				count_ = otherArray.count();
				std::size_t capacity = (otherArray.count() > initialCapacity) ? otherArray.count() : initialCapacity;
				assignMemoryAndCapacityForSize(capacity);
				values.moveValuesFrom(std::move(otherArray));
			}
		}
		
	public:
		
		// MARK: Constructors
		
		CONSTEXPR Array() {}
		CONSTEXPR Array(const int initialCapacity) { assignMemoryAndCapacityForSize(initialCapacity, true); }
		CONSTEXPR Array(std::size_t initialCapacity) { assignMemoryAndCapacityForSize(initialCapacity, true); }
		CONSTEXPR Array(InitializerList&& elements, std::size_t initialCapacity = 2) { assignNewMagicElements(elements, initialCapacity); }
		CONSTEXPR Array(const Array& otherArray, std::size_t initialCapacity = 2) { assignArrayWithOptionalInitialCapacity(otherArray, initialCapacity); }
		CONSTEXPR Array(Array&& otherArray, std::size_t initialCapacity = 2) { assignArrayWithOptionalInitialCapacity(std::move(otherArray), initialCapacity); }
		CONSTEXPR Array(const SizeType count, const Type& initialValue) {
			assignMemoryAndCapacityForSize(count);
			this->count_ = count;
			Type n {initialValue};
			std::generate(this->begin(), this->end(), [&]{ return n++; });
		}
		
		template <typename Container, typename = typename std::enable_if<
		!std::is_same<Container,Array>::value &&
		!std::is_same<Container,Type>::value &&
		!std::is_arithmetic<Container>::value>::type>
		CONSTEXPR Array(Container&& elements, SizeType initialCapacity = 2) { assignNewMagicElements(elements, initialCapacity); }
		
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
				this->append(std::move(newElement));
			}
			else if (position == &values[0]) {
				this->insert(std::move(newElement), 0);
			}
			else if (position > &values[0] && position < &values[count_]) {
				this->insert(std::move(newElement), position - &values[0]);
			}
			else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		CONSTEXPR void insert(const Type& newElement, const SizeType index, const SizeType capacityResizeFactor = 2) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(newElement);
				return;
			}
			
			if (values.capacity() == count_) {
				
				SizeType newCapacity = values.capacity() * capacityResizeFactor;
				
				Pointer newValues (newCapacity);
				
				std::copy(&values[0], &values[index], &newValues[0]);
				newValues.copyValuesFrom(values, index+1);
				
				values = std::move(newValues);
			}
			else {
				std::copy(&values[index], &values[count_], &values[index + 1]);
			}
			
			values[index] = newElement;
			count_ += 1;
		}
		
		CONSTEXPR void insert(Type&& newElement, const SizeType index, const SizeType capacityResizeFactor = 2) {
			
			if (index != 0) {
				checkIfOutOfRange(index);
			}
			else if (index == count_ || this->isEmpty()) {
				this->append(std::move(newElement));
				return;
			}
			
			if (values.capacity() == count_) {
				
				SizeType newCapacity = values.capacity() * capacityResizeFactor;
				
				Pointer newValues (newCapacity);
				
				std::move(&values[0], &values[index], &newValues[0]);
				newValues.moveValuesFrom(values, index+1);
				
				values = std::move(newValues);
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
		
		CONSTEXPR void append(const Type& newElement, const SizeType capacityResizeFactor = 2) {
			
			if (values.capacity() == count_) {
				resizeValuesToSize(values.capacity() * capacityResizeFactor);
			}
			values[count_] = newElement;
			count_ += 1;
		}
		
		CONSTEXPR void append(Type&& newElement, const SizeType capacityResizeFactor = 2) {
			
			if (values.capacity() == count_) {
				resizeValuesToSize(values.capacity() * capacityResizeFactor, true);
			}
			values[count_] = std::move(newElement);
			count_ += 1;
		}
		
		CONSTEXPR void append(InitializerList newElements) { appendNewElements(newElements); }
		CONSTEXPR void appendElements(InitializerList newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		CONSTEXPR void appendElements(const Container& newElements) { appendNewElements(newElements); }
		
		template<typename Container>
		CONSTEXPR void appendElements(Container&& newElements) { appendNewElementsMOVE(std::move(newElements)); }
		
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
			
			Pointer auxValues = std::move(this->values);
			this->values = std::move(otherArray.values);
			otherArray.values = std::move(auxValues);
			
			std::swap(this->count_, otherArray.count_);
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
			this->swap(otherArray);
			container = Array::to<Container>(otherArray);
		}
		
		CONSTEXPR bool contains(const Type& element) const {
			for (const Type& elm: (*this)) {
				if (element == elm) { return true; }
			}
			return false;
		}
		 
		/// Returns the index of the first ocurrence of the element. Last position if the element isn't found
		CONSTEXPR SizeType find(const Type& element) const {
			return (std::find(&values[0], &values[count_], element) - &values[0]);
		}
		
		CONSTEXPR SizeType findIf(std::function<bool(const Type&)> findFunction) const {
			return (std::find_if(&values[0], &values[count_], findFunction) - &values[0]);
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
		 
		std::string toString() const {
			
			if (this->isEmpty()) {
				return "[]";
			}
			
			std::string output = "[";
			SizeType position = 0;
			
			for (const Type& value: *this) {
				output += [&] {
					
					#if (__cplusplus >= 201406L) && defined(__clang__)
					
						if constexpr (std::is_same<Type, std::string>::value) {
							return ("\"" + evt::ArrayPrint::to_string(value) + "\"");
						} else if constexpr (std::is_same<Type, char>::value) {
							return ("\'" + evt::ArrayPrint::to_string(value) + "\'");
						} else if constexpr (std::is_arithmetic<Type>::value || std::is_same<Type, protocols::CustomStringConvertible>::value || std::is_base_of<protocols::CustomStringConvertible, Type>::value) {
							return evt::ArrayPrint::to_string(value);
						}
						return std::string("Object");
					
					#else
						if (typeid(value) == typeid(std::string)) {
							return ("\"" + evt::ArrayPrint::to_string(value) + "\"");
						} else if (typeid(value) == typeid(char)) {
							return ("\'" + evt::ArrayPrint::to_string(value) + "\'");
						}
						return evt::ArrayPrint::to_string(value);
					
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
		
		friend std::ostream& operator<<(std::ostream& os, const Array& object) noexcept {
			return os << object.toString();
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
		
		CONSTEXPR bool equal(InitializerList&& elements) const {
			return this->operator==(elements);
		}
		
		CONSTEXPR Array filter(std::function<bool(const Type&)> filterFunction) const {
			Array filteredArray;
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					filteredArray.append(element);
				}
			}
			return filteredArray;
		}
			
		template <typename MapType>
		Array<MapType> map(std::function<MapType(const Type&)> mapFunctor) const {
			Array<MapType> mappedArray(this->count());
			for (const auto& element: *this) {
				mappedArray.append(mapFunctor(element));
			}
			return mappedArray;
		}
			
		Array map(std::function<Type(const Type&)> mapFunctor) const {
			return this->map<Type>(mapFunctor);
		}
			
		template <typename ReduceType>
		ReduceType reduce(std::function<ReduceType(const ReduceType result, const Type&)> reduceFunctor, ReduceType initialValue = ReduceType()) const {
			ReduceType reducedArrayValue{initialValue};
			for (const auto& element: *this) {
				reducedArrayValue = reduceFunctor(reducedArrayValue, element);
			}
			return reducedArrayValue;
		}
		
		CONSTEXPR size_t sum() const {
			return this->reduce<size_t>([](const size_t result, const Type& value){ return result + value; });
		}
		
		CONSTEXPR double mean() const {
			return double(this->sum()) / double(this->count());
		}
			
		std::string joinToString(const std::string& separator = ", ",
								 const std::string& prefix = "",
								 const std::string& postfix = "",
								 const size_t limit = -1,
								 const std::string& truncated = "...",
								 std::function<const std::string(const Type&)> transform = [](const Type& element) { return ArrayPrint::to_string(element); }) const {
			
			std::string output = "";
			output += prefix;
			
			std::size_t index = 0;
			for (const auto& element: (*this)) {
				
				if (index < limit || limit == 0) {
					if (index+1 < this->count()) {
						output += transform(element) + separator;
					} else {
						output += transform(element);
					}
				} else {
					output += truncated;
					output += postfix;
					return output;
				}
				index += 1;
			}
			
			output += postfix;
			return output;
		}
		
		std::string joinToString(std::function<const std::string(const Type&)> transform) const {
			return this->joinToString(", ", "", "", -1, "...", transform);
		}
		
		std::string joinToString(const std::string& separator, std::function<const std::string(const Type&)> transform) const {
			return this->joinToString(separator, "", "", -1, "...", transform);
		}
			
		CONSTEXPR Optional<Type> first(std::function<bool(const Type&)> filterFunction) const {
			for (const auto& element: *this) {
				if (filterFunction(element)) {
					return element;
				}
			}
			return nullptr;
		}
		
		Optional<Type> last(std::function<bool(const Type&)> filterFunction) const {
			
			Optional<Type> optElement;
			
			for (const Type& element: *this) {
				if (filterFunction(element)) {
					optElement = element;
				}
			}
			
			return optElement;
		}
		
		CONSTEXPR SizeType countOf(std::function<bool(const Type&)> countOfFunction) const {
			return std::count_if(this->begin(), this->end(), countOfFunction);
		}
		
		CONSTEXPR SizeType countOf(const Type& value) const {
			return std::count(this->begin(), this->end(), value);
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
			return appendNewElementsMOVE(std::move(newElements));
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

		Array& operator=(Array&& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count();
				assignMemoryAndCapacityForSize(otherArray.capacity());
				values.moveValuesFrom(std::move(otherArray));
			}
			
			return *this;
		}
		
		Array& operator=(const Array& otherArray) {
			
			if (this != &otherArray) {
				count_ = otherArray.count();
				assignMemoryAndCapacityForSize(otherArray.capacity());
				values.copyValuesFrom(otherArray);
			}
			return *this;
		}
		
		CONSTEXPR void moveFrom(Array&& otherArray) {
			
			if (this != &otherArray) {
				values = std::move(otherArray.values);
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
		
		CONSTEXPR void sort(std::function<bool(Type&,Type&)> compareFunction = std::less_equal<Type>()) {
			std::sort(&values[0], &values[count_], compareFunction);
		}
		
		CONSTEXPR Array sorted(std::function<bool(Type&,Type&)> compareFunction = std::less_equal<Type>()) const {
			
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

#undef CONSTEXPR
