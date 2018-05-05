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
	
	namespace numbers {
	
#define ArithmeticType_typename typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value || std::is_same<ArithmeticType, __int128_t>::value ||std::is_same<ArithmeticType, __uint128_t>::value>::type
		
		template <ArithmeticType_typename>
		class Number {
			
		private:
			
			ArithmeticType value_{};
			
			CONSTEXPR void throwOverflow() const { throw std::overflow_error("value overflows when stored in this type"); }
			
			CONSTEXPR bool isIntegral() const {
				return std::is_integral<ArithmeticType>();
			}
			
			template <typename Type>
			CONSTEXPR void assignNumberIfDoesNotOverflow(Type number) {
				
				if (!this->isIntegral()) { value_ = static_cast<ArithmeticType>(number); return; }
				
				if ((std::numeric_limits<Type>::max() > std::numeric_limits<ArithmeticType>::max() && number > static_cast<Type>(std::numeric_limits<ArithmeticType>::max())) || (number < Type{} && std::is_unsigned<ArithmeticType>())) {
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
			CONSTEXPR ArithmeticType operator+=(const anyType& number) { return (*this = *this + number); }
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator-=(const anyType& number) { return (*this = *this - number); }
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator*=(const anyType& number) { return (*this = *this * number); }
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator/=(const anyType& number) { return (*this = *this / number); }
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator%=(const anyType& number) const { return (*this = *this % number); }
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator/(const anyType& number) const {
				
				if (this->value_ == 0 && number == 0) { return std::numeric_limits<ArithmeticType>::quiet_NaN(); }
				if (number == 0) { return std::numeric_limits<ArithmeticType>::infinity(); }
				
				this->checkOperatorDivide(number);
				return (this->value_ / number);
			}
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator*(const anyType& number) const {
				this->checkOperatorMultiplyOverflow(number);
				return (this->value_ * number);
			}
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator%(const anyType& number) const {
				return std::fmod(this->value_, number);
			}
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator-(const anyType& number) const {
				this->checkOperatorSubstractOverflow(number);
				return (this->value_ - number);
			}
			
			template <typename anyType>
			CONSTEXPR ArithmeticType operator+(const anyType& number) const {
				this->checkOperatorAdd(number);
				return (this->value_ + number);
			}
			
			template <typename Type>
			CONSTEXPR friend Number<ArithmeticType> operator+(Type number, const Number<ArithmeticType>& otherNumber) {
				Number<ArithmeticType> newNumber(otherNumber); newNumber += number;
				return newNumber;
			}
			
			template <typename Type>
			CONSTEXPR friend Number<ArithmeticType> operator-(Type number, const Number<ArithmeticType>& otherNumber) {
				Number<ArithmeticType> newNumber(otherNumber); newNumber -= number;
				return newNumber;
			}
			
			template <typename Type>
			CONSTEXPR friend Number<ArithmeticType> operator*(Type number, const Number<ArithmeticType>& otherNumber) {
				Number<ArithmeticType> newNumber(otherNumber); newNumber *= number;
				return newNumber;
			}
			
			template <typename Type>
			CONSTEXPR friend Number<ArithmeticType> operator/(Type number, const Number<ArithmeticType>& otherNumber) {
				Number<ArithmeticType> newNumber(otherNumber); newNumber /= number;
				return newNumber;
			}
			
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
			
		public:
			
			CONSTEXPR void set(ArithmeticType number) noexcept {
				value_ = number;
			}
			
			CONSTEXPR ArithmeticType value() const noexcept {
				return value_;
			}
		protected:
			
			template <typename anyType>
			CONSTEXPR void checkOperatorSubstractOverflow(anyType number) const {
				if (this->value_ == std::numeric_limits<ArithmeticType>::min() && number > 0) {
					this->throwOverflow();
				}
				else {
					ArithmeticType maximumValueCanSubstract = std::numeric_limits<ArithmeticType>::min() + this->absolute();
					
					if (std::numeric_limits<ArithmeticType>::min() < std::numeric_limits<anyType>::min() && static_cast<ArithmeticType>(number) > maximumValueCanSubstract) {
						this->throwOverflow();
					} else if (std::numeric_limits<anyType>::min() <= std::numeric_limits<ArithmeticType>::min() && number > static_cast<anyType>(maximumValueCanSubstract)) {
						this->throwOverflow();
					}
				}
			}
			
			template <typename anyType>
			void checkOperatorMultiplyOverflow(anyType number) const {
				
				ArithmeticType maximumValueCanMultiply = std::numeric_limits<ArithmeticType>::max() / this->value_;
				
				if ((std::numeric_limits<anyType>::max() > std::numeric_limits<ArithmeticType>::max() && number > static_cast<anyType>(maximumValueCanMultiply)) || (std::numeric_limits<ArithmeticType>::max() > std::numeric_limits<anyType>::max() && static_cast<ArithmeticType>(number) > maximumValueCanMultiply)) {
					this->throwOverflow();
				}
				else if ((std::is_unsigned<ArithmeticType>() && number < 0) || (number >= 1 && (this->value_ * number) < this->value_)) {
					this->throwOverflow();
				}
			}
			
			template <typename anyType>
			CONSTEXPR void checkOperatorDivide(anyType number) const {
				if ((std::is_unsigned<ArithmeticType>() && number < 0) || (number < 1 && (this->value_ / number) < this->value_)) {
					this->throwOverflow();
				}
				// No need to check divisions like: X / 0.001, with Float types it will return "inf" in overflows :D XD
			}
			
			template <typename anyType>
			CONSTEXPR void checkOperatorAdd(anyType number) const {
				if ((std::is_unsigned<ArithmeticType>() && number < 0) || (this->value_ == std::numeric_limits<ArithmeticType>::max() && number > 0)) {
					this->throwOverflow();
				}
				else {
					ArithmeticType maximumValueCanAdd = std::numeric_limits<ArithmeticType>::max() - this->absolute();
					
					if (std::numeric_limits<ArithmeticType>::max() > std::numeric_limits<anyType>::max() && static_cast<ArithmeticType>(number) > maximumValueCanAdd) {
						this->throwOverflow();
					} else if (std::numeric_limits<anyType>::max() >= std::numeric_limits<ArithmeticType>::max() && number > static_cast<anyType>(maximumValueCanAdd)) {
						this->throwOverflow();
					}
				}
			}
		};
		
		template <typename Type, typename = typename std::enable_if<!std::is_same<Type,__int128_t>::value>::type>
		std::ostream& operator<<(std::ostream& os, const Number<Type>& number) noexcept {
			return os << number.value();
			}
			
			std::ostream& operator<<(std::ostream& dest, const Number<__int128_t>& number) {
				if (std::ostream::sentry(dest)) {
					__uint128_t tmp = number.value() < 0 ? -number.value() : number.value();
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
			
			std::ostream& operator<<(std::ostream& dest, const Number<__uint128_t>& number) {
				if (std::ostream::sentry(dest)) {
					__uint128_t tmp = number.value();
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
	}
}

#undef CONSTEXPR
#undef CONSTEXPRVar
#undef operatorAssignment

#if !defined(__clang__) || INTPTR_MAX != INT64_MAX
	#undef __int128_t
	#undef __uint128_t
#endif
