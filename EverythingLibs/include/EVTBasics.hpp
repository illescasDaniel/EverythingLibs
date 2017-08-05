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

// namespace: evt

#include "EVTBasics/EVTAny.hpp"
#include "EVTBasics/EVTArguments.hpp"
#include "EVTBasics/EVTArray.hpp" // ~C++17 Recommended
#include "EVTBasics/EVTFile.hpp"
#include "EVTBasics/EVTOptional.hpp"
#include "EVTBasics/EVTPointer.hpp"
#include "EVTBasics/EVTThinPointer.hpp"
#include "EVTBasics/EVTPrint.hpp" // Might be a little 'heavy' to import, you can comment this out this library if you don't really need it
#include "EVTBasics/EVTReadOnly.hpp"
#include "EVTBasics/EVTLazy.hpp"

#if __cplusplus >= 201406L && __has_include(<string_view>) // ~C++17 Libraries
	#include "EVTBasics/EVTString.hpp"
#endif
