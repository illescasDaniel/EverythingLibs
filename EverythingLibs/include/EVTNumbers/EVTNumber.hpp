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

namespace evt {
	
	#define ArithmeticType_typename typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value,bool>::type
	#define operatorAssignment(_op_, _op2_); template <typename anyType> inline ArithmeticType operator _op_ (const anyType& Var) { return (*this = *this _op2_ Var); }
	
	template <typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value,bool>::type>
	class Number {
		
	private:
		
		ArithmeticType value_;
		
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
			return (value_ < 0) ? -value_ : value_;
		}
		
		template <typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value,bool>::type>
		Type as() const {
			return static_cast<Type>(value_);
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
