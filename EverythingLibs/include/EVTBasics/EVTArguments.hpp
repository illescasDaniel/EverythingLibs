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

#include <string>
#include <stdexcept>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

namespace evt {
	
	class Arguments {
		
		std::unique_ptr<std::string[]> args;
		std::size_t size_ {};
		
	public:
		
		CONSTEXPR Arguments(const int& argc, char* argv[], bool loadFirstArg = false) {
			this->size_ = loadFirstArg ? argc : argc-1;
			this->args = std::unique_ptr<std::string[]>(new std::string[size_]);
			
			for (std::size_t i = 0; i < size_; ++i) {
				args[i] = argv[loadFirstArg ? i : i+1];
			}
		}
		
		std::string& operator[](const size_t& pos) const {
			if (pos >= size_) { throw std::out_of_range("Index out of range"); }
			return args[pos];
		}
		
		std::size_t size() const {
			return size_;
		}
		
		std::string toString() const {
			
			std::string output = "[";
			
			for (std::size_t i = 0; i < this->size(); ++i) {
				output += (*this)[i] + ((i+1 < this->size()) ? (std::string(", ")) : (std::string("")));
			}
			
			output += "]";
			
			return output;
		}
		
		friend std::ostream& operator<<(std::ostream& os, const Arguments& arguments) {
			return os << arguments.toString();
		}
	};
}

#undef CONSTEXPR
