//
//  EVTSafeNumber.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 06/05/2018.
//  Copyright © 2018 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include <exception>
#include <initializer_list>
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

namespace evt {
	
	namespace numbers {
		namespace safe {
			
			struct division_by_zero: public std::runtime_error {
				division_by_zero(): std::runtime_error("indetermination, dividing by zero") {}
			};
			struct dividing_zero_by_zero: public std::runtime_error {
				dividing_zero_by_zero(): std::runtime_error("indetermination, dividing zero by zero") {}
			};
			
			template <typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type>
			class SafeNumber {
				ArithmeticType value_ {};
			public:
				
				static CONSTEXPRVar ArithmeticType min = std::numeric_limits<ArithmeticType>::min();
				static CONSTEXPRVar ArithmeticType max = std::numeric_limits<ArithmeticType>::max();
				static CONSTEXPRVar ArithmeticType minPositive = std::numeric_limits<ArithmeticType>::denorm_min();
				static CONSTEXPRVar ArithmeticType lowest = std::numeric_limits<ArithmeticType>::lowest();
				
				CONSTEXPR SafeNumber(const SafeNumber& value): value_(value) { }
				CONSTEXPR explicit SafeNumber(SafeNumber&& value): value_(value) { }
				
				// Constructors
				CONSTEXPR explicit SafeNumber(const ArithmeticType value): value_(value) { }
				CONSTEXPR explicit SafeNumber(ArithmeticType&& value): value_(value) { }
				
				CONSTEXPR SafeNumber(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->value_ = static_cast<ArithmeticType>(*listValue);
					}
				}
				
				template <typename OtherType>
				CONSTEXPR explicit SafeNumber(const OtherType value) = delete;
				
				// Relational operators
				
				friend inline bool operator<(const SafeNumber& lhs, const SafeNumber& rhs){ return lhs.value_ < rhs.value_; }
				friend inline bool operator> (const SafeNumber& lhs, const SafeNumber& rhs){ return rhs < lhs; }
				friend inline bool operator<=(const SafeNumber& lhs, const SafeNumber& rhs){ return !(lhs > rhs); }
				friend inline bool operator>=(const SafeNumber& lhs, const SafeNumber& rhs){ return !(lhs < rhs); }
				friend inline bool operator==(const SafeNumber& lhs, const SafeNumber& rhs){ return lhs.value_ == rhs.value_; }
				friend inline bool operator!=(const SafeNumber& lhs, const SafeNumber& rhs){ return !(lhs == rhs); }
				
				friend inline bool operator<(const SafeNumber& lhs, const ArithmeticType rhs){ return lhs.value_ < rhs; }
				friend inline bool operator> (const SafeNumber& lhs, const ArithmeticType rhs){ return rhs < lhs; }
				friend inline bool operator<=(const SafeNumber& lhs, const ArithmeticType rhs){ return !(lhs > rhs); }
				friend inline bool operator>=(const SafeNumber& lhs, const ArithmeticType rhs){ return !(lhs < rhs); }
				friend inline bool operator==(const SafeNumber& lhs, const ArithmeticType rhs){ return lhs.value_ == rhs; }
				friend inline bool operator!=(const SafeNumber& lhs, const ArithmeticType rhs){ return !(lhs == rhs); }
				
				// Operator=
				
				CONSTEXPR SafeNumber& operator=(const ArithmeticType value) {
					this->value_ = value;
					return *this;
				}
				
				SafeNumber& operator=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->value_ = static_cast<ArithmeticType>(*listValue);
					}
					return *this;
				}
				
				template <typename OtherType>
				SafeNumber& operator=(const OtherType value) = delete;
				
				// Operator++, --
				
				/// This operator doesn't check overflow
				inline SafeNumber operator++() noexcept {
					return { ++this->value_ };
				}
				
				inline SafeNumber operator++(int) {
					if (this->value_ == this->max) {
						throw std::overflow_error("value overflows when stored in this type");
					}
					return { this->value_++ };
				}
				
				/// This operator doesn't check underflow
				inline SafeNumber operator--() noexcept {
					return { --this->value_ };
				}
				
				inline SafeNumber operator--(int) {
					if (this->value_ == this->lowest) {
						throw std::underflow_error("value underflows when stored in this type");
					}
					return { this->value_-- };
				}
				
				// Operator +
				
				inline SafeNumber operator+(const ArithmeticType value) {
					
					if (value < 0) { return this->operator-(static_cast<ArithmeticType>(-value)); }
					
					auto maximumValueToAdd = std::numeric_limits<ArithmeticType>::max() - this->value_;
					if (value > maximumValueToAdd) {
						throw std::overflow_error("value overflows when stored in this type");
					}
					const auto number = static_cast<ArithmeticType>(this->value_ + value);
					return SafeNumber(number);
				}
				template <typename OtherType>
				SafeNumber operator+(const OtherType value) = delete;
				
				// Operator -
				
				inline SafeNumber operator-(const ArithmeticType value) {
					
					if (value < 0) { return this->operator+(static_cast<ArithmeticType>(-value)); }
					
					auto maximumValueToSubstract = std::numeric_limits<ArithmeticType>::lowest() - this->value_;
					if (value > abs(maximumValueToSubstract)) {
						throw std::underflow_error("value underflows when stored in this type");
					}
					const auto number = static_cast<ArithmeticType>(this->value_ - value);
					return SafeNumber(number);
					
				}
				template <typename OtherType>
				SafeNumber operator-(const OtherType value) = delete;
				
				// Operator *
				
