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

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	namespace numbers {
		
		template <typename FloatingType = float, typename = typename std::enable_if<std::is_floating_point<FloatingType>::value>::type>
		class FloatingPoint: public Number<FloatingType> {
			
			typedef Number<FloatingType> super;
			
		public:
			
			#if (__cplusplus >= 201406L)
				static constexpr FloatingType pi = 3.141592653589793;
			#endif
			
			CONSTEXPR FloatingPoint() noexcept  {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_arithmetic<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR FloatingPoint(Type floatNumber) : super(floatNumber) {}
			
			template <typename Type, typename = typename std::enable_if<
			std::is_arithmetic<Type>::value ||
			std::is_same<Type, __int128_t>::value ||
			std::is_same<Type, __uint128_t>::value>::type>
			CONSTEXPR FloatingPoint(const Number<Type>& number): super(number) {}
			
			
			template <typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value,bool>::type>
			CONSTEXPR FloatingType operator=(const Number<ArithmeticType>& number) noexcept  {
				super::set(number);
				return *this;
			}
			
			static FloatingPoint random(FloatingPoint lowerBound = std::numeric_limits<FloatingType>::denorm_min(),
									   FloatingPoint upperBound = std::numeric_limits<FloatingType>::max()) {
				
				std::random_device rd;
				std::mt19937_64 rng(rd());
				
				if (lowerBound > upperBound) { std::swap(lowerBound, upperBound); }
				std::uniform_real_distribution<FloatingType> randomValue(lowerBound, upperBound);
				
				return FloatingPoint(randomValue(rng));
			}
		};
		
		typedef FloatingPoint<float> Float;
		typedef FloatingPoint<float> Float32;
		typedef FloatingPoint<double> Float64;
		typedef FloatingPoint<long double> Float80;
		typedef FloatingPoint<double> Double;
		typedef FloatingPoint<long double> LongDouble;
	}
}

#undef CONSTEXPR
