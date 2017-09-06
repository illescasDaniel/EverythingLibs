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

#include "EVTNumber.hpp"
#include <ostream>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	namespace numbers {
	
		struct Division {
			std::intmax_t quotient = 0;
			std::intmax_t remaining = 0;
			Division(const std::intmax_t quotient, const std::intmax_t remaining): quotient(quotient), remaining(remaining) {}
			std::string toString() const noexcept { return "Quotient: " + std::to_string(quotient) + "\nRemaining: " + std::to_string(remaining); }
			friend std::ostream& operator<<(std::ostream& os, const Division& object) noexcept { return os << object.toString(); }
		};
		
		template <typename IntegralType = int, typename = typename std::enable_if<std::is_integral<IntegralType>::value,bool>::type>
		class Integer: public Number<IntegralType> {
			
			typedef Number<IntegralType> super;
			
		public:
			
			CONSTEXPR Integer() noexcept  {}
			CONSTEXPR Integer(IntegralType integer) noexcept : super(integer) {}
			
			CONSTEXPR bool isOdd() noexcept {
				return (super::value() & 1) == 1;
			}
			
			CONSTEXPR bool isEven() noexcept {
				return (super::value() & 1) == 0;
			}
			
			CONSTEXPR bool isPrime() noexcept {
				
				IntegralType number = (*this).absolute();
				
				if (number <= 1) {
					return false;
				} else if (number <= 3) {
					return true;
				} else if (number % 2 == 0 or number % 3 == 0) {
					return false;
				}
				
				IntegralType currentNumber {5};
				
				while (currentNumber * currentNumber <= number) {
					if (number % currentNumber == 0 or number % (currentNumber + 2) == 0) {
						return false;
					}
					currentNumber += 6;
				}
				
				return true;
			}
			
			static Integer random(Integer lowerBound = std::numeric_limits<IntegralType>::denorm_min(),
								 Integer upperBound = std::numeric_limits<IntegralType>::max()) {
				
				if ((lowerBound < 0 || upperBound < 0) && std::is_unsigned<IntegralType>()) {
					throw std::logic_error("Type is unsigned and bounds were negative!");
				}
				
				std::random_device rd;
				std::mt19937_64 rng(rd());
				
				if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
				std::uniform_int_distribution<IntegralType> randomValue(lowerBound, upperBound);
				
				return Integer(randomValue(rng));
			}
			
			Division dividedBy(const IntegralType number) const {
				auto divisionResult = std::lldiv(super::value(), number);
				return Division(divisionResult.quot, divisionResult.rem);
			}
		};
		
		typedef Integer<int> Int;
		typedef Integer<unsigned int> UInt;
		typedef Integer<size_t> Size;
		typedef Integer<int8_t> Int8;
		typedef Integer<int16_t> Int16;
		typedef Integer<int32_t> Int32;
		typedef Integer<int64_t> Int64;
		typedef Integer<uint8_t> UInt8;
		typedef Integer<uint16_t> UInt16;
		typedef Integer<uint32_t> UInt32;
		typedef Integer<uint64_t> UInt64;
		typedef Integer<intmax_t> IntMax;
		typedef Integer<uintmax_t> UIntMax;
		
		std::ostream& operator<<(std::ostream& os, const UInt8& number) noexcept {
			return os << static_cast<uint16_t>(number);
		}
			
		std::ostream& operator<<(std::ostream& os, const Int8& number) noexcept {
			return os << static_cast<int16_t>(number);
		}
	}
}

#undef CONSTEXPR
