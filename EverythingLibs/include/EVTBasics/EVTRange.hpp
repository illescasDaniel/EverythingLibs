//
//  EVTRange.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 30/08/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

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
