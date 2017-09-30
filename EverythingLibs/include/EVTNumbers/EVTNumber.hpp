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
#include <stdexcept>

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

#if !defined(__clang__) || INTPTR_MAX != INT64_MAX
	#define __int128_t intmax_t
	#define __uint128_t uintmax_t
#endif

namespace evt {
	
	#define ArithmeticType_typename typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value || std::is_same<ArithmeticType, __int128_t>::value ||std::is_same<ArithmeticType, __uint128_t>::value>::type
	
	template <ArithmeticType_typename>
	class Number {
		
	private:
		
		ArithmeticType value_{};
		
		CONSTEXPR void throwOverflow() { throw std::overflow_error("value overflows when stored in this integer type"); }
		
		CONSTEXPR bool isIntegral() {
			return std::is_integral<ArithmeticType>();
		}
		
		CONSTEXPR void checkIfPositiveOverflow() {
			if (!this->isIntegral()) { return; }
			if (this->value_ == std::numeric_limits<ArithmeticType>::max()) {
				this->throwOverflow();
			}
		}
		
		CONSTEXPR void checkIfNegativeOverflow() {
			if (!this->isIntegral()) { return; }
			if (this->value_ == std::numeric_limits<ArithmeticType>::lowest()) {
				this->throwOverflow();
			}
		}
		
		template <typename Type>
		CONSTEXPR void assignNumberIfDoesNotOverflow(Type number) {
			
			if (!this->isIntegral()) { value_ = static_cast<ArithmeticType>(number); return; }
			
			if ((std::numeric_limits<Type>::max() > std::numeric_limits<ArithmeticType>::max() && number > static_cast<Type>(std::numeric_limits<ArithmeticType>::max())) || (number < Type{} && std::is_unsigned<ArithmeticType>())){
				this->throwOverflow();
			}
			else {
				value_ = static_cast<ArithmeticType>(number);
			}
		}
		
	public:
		
		static CONSTEXPRVar ArithmeticType min = std::numeric_limits<ArithmeticType>::min();
		static CONSTEXPRVar ArithmeticType max = std::numeric_limits<ArithmeticType>::max();
		static CONSTEXPRVar ArithmeticType minPositive = std::numeric_limits<ArithmeticType>::denorm_min();
		static CONSTEXPRVar ArithmeticType lowest = std::numeric_limits<ArithmeticType>::lowest();
		
		CONSTEXPR Number() noexcept {}
		
		template <typename Type, typename = typename std::enable_if<
		std::is_arithmetic<Type>::value ||
		std::is_same<Type, __int128_t>::value ||
		std::is_same<Type, __uint128_t>::value>::type>
		CONSTEXPR Number(Type number) {
			this->assignNumberIfDoesNotOverflow(number);
		}
		
		template <typename Type, typename = typename std::enable_if<
		std::is_arithmetic<Type>::value ||
		std::is_same<Type, __int128_t>::value ||
		std::is_same<Type, __uint128_t>::value>::type>
		CONSTEXPR Number(const Number<Type>& number) {
			this->assignNumberIfDoesNotOverflow(number);
		}

		CONSTEXPR operator ArithmeticType() const {
			return value_;
		}
		
		// Operators overloading

		template <typename anyType>
		CONSTEXPR ArithmeticType operator+=(const anyType& Var) { this->checkIfPositiveOverflow(); return (*this = *this + Var); }
		
		template <typename anyType>
		CONSTEXPR ArithmeticType operator-=(const anyType& Var) { this->checkIfNegativeOverflow(); return (*this = *this - Var); }
		
		template <typename anyType>
		CONSTEXPR ArithmeticType operator*=(const anyType& Var) { this->checkIfPositiveOverflow(); return (*this = *this * Var); }
		
		template <typename anyType>
		CONSTEXPR ArithmeticType operator/=(const anyType& Var) { this->checkIfNegativeOverflow(); return (*this = *this / Var); }
			
		CONSTEXPR ArithmeticType operator++() { this->checkIfPositiveOverflow(); return (*this = *this + 1); }
		CONSTEXPR ArithmeticType operator--() { this->checkIfPossitiveOverflow(); return (*this = *this - 1); }
		CONSTEXPR ArithmeticType operator++(int) { this->checkIfPositiveOverflow(); return (this->operator++() - 1); }
		CONSTEXPR ArithmeticType operator--(int) { this->checkIfPossitiveOverflow(); return (this->operator--() + 1); }
		
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
		
	public:
		
		CONSTEXPR void set(ArithmeticType number) noexcept {
			value_ = number;
		}
		
		CONSTEXPR ArithmeticType value() const noexcept {
			return value_;
		}
	};
	
	template <typename Type>
	std::ostream& operator<<(std::ostream& os, const Number<Type>& number) noexcept {
		return os << number.value();
	}
}

#undef CONSTEXPR
#undef CONSTEXPRVar
#undef operatorAssignment

#if !defined(__clang__) || INTPTR_MAX != INT64_MAX
	#undef __int128_t
	#undef __uint128_t
#endif
