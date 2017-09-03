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

namespace evt {
	
	struct Range {
		size_t first_{};
		size_t last_{};
		Range(size_t first, size_t last): first_(first), last_(last) {}
		Range(size_t last): first_(size_t{}), last_(last) {}
	};
	
	struct RangeUntil {
		size_t first_{};
		size_t last_{};
		RangeUntil(size_t first, size_t last): first_(first) {
			if (last > 0) {
				this->last_ = last - 1;
			}
		}
		RangeUntil(size_t last): first_(size_t{}) {
			if (last > 0) {
				this->last_ = last - 1;
			}
		}
	};
	
	struct RangeIterator {
	private:
		size_t value_{};
	public:
		RangeIterator(size_t value): value_(value) {}
		size_t operator*() const { return value_; }
		bool operator!=(const RangeIterator& range) const { return value_ != range.value_; }
		RangeIterator operator++() { return {++value_}; }
	};
	
	RangeIterator begin(const Range& range) { return { range.first_ }; }
	RangeIterator end(const Range& range) { return { range.last_ + 1 }; }
	
	RangeIterator begin(const RangeUntil& range) { return { range.first_ }; }
	RangeIterator end(const RangeUntil& range) { return { range.last_ + 1 }; }
}
