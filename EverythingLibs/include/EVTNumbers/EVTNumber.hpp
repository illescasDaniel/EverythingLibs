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

#include <random>
#include <cstdint>
#include <cstdlib>
#include <cmath>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

#if (__cplusplus > 201103L)
#define CONSTEXPRVar constexpr
#else
#define CONSTEXPRVar const
#endif

#if INTPTR_MAX == INT32_MAX
	#define __int128_t intmax_t
	#define __uint128_t uintmax_t
#endif

namespace evt {
	
	#define ArithmeticType_typename typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value || std::is_same<ArithmeticType, __int128_t>::value ||std::is_same<ArithmeticType, __uint128_t>::value>::type
	#define operatorAssignment(_op_, _op2_); template <typename anyType> inline ArithmeticType operator _op_ (const anyType& Var) { return (*this = *this _op2_ Var); }
	
	template <ArithmeticType_typename>
	class Number {
		
	private:
		
		ArithmeticType value_{};
		
	public:
		
		static CONSTEXPRVar ArithmeticType min = std::numeric_limits<ArithmeticType>::min();
		static CONSTEXPRVar ArithmeticType max = std::numeric_limits<ArithmeticType>::max();
		static CONSTEXPRVar ArithmeticType minPositive = std::numeric_limits<ArithmeticType>::denorm_min();
		static CONSTEXPRVar ArithmeticType lowest = std::numeric_limits<ArithmeticType>::lowest();
		
		CONSTEXPR Number() noexcept {}
		CONSTEXPR Number(ArithmeticType number) noexcept : value_(number) {}
		
		CONSTEXPR operator ArithmeticType() const {
			return value_;
		}
		
		// Operators overloading
		operatorAssignment(+=,+) operatorAssignment(-=,-) operatorAssignment(*=,*) operatorAssignment(/=,/) operatorAssignment(%=,%)
		
		CONSTEXPR ArithmeticType operator++() { return (*this = *this + 1); }
		CONSTEXPR ArithmeticType operator--() { return (*this = *this - 1); }
		CONSTEXPR ArithmeticType operator++(int) { return (this->operator++() - 1); }
		CONSTEXPR ArithmeticType operator--(int) { return (this->operator--() + 1); }
		
		std::string toString() const {
			return std::to_string(value_);
		}
		
		CONSTEXPR ArithmeticType absolute() const noexcept {
			
		#if (__cplusplus > 201402L) && defined(__clang__)
			if constexpr (std::is_unsigned<ArithmeticType>()) {
				return value_;
			}
		#endif
			return (value_ < 0) ? -value_ : value_;
		}
		
		template <typename Type, typename = typename std::enable_if<
		std::is_arithmetic<Type>::value ||
		std::is_same<Type, __int128_t>::value ||
		std::is_same<Type, __uint128_t>::value>::type>
		Type as() const {
			return static_cast<Type>(value_);
		}
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
		auto to(Arithmetic exponent) {
			return std::pow(this->value_, exponent);
		}
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
		auto operator^(Arithmetic exponent) {
			return this->to(exponent);
		}
		
		template <typename Arithmetic, typename = typename std::enable_if<std::is_arithmetic<Arithmetic>::value>::type>
		auto remainderDividingBy(Arithmetic other) {
			return std::fmod(this->value_, other);
		}
		
	protected:
		
		CONSTEXPR void set(ArithmeticType number) noexcept {
			value_ = number;
		}
		
		CONSTEXPR ArithmeticType value() const noexcept {
			return value_;
		}
	};
}

#undef CONSTEXPR
#undef CONSTEXPRVar
#undef operatorAssignment

#if INTPTR_MAX == INT32_MAX
	#undef __int128_t
	#undef __uint128_t
#endif
