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

#include "EVTBasics/EVTArray.hpp"
#include "EVTBasics/EVTOptional.hpp"
#include "EVTObject.hpp"
#include <type_traits>
#include <random>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <chrono>
#include <iostream>
#include <limits>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	namespace utils {
		
		// Some MACROS
		
		#define cplusplusVersion __cplusplus
		#define cplusplus1z 201406L
		#define cplusplus14 201402L
		#define cplusplus11 201103L
		#define cplusplus98 199711L
		
		#define xAssert(condition_, message_) if (bool(condition_) == false) { \
		std::cerr << "- Assertion failed: " << (#condition_)<< "\n- Error: " << (message_) << std::endl; exit(1); }
		
		#define guard(condition_) if (bool(condition_)){}
		
		#define sizeOf sizeof
		
		#define ref auto&
		#define constRef const auto&
		#define var auto
		#define constVar const auto
		
		// Useful functions
			
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		CONSTEXPR bool isOdd(const Type number) {
			return (number & 1) == 1;
		}
			
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		CONSTEXPR bool isEven(const Type number) {
			return (number & 1) == 0;
		}
			
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		CONSTEXPR bool isPrime(Type number) {
			
			if (number <= 1) {
				return false;
			} else if (number <= 3) {
				return true;
			} else if (number % 2 == 0 or number % 3 == 0) {
				return false;
			}
			
			Type currentNumber {5};
			
			while (currentNumber * currentNumber <= number) {
				if (number % currentNumber == 0 or number % (currentNumber + 2) == 0) {
					return false;
				}
				currentNumber += 6;
			}
			
			return true;
		}
			
		template <typename IntegralType = uint64_t, typename = typename std::enable_if<std::is_integral<IntegralType>::value,bool>::type>
		IntegralType randomIntegralNumber(IntegralType lowerBound = std::numeric_limits<IntegralType>::denorm_min(),
										  IntegralType upperBound = std::numeric_limits<IntegralType>::max()) {
			
			std::random_device rd;
			std::mt19937_64 rng(rd());
			
			if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
			std::uniform_int_distribution<IntegralType> randomValue(lowerBound, upperBound);
			
			return randomValue(rng);
		}
			
		template <typename FloatingPointType = double, typename = typename std::enable_if<std::is_floating_point<FloatingPointType>::value,bool>::type>
		FloatingPointType randomRealNumber(FloatingPointType lowerBound = std::numeric_limits<FloatingPointType>::denorm_min(),
										   FloatingPointType upperBound = std::numeric_limits<FloatingPointType>::max()) {
			
			std::random_device rd;
			std::mt19937_64 rng(rd());
			
			if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
			std::uniform_real_distribution<FloatingPointType> randomValue(lowerBound, upperBound);
			
			return randomValue(rng);
		}
			
		std::mt19937_64 randomGenerator() {
			
			#ifdef __APPLE__
				std::mt19937_64 rng(arc4random());
			#else
				std::random_device rd;
				std::mt19937_64 rng(rd());
			#endif
			
			return rng;
		}
			
		template <typename Array, typename = typename std::enable_if<std::is_array<Array>::value,bool>::type>
		CONSTEXPR size_t arrayLengthOf(const Array& array) {
			return std::end(array) - std::begin(array);
		}
			
		// TRIM: https://stackoverflow.com/a/217605/6303785
			
		void leftTrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		}
			
		void rightTrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		}
			
		void trim(std::string &s) {
			leftTrim(s);
			rightTrim(s);
		}
			
		std::string trimmed(std::string s) {
			trim(s);
			return s;
		}
			
		std::string toUpper(std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
			return str;
		}
			
		std::string toLower(std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str;
		}
			
		std::string operator "" _upper(char const * str, [[maybe_unused]] std::size_t size) {
			return toUpper(str);
		}
			
		std::string operator "" _lower(char const * str, [[maybe_unused]] std::size_t size) {
			return toLower(str);
		}
			
		Array<std::string> toUpperContainer(const Array<std::string>& strings) {
			std::for_each(strings.begin(), strings.end(), [](std::string& str){ str = toUpper(str); });
			return strings;
		}
		 
		Array<std::string> toLowerContainer(const Array<std::string>& strings) {
			std::for_each(strings.begin(), strings.end(), [](std::string& str){ str = toLower(str); });
			return strings;
		}
		
		template <typename Type = std::string>
		CONSTEXPR Optional<Type> readLine(const std::string& promptText = "") {
			
			Type readContent;
			
			if (!promptText.empty()) { std::cout << promptText << ' '; }
			
			#if (__cplusplus > cplusplus14) && defined(__clang__)
				if constexpr (std::is_same<std::string, Type>()) {
					std::getline(std::cin, readContent);
				}
				else {
			#endif
					bool willExit = false;
					if (!(std::cin >> readContent)) {
						std::cin.clear();
						willExit = true;
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					if (willExit) { return nullptr; }
			#if (__cplusplus > cplusplus14) && defined(__clang__)
			}
			#endif
			
			return readContent;
		}
		
		template <typename Type = std::string>
		CONSTEXPR Optional<Type> input(const std::string& promptText = "") {
			return readLine<Type>(promptText);
		}
		
		#if (__cplusplus > cplusplus14) && defined(__clang__)
		template <typename Type>
		std::string quoted(const Type& value, bool quoteAnything = true) {
			if constexpr (std::is_same<std::string, Type>()) {
				return "\"" + value + "\"";
			}
			else if constexpr (std::is_same<const char*, Type>()) {
				return "\"" + std::string(value) + "\"";
			}
			else if constexpr (std::is_same<char, Type>()) {
				return "'" + std::string(1, value) + "'";
			}
			else if constexpr (std::is_arithmetic<Type>()) {
				return quoteAnything ? ("\"" + std::to_string(value) + "\"") : std::to_string(value);
			}
			else if constexpr (std::is_object<Type>()) {
				return quoteAnything ? "\"[Object]\"" : "[Object]";
			}
			else {
				return "[Error, value is not a valid type to quote]";
			}
		}
		#else
		std::string quoted(const std::string& str) {
			return "\"" + str + "\"";
		}
		
		std::string quoted(const char* str) {
			return "\"" + std::string(str) + "\"";
		}
		
		std::string quoted(const char chr) {
			return "'" + std::string(1, chr) + "'";
		}
		#endif

		template <typename Function>
		CONSTEXPR float benchmark(const Function& functionToBenchmark, std::size_t iterations = 1) {
			
			float averageTime = 0.0;
			
			for (size_t i = 0; i < iterations; ++i) {
				
				auto start = std::chrono::high_resolution_clock::now();
				functionToBenchmark();
				auto end = std::chrono::high_resolution_clock::now();
				
				averageTime += float(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / float(1000000000);
			}
			
			return averageTime / float(iterations);
		}
		
		template <typename Function>
		CONSTEXPR void repeat(std::size_t timesToRepeat, const Function& functionToRepeat) {
			for (std::size_t i = 0; i < timesToRepeat; i++) {
				functionToRepeat(i);
			}
		}
	}
}

#undef CONSTEXPR
