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
	
	class StringView: public std::basic_string_view<CharType>, public EVTObject {
		
		typedef std::basic_string_view<CharType> super;
		
		// Will hide these methods from the public access
		constexpr bool empty() const noexcept { return super::empty(); }
		constexpr std::size_t max_size() const noexcept { return super::max_size(); }
		constexpr const CharType& front() const { return super::front(); }
		constexpr const CharType& back() const { return super::back(); }
		constexpr const CharType& at(std::size_t pos) const { return super::at(pos); };
		constexpr void remove_prefix(std::size_t n) { return super::remove_prefix(n); }
		constexpr void remove_suffix(std::size_t n) { return super::remove_suffix(n); }
		static constexpr std::size_t npos = super::npos;
		constexpr StringView substr(std::size_t pos = 0, std::size_t count = super::npos ) const { return super::substr(pos, count); }
		
	public:
		
		constexpr StringView() noexcept : super() {}
		constexpr StringView(const std::basic_string_view<CharType>& other) noexcept : super(other) {}
		constexpr StringView(const std::string& other) noexcept : super(other) {}
		constexpr StringView(const CharType* str, std::size_t count) noexcept : super(str, count) {}
		constexpr StringView(const CharType* str) noexcept : super(str) {}
		
		constexpr bool isEmpty() const noexcept { return this->empty(); }
		constexpr std::size_t count() const noexcept { return this->length(); }
		constexpr std::size_t maxSize() const noexcept { return this->max_size(); }
		constexpr const CharType& operator[](std::size_t position) const { return this->at(position); }
		constexpr const CharType& first() const { return (*this)[0]; }
		constexpr const CharType& last() const { return (*this)[this->count()-1]; }
		constexpr void removePrefix(std::size_t prefixSize) { return this->remove_prefix(prefixSize); }
		constexpr void removeSuffix(std::size_t suffixSize) { return this->remove_suffix(suffixSize); }
		constexpr std::size_t findFirstOf(const StringView& str, std::size_t position = 0) const noexcept { return this->find_first_of(str, position); }
		constexpr std::size_t findFirstNotOf(const StringView& str, std::size_t position = 0) const noexcept { return this->find_first_not_of(str, position); }
		constexpr std::size_t findLastOf(const StringView& str, std::size_t position = StringView::nullPosition) const noexcept { return this->find_last_of(str, position); }
		constexpr std::size_t findLastNotOf(const StringView& str, std::size_t position = StringView::nullPosition) const noexcept { return this->find_last_not_of(str, position); }
		constexpr StringView subString(std::size_t position = 0, std::size_t count = StringView::nullPosition ) const { return this->substr(position, count); }
		
		static constexpr std::size_t nullPosition = super::npos;
		
		enum TrimMode { start, end, both };
		constexpr void trim(const TrimMode& trimMode = both) {
			
			switch (trimMode) {
					
				case start:
					if (std::size_t position = this->findFirstNotOf(" "); position != StringView::nullPosition) {
						this->removePrefix(position);
					}
					break;
					
				case end:
					if (std::size_t position = this->count() - this->findLastOf(" ") + 1; position != StringView::nullPosition) {
						this->removeSuffix(position);
					}
					break;
					
				case both:
					if (std::size_t position = this->findFirstNotOf(" "); position != StringView::nullPosition) {
						this->removePrefix(position);
					}
					if (std::size_t position = this->count() - this->findLastOf(" ") + 1; position != StringView::nullPosition) {
						this->removeSuffix(position);
					}
					break;
			}
		}
		
		constexpr StringView trimmed(const TrimMode& trimMode = both) const {
			StringView aux(*this);
			aux.trim(trimMode);
			return aux;
		}
		
		constexpr bool contains(const StringView& str, size_t pos = 0) const {
			return this->find(str, pos) != StringView::nullPosition;
		}
		
		constexpr std::size_t reverseFind(const StringView& str, std::size_t position = StringView::nullPosition) {
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
	};
	
	typedef StringView<char> String;
	
	constexpr String operator "" _sv(const char* str, size_t length) noexcept { return String(str, length); }
	constexpr StringView<wchar_t> operator "" _sv(const wchar_t* str, size_t length) noexcept { return StringView<wchar_t>(str, length); }
	constexpr StringView<char16_t> operator "" _sv(const char16_t* str, size_t length) noexcept { return StringView<char16_t>(str, length); }
	constexpr StringView<char32_t> operator "" _sv(const char32_t* str, size_t length) noexcept { return StringView<char32_t>(str, length); }
}
