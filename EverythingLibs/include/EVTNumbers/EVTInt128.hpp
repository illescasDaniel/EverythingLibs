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
			
			__int128_t getValue() const {
				return super::value();
			}
			
		public:
			
			CONSTEXPR Int128() noexcept  {}
			
			template <typename Type>
			CONSTEXPR Int128(Type integer) noexcept: super(integer) {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_integral<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR Int128(const Number<Type>& number): super(number) {}
			
			friend std::ostream& operator<<(std::ostream& dest, const Int128& number) {
				if (std::ostream::sentry(dest)) {
					__uint128_t tmp = number.getValue() < 0 ? -number.getValue() : number.getValue();
					char buffer[128];
					char* d = std::end(buffer);
					do {
						d -= 1;
						*d = "0123456789"[tmp % 10];
						tmp /= 10;
					} while (tmp != 0);
					
					if (number.as<__int128_t>() < 0) {
						d -= 1;
						*d = '-';
					}
					long len = std::end(buffer) - d;
					if (dest.rdbuf()->sputn(d, len) != len) {
						dest.setstate(std::ios_base::badbit);
					}
				}
				
				return dest;
			}
			
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
				return std::pow(intmax_t(this->getValue()), exponent);
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto operator^(Arithmetic exponent) {
				return this->to(exponent);
			}
			
			#define internalOperator(operation) \
			template <typename Type> \
			CONSTEXPR Int128 operator operation (Type otherNumber) const noexcept { \
				Int128 newNumber(this->value() operation otherNumber); \
				return newNumber; \
			}
			
			#define externalOperator(operation) \
			template <typename Type> \
			CONSTEXPR friend Int128 operator operation (Type number, const Int128& otherNumber) noexcept { \
				Int128 newNumber(otherNumber operation number); \
				return newNumber; \
			}
			
			internalOperator(+); internalOperator(-); internalOperator(*); internalOperator(/); internalOperator(%);
			externalOperator(+); externalOperator(-); externalOperator(*); externalOperator(/); externalOperator(%);
		};
		
		#undef internalOperator
		#undef externalOperator
		#undef assignmentOperator
		
		class UInt128: public Integer<__uint128_t> {
			
			typedef Integer<__uint128_t> super;
			
			__uint128_t getValue() const {
				return super::value();
			}
			
		public:
			
			CONSTEXPR UInt128() noexcept  {}
			
			template <typename Type>
			CONSTEXPR UInt128(Type integer): super(integer) {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_integral<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR UInt128(const Number<Type>& number): super(number) {}
			
			friend std::ostream& operator<<(std::ostream& dest, const UInt128& number) {
				if (std::ostream::sentry(dest)) {
					__uint128_t tmp = number.getValue();
					char buffer[128];
					char* d = std::end(buffer);
					do {
						d -= 1;
						*d = "0123456789"[tmp % 10];
						tmp /= 10;
					} while (tmp != 0);
					
					long len = std::end(buffer) - d;
					if (dest.rdbuf()->sputn(d, len) != len) {
						dest.setstate(std::ios_base::badbit);
					}
				}
				
				return dest;
			}
			
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
				return std::pow(intmax_t(this->getValue()), exponent);
			}
			
			template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
			auto operator^(Arithmetic exponent) {
				return this->to(exponent);
			}
			
			#define internalOperator(operation) \
			template <typename Type> \
			CONSTEXPR UInt128 operator operation (Type otherNumber) const noexcept { \
			UInt128 newNumber(this->value() operation otherNumber); \
			return newNumber; \
			}
			
			#define externalOperator(operation) \
			template <typename Type> \
			CONSTEXPR friend UInt128 operator operation (Type number, const Int128& otherNumber) noexcept { \
			UInt128 newNumber(otherNumber operation number); \
			return newNumber; \
			}
			
			internalOperator(+); internalOperator(-); internalOperator(*); internalOperator(/); internalOperator(%);
			externalOperator(+); externalOperator(-); externalOperator(*); externalOperator(/); externalOperator(%);
		};
		
		#undef internalOperator
		#undef externalOperator
	}
}

#undef CONSTEXPR
