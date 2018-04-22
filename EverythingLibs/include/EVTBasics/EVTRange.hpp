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

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#define CONSTEXPRvar constexpr
#else
#define CONSTEXPR inline
#define CONSTEXPRvar const
#endif

namespace evt {
	
	struct RangeIterator {
	private:
		size_t value_{};
		bool reversed_ = false;
	public:
		CONSTEXPR RangeIterator(const size_t value, const bool reversed = false): value_(value), reversed_(reversed) {}
		CONSTEXPR size_t operator*() const { return value_; }
		CONSTEXPR bool operator!=(const RangeIterator& range) const { return value_ != range.value_; }
		CONSTEXPR RangeIterator operator++() { return reversed_ ? RangeIterator(--value_) : RangeIterator(++value_); }
	};
	
	template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
	struct UncountableRange {
	private:
		const Type lowerBound_;
		const Type upperBound_;
	public:
		
		CONSTEXPR UncountableRange(const Type lowerBound, const Type upperBound): lowerBound_(lowerBound), upperBound_(upperBound) {}
		CONSTEXPR bool contains(const float number) const {
			return (lowerBound_ < upperBound_)
			? (number >= lowerBound_ && number < upperBound_)
			: (number > upperBound_ && number <= lowerBound_);
		}
		CONSTEXPR bool isEmpty() const { return lowerBound_ == upperBound_; }
		
		friend std::ostream& operator<<(std::ostream& os, const UncountableRange& number) noexcept {
			return os << number.toString();
		}
		
		std::string toString() const {
			auto lowerToHigherRange = "(" + std::to_string(lowerBound_) + "..<" + std::to_string(upperBound_) + ")";
			auto higherToLowerRange = "(" + std::to_string(upperBound_) + ">.." + std::to_string(lowerBound_) + ")";
			return lowerBound_ < upperBound_ ? lowerToHigherRange : higherToLowerRange;
		}
	};
	
	template <CONSTEXPRvar size_t lowerBound, CONSTEXPRvar size_t upperBound>
	struct Range {
		CONSTEXPR bool contains(const size_t number) const {
			return (lowerBound < upperBound)
			? (number >= lowerBound && number < upperBound)
			: (number > upperBound && number <= lowerBound);
		}
		CONSTEXPR bool isEmpty() const { return lowerBound == upperBound; }
		CONSTEXPR size_t count() const {
			return (lowerBound < upperBound)
			? (upperBound - lowerBound)
			: (lowerBound - upperBound);
		}
		CONSTEXPR auto begin() { return lowerBound < upperBound ? RangeIterator(lowerBound) : RangeIterator(lowerBound, true); }
		CONSTEXPR auto end() { return lowerBound < upperBound ? RangeIterator(upperBound) : RangeIterator(upperBound, true); }
		
		friend std::ostream& operator<<(std::ostream& os, const Range& number) noexcept {
			return os << number.toString();
		}
		
		std::string toString() const {
			auto lowerToHigherRange = "(" + std::to_string(lowerBound) + "..<" + std::to_string(upperBound) + ")";
			auto higherToLowerRange = "(" + std::to_string(upperBound) + ">.." + std::to_string(lowerBound) + ")";
			return lowerBound < upperBound ? lowerToHigherRange : higherToLowerRange;
		}
	};
	
	template <CONSTEXPRvar size_t lowerBound, CONSTEXPRvar size_t upperBound>
	struct ClosedRange {
		CONSTEXPR bool contains(const size_t number) const {
			return (lowerBound < upperBound)
			? (number >= lowerBound && number <= upperBound)
			: (number >= upperBound && number <= lowerBound);
		}
		CONSTEXPR size_t count() const {
			return (lowerBound < upperBound)
			? (upperBound - lowerBound)+1
			: (lowerBound - upperBound)+1;
		}
		CONSTEXPR bool isEmpty() const { return false; }
		CONSTEXPR auto begin() { return lowerBound < upperBound ? RangeIterator(lowerBound) : RangeIterator(lowerBound, true); }
		CONSTEXPR auto end() { return lowerBound < upperBound ? RangeIterator(upperBound + 1) : RangeIterator(upperBound-1, true); }
		
		friend std::ostream& operator<<(std::ostream& os, const ClosedRange& number) noexcept {
			return os << number.toString();
		}
		
		std::string toString() const {
			auto lowerToHigherRange = "(" + std::to_string(lowerBound) + "..." + std::to_string(upperBound) + ")";
			auto higherToLowerRange = "(" + std::to_string(upperBound) + "..." + std::to_string(lowerBound) + ")";
			return lowerBound < upperBound ? lowerToHigherRange : higherToLowerRange;
		}
	};
}
