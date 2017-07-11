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

#include "EVTArray.hpp"
#include <type_traits>
#include <random>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <chrono>

namespace evt {
	
	namespace utils {
		
		#define cplusplusVersion __cplusplus
		#define cplusplus1z 201406
		#define cplusplus14 201402
		#define cplusplus11 201100
		
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		static inline bool isOdd(const Type number) {
			return (number & 1) == 1;
		}
		
		template <typename Type, typename = typename std::enable_if<std::is_integral<Type>::value,bool>::type>
		static inline bool isEven(const Type number) {
			return (number & 1) == 0;
		}
		
		uint64_t randomNumber(uint64_t upperLimit = std::mt19937_64::max()) {
			return std::mt19937_64{ std::random_device{}() }() % upperLimit;
		}
		
		uint64_t randomDistributedNumber(uint64_t lowerBound = std::mt19937_64::min(), uint64_t upperBound = std::mt19937_64::max()) {
			
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<uint64_t> randomValue(lowerBound, upperBound);
			
			return randomValue(rng);
		}
		
		template <typename Array, typename = typename std::enable_if<std::is_array<Array>::value,bool>::type>
		size_t arrayLengthOf(const Array& array) {
			return std::end(array) - std::begin(array);
		}
		
		// TRIM: https://stackoverflow.com/a/217605/6303785
		
		inline void leftTrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		}
		
		inline void rightTrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		}
		
		inline void trim(std::string &s) {
			leftTrim(s);
			rightTrim(s);
		}
		
		inline std::string trimmed(std::string s) {
			trim(s);
			return s;
		}
		
		std::string operator "" _upper(char const * str, [[maybe_unused]] std::size_t size) {
			std::string output = str;
			std::transform(output.begin(), output.end(), output.begin(), ::toupper);
			return output;
		}
		
		std::string operator "" _lower(char const * str, [[maybe_unused]] std::size_t size) {
			std::string output = str;
			std::transform(output.begin(), output.end(), output.begin(), ::tolower);
			return output;
		}
		
		std::string toUpper(std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
			return str;
		}
		
		std::string toLower(std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str;
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
		Type readLine() {
			Type readContent{};
			std::cin >> readContent;
			return readContent;
		}
		
		std::string quoted(std::string str) {
			return "\"" + str + "\"";
		}
		
		template <typename Function>
		float benchmark(const Function& function, size_t iterations = 1) {
			
			float averageTime = 0.0;
			
			for (size_t i = 0; i < iterations; ++i) {
				
				auto start = std::chrono::high_resolution_clock::now();
				function();
				auto end = std::chrono::high_resolution_clock::now();
				
				averageTime += float(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / float(1000000000);
			}
			
			return averageTime / float(iterations);
		}
	}
}