				inline SafeNumber operator*(const ArithmeticType value) {
					
					auto maximumValueToMultiply = std::numeric_limits<ArithmeticType>::max() / this->value_;
					if (abs(value) > abs(maximumValueToMultiply)) {
						throw std::overflow_error("value overflows when stored in this type");
					}
					const auto number = static_cast<ArithmeticType>(this->value_ * value);
					return SafeNumber(number);
					
				}
				template <typename OtherType>
				SafeNumber operator*(const OtherType value) = delete;
				
				// Operator %
				
				inline SafeNumber operator%(const ArithmeticType value) {
					
					if (value > std::numeric_limits<ArithmeticType>::max() || value < std::numeric_limits<ArithmeticType>::lowest()) {
						throw std::overflow_error("value overflows when stored in this type");
					}
					const auto number = static_cast<ArithmeticType>(fmod(this->value_, value));
					return SafeNumber(number);
					
				}
				
				template <typename OtherType>
				SafeNumber operator%(const OtherType value) = delete;
				
				// Operator /
				
				inline SafeNumber operator/(const ArithmeticType value) {
					
					// throw exceptions or maybe not...?
					if (this->value_ == 0 && value == 0) {
						throw dividing_zero_by_zero();
						this->value_ = std::numeric_limits<ArithmeticType>::quiet_NaN(); return *this;
					}
					if (value == 0) {
						throw division_by_zero();
						this->value_ = std::numeric_limits<ArithmeticType>::infinity(); return *this;
					}
					
					auto maximumValueToDivide = this->value_ / std::numeric_limits<ArithmeticType>::max();
					
					if (abs(value) < abs(maximumValueToDivide)) {
						throw std::overflow_error("value overflows when stored in this type");
					}
					
					const auto number = static_cast<ArithmeticType>(this->value_ / value);
					return SafeNumber(number);
					
				}
				template <typename OtherType>
				SafeNumber operator/(const OtherType value) = delete;
				
				// Operator+=
				
				inline SafeNumber& operator+=(const ArithmeticType value) {
					this->value_ = (*this + value).value_;
					return *this;
				}
				
				inline SafeNumber& operator+=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->operator+=(*listValue);
					}
					return *this;
				}
				
				
				template <typename OtherType>
				SafeNumber& operator+=(const OtherType value) = delete;
				
				// Operator -=
				
				inline SafeNumber& operator-=(const ArithmeticType value) {
					this->value_ = (*this - value).value_;
					return *this;
				}
				
				inline SafeNumber& operator-=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->operator-=(*listValue);
					}
					return *this;
				}
				
				
				template <typename OtherType>
				SafeNumber& operator-=(const OtherType value) = delete;
				
				// Operator *=
				
				inline SafeNumber& operator*=(const ArithmeticType value) {
					this->value_ = (*this * value).value_;
					return *this;
				}
				
				inline SafeNumber& operator*=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->operator*=(*listValue);
					}
					return *this;
				}
				
				
				template <typename OtherType>
				SafeNumber& operator*=(const OtherType value) = delete;
				
				// Operator /=
				
				inline SafeNumber& operator/=(const ArithmeticType value) {
					this->value_ = (*this / value).value_;
					return *this;
				}
				
				inline SafeNumber& operator/=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->operator/=(*listValue);
					}
					return *this;
				}
				
				
				template <typename OtherType>
				SafeNumber& operator/=(const OtherType value) = delete;
				
				// Operator %=
				
				inline SafeNumber& operator%=(const ArithmeticType value) {
					this->value_ = (*this % value).value_;
					return *this;
				}
				
				inline SafeNumber& operator%=(std::initializer_list<ArithmeticType> value) {
					const auto listValue = value.begin();
					if (listValue != nullptr) {
						this->operator%=(*listValue);
					}
					return *this;
				}
				
				
				template <typename OtherType>
				SafeNumber& operator%=(const OtherType value) = delete;
				
				// Casting Operator
				
				explicit constexpr operator ArithmeticType() const {
					return this->value_;
				}
				
				template <typename OtherType>
				explicit constexpr operator OtherType() const = delete;
				
				// Ostream operator
				
				friend std::ostream& operator<<(std::ostream& os, const SafeNumber<ArithmeticType> safeNumber) noexcept {
					return os << +safeNumber.value_;
				}
			};
			
			// Try to ALWAYS use the namespace before any of these, like: safe::float_t
			typedef SafeNumber<float> float_t;
			typedef SafeNumber<float> float32;
			typedef SafeNumber<double> float64;
			typedef SafeNumber<double> double_t;
			typedef SafeNumber<long double> long_double;
			typedef SafeNumber<long double> float80;
			
			typedef SafeNumber<uint8_t> uint8_t;
			typedef SafeNumber<uint16_t> uint16_t;
			typedef SafeNumber<uint32_t> uint32_t;
			typedef SafeNumber<uint64_t> uint64_t;
			
			typedef SafeNumber<int8_t> int8_t;
			typedef SafeNumber<int16_t> int16_t;
			typedef SafeNumber<int32_t> int32_t;
			typedef SafeNumber<int64_t> int64_t;
			
			typedef SafeNumber<size_t> size_t;
			// ...
		}
	}
}
		
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<float>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<double>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<long double>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<uint8_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<uint16_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<uint32_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<uint64_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<size_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<int8_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<int16_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<int32_t>> { constexpr static bool value = true; };
template <> struct std::is_arithmetic<evt::numbers::safe::SafeNumber<int64_t>> { constexpr static bool value = true; };
					
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<uint8_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<uint16_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<uint32_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<uint64_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<size_t>> { constexpr static bool value = true; };
					
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<int8_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<int16_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<int32_t>> { constexpr static bool value = true; };
template <> struct std::is_unsigned<evt::numbers::safe::SafeNumber<int64_t>> { constexpr static bool value = true; };
