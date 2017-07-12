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

#include <ostream>

namespace evt {
	
	namespace numbers {
		
		class Int128 {
			__int128_t value_ {0};
		public:
			
			Int128() { }
			Int128(const __int128_t value_) {
				this->value_ = value_;
			}
			Int128(const Int128& number) {
				this->value_ = number.value_;
			}
			
			friend std::ostream& operator<<(std::ostream& dest, const Int128& number) {
				if (std::ostream::sentry(dest)) {
					__uint128_t tmp = number.value_ < 0 ? -number.value_ : number.value_;
					char buffer[128];
					char* d = std::end(buffer);
					do {
						d -= 1;
						*d = "0123456789"[tmp % 10];
						tmp /= 10;
					} while (tmp != 0);
					
					if (number.value_ < 0) {
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
			
			template <typename Type>
			inline operator Type() const {
				return value_;
			}
			
			#define internalOperator(operation) \
			template <typename Type> \
			Int128 operator operation (Type otherNumber) const { \
				Int128 newNumber(this->value_ operation otherNumber); \
				return newNumber; \
			}
			
			#define externalOperator(operation) \
			template <typename Type> \
			friend Int128 operator operation (Type number, Int128 otherNumber) { \
				Int128 newNumber(otherNumber operation number); \
				return newNumber; \
			}
			
			#define assignmentOperator(operation) \
			template <typename Type> \
			Int128& operator operation (Type otherNumber) { \
				this->value_ operation otherNumber; \
				return *this; \
			}
			
			internalOperator(+); internalOperator(-); internalOperator(*); internalOperator(/); internalOperator(%);
			externalOperator(+); externalOperator(-); externalOperator(*); externalOperator(/); externalOperator(%);
			assignmentOperator(+=); assignmentOperator(-=); assignmentOperator(*=); assignmentOperator(/=);
		};
	}
}
