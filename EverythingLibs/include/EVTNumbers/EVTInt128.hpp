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

#include "EVTInteger.hpp"

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	namespace numbers {
		
		class Int128: public Integer<__int128_t> {
			
			typedef Integer<__int128_t> super;
			
		public:
			
			CONSTEXPR Int128() noexcept  {}
			
			template <typename Type>
			CONSTEXPR Int128(Type integer) noexcept: super(integer) {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_integral<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR Int128(const Number<Type>& number): super(number) {}
			
			static Int128 random(Int128 lowerBound = std::numeric_limits<__int128_t>::denorm_min(),
									   Int128 upperBound = std::numeric_limits<__int128_t>::max()) {
				
				if ((lowerBound < 0 || upperBound < 0) && std::is_unsigned<__int128_t>()) {
					throw std::logic_error("Type is unsigned and bounds were negative!");
				}
				
				std::random_device rd;
				std::mt19937_64 rng(rd());
				
				if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
				std::uniform_int_distribution<__int128_t> randomValue(lowerBound, upperBound);
				
				return Int128(randomValue(rng));
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto to(Arithmetic exponent) {
				return std::pow(intmax_t(this->value()), exponent);
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto operator^(Arithmetic exponent) {
				return this->to(exponent);
			}
			
			template <typename Type>
			CONSTEXPR Int128 operator+(Type otherNumber) const {
				this->checkOperatorAdd(otherNumber);
				return Int128(this->value() + otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR Int128 operator-(Type otherNumber) const {
				this->checkOperatorSubstractOverflow(otherNumber);
				return Int128(this->value() - otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR Int128 operator*(Type otherNumber) const {
				this->checkOperatorMultiplyOverflow(otherNumber);
				return Int128(this->value() * otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR Int128 operator/(Type otherNumber) const {
				this->checkOperatorSubstractOverflow(otherNumber);
				return Int128(this->value() / otherNumber);
			}
		};
		
		#undef internalOperator
		#undef externalOperator
		#undef assignmentOperator
		
		class UInt128: public Integer<__uint128_t> {
			
			typedef Integer<__uint128_t> super;
			
		public:
			
			CONSTEXPR UInt128() noexcept  {}
			
			template <typename Type>
			CONSTEXPR UInt128(Type integer): super(integer) {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_integral<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR UInt128(const Number<Type>& number): super(number) {}
			
			static UInt128 random(UInt128 lowerBound = std::numeric_limits<__uint128_t>::denorm_min(),
								 UInt128 upperBound = std::numeric_limits<__uint128_t>::max()) {
				
				std::random_device rd;
				std::mt19937_64 rng(rd());
				
				if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
				std::uniform_int_distribution<__uint128_t> randomValue(lowerBound, upperBound);
				
				return UInt128(randomValue(rng));
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto to(Arithmetic exponent) {
				return std::pow(intmax_t(this->value()), exponent);
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto operator^(Arithmetic exponent) {
				return this->to(exponent);
			}
			
			template <typename Type>
			CONSTEXPR UInt128 operator+(Type otherNumber) const {
				this->checkOperatorAdd(otherNumber);
				return UInt128(this->value() + otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR UInt128 operator-(Type otherNumber) const {
				this->checkOperatorSubstractOverflow(otherNumber);
				return UInt128(this->value() - otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR auto operator*(Type otherNumber) const {
				this->checkOperatorMultiplyOverflow(otherNumber);
				return UInt128(this->value() * otherNumber);
			}
			
			template <typename Type>
			CONSTEXPR UInt128 operator/(Type otherNumber) const {
				this->checkOperatorSubstractOverflow(otherNumber);
				return UInt128(this->value() / otherNumber);
			}
		};
		
		#undef internalOperator
		#undef externalOperator
	}
}

#undef CONSTEXPR
