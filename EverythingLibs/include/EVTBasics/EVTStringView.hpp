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

#include <string_view>
#include "../EVTObject.hpp"

namespace evt {
	
	template <typename CharType, typename = typename std::enable_if<
			std::is_same<CharType, char>::value ||
			std::is_same<CharType, wchar_t>::value ||
			std::is_same<CharType, char16_t>::value ||
			std::is_same<CharType, char32_t>::value>::type,
	typename Traits = std::char_traits<CharType>>
	
	class BasicStringView: public std::basic_string_view<CharType>, public Object {
		
		typedef std::basic_string_view<CharType> super;
		
		// Will hide these methods from the public access
		constexpr bool empty() const noexcept { return super::empty(); }
		constexpr std::size_t max_size() const noexcept { return super::max_size(); }
		constexpr const CharType& front() const { return super::front(); }
		constexpr const CharType& back() const { return super::back(); }
		constexpr void remove_prefix(std::size_t n) { return super::remove_prefix(n); }
		constexpr void remove_suffix(std::size_t n) { return super::remove_suffix(n); }
		static constexpr std::size_t npos = super::npos;
		constexpr BasicStringView substr(std::size_t pos = 0, std::size_t count = super::npos ) const { return super::substr(pos, count); }
		
	public:	
		constexpr BasicStringView() noexcept : super() {}
		constexpr BasicStringView(const std::basic_string_view<CharType>& other) noexcept : super(other) {}
		constexpr BasicStringView(const std::string& other) noexcept : super(other) {}
		constexpr BasicStringView(const CharType* str, std::size_t count) noexcept : super(str, count) {}
		constexpr BasicStringView(const CharType* str) noexcept : super(str) {}
		
		constexpr bool isEmpty() const noexcept { return this->empty(); }
		constexpr std::size_t count() const noexcept { return this->length(); }
		constexpr std::size_t maxSize() const noexcept { return this->max_size(); }
		constexpr const CharType& operator[](std::size_t position) const { return this->at(position); }
		constexpr const CharType& first() const { return (*this)[0]; }
		constexpr const CharType& last() const { return (*this)[this->count()-1]; }
		constexpr void removePrefix(std::size_t prefixSize) { return this->remove_prefix(prefixSize); }
		constexpr void removeSuffix(std::size_t suffixSize) { return this->remove_suffix(suffixSize); }
		constexpr std::size_t findFirstOf(const BasicStringView& str, std::size_t position = 0) const noexcept { return this->find_first_of(str, position); }
		constexpr std::size_t findFirstNotOf(const BasicStringView& str, std::size_t position = 0) const noexcept { return this->find_first_not_of(str, position); }
		constexpr std::size_t findLastOf(const BasicStringView& str, std::size_t position = BasicStringView::nullPosition) const noexcept { return this->find_last_of(str, position); }
		constexpr std::size_t findLastNotOf(const BasicStringView& str, std::size_t position = BasicStringView::nullPosition) const noexcept { return this->find_last_not_of(str, position); }
		constexpr BasicStringView subString(std::size_t position = 0, std::size_t count = BasicStringView::nullPosition ) const { return this->substr(position, count); }
		
		// Don't try to assign it to another StringView, for whatever reason it doesn't seem to 'work' fine
		constexpr BasicStringView operator+(const BasicStringView& otherStr) const {
			return BasicStringView(std::string(*this) + std::string(otherStr));
		}
		
		static constexpr std::size_t nullPosition = super::npos;
		
		enum TrimMode { trimStart, trimEnd, trimBoth };
		constexpr void trim(const TrimMode& trimMode = trimBoth) {
			
			switch (trimMode) {
					
				case trimStart:
					if (std::size_t position = this->findFirstNotOf(" "); position != BasicStringView::nullPosition) {
						this->removePrefix(position);
					}
					break;
					
				case trimEnd:
					if (std::size_t position = this->count() - this->findLastOf(" ") + 1; position != BasicStringView::nullPosition) {
						this->removeSuffix(position);
					}
					break;
					
				case trimBoth:
					if (std::size_t position = this->findFirstNotOf(" "); position != BasicStringView::nullPosition) {
						this->removePrefix(position);
					}
					if (std::size_t position = this->count() - this->findLastOf(" ") + 1; position != BasicStringView::nullPosition) {
						this->removeSuffix(position);
					}
					break;
			}
		}
		
		constexpr BasicStringView trimmed(const TrimMode& trimMode = trimBoth) const {
			BasicStringView aux(*this);
			aux.trim(trimMode);
			return aux;
		}
		
		constexpr bool contains(const BasicStringView& str, size_t pos = 0) const {
			return this->find(str, pos) != BasicStringView::nullPosition;
		}
		
		constexpr std::size_t reverseFind(const BasicStringView& str, std::size_t position = BasicStringView::nullPosition) {
			return this->find(str, position);
		}
		
		std::string toString() const override {
			
			const CharType* str = this->data();
			std::string outputStr;
			for (std::size_t i = 0; i < this->size(); i++) {
				outputStr += str[i];
			}
			return outputStr;
		}
		
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		BasicStringView operator*(Type number) const {
			std::string originalStr(*this);
			std::string concatenatedStr;
			for (Type i = 0; i < number; i++) {
				concatenatedStr += originalStr;
			}
			return BasicStringView(concatenatedStr);
		}
	};
	
	typedef BasicStringView<char> StringView;
	
	constexpr StringView operator "" _sv(const char* str, size_t length) noexcept { return StringView(str, length); }
	constexpr BasicStringView<wchar_t> operator "" _sv(const wchar_t* str, size_t length) noexcept { return BasicStringView<wchar_t>(str, length); }
	constexpr BasicStringView<char16_t> operator "" _sv(const char16_t* str, size_t length) noexcept { return BasicStringView<char16_t>(str, length); }
	constexpr BasicStringView<char32_t> operator "" _sv(const char32_t* str, size_t length) noexcept { return BasicStringView<char32_t>(str, length); }
}
