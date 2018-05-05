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

// Basic libraries (the other libraries should work without re-including this, but just in case you need them anyway)
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <memory>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <utility>
#include <typeinfo>

// Own Libraries
#include "EVTUtils.hpp"
#include "EVTBasics.hpp"
#include "EVTNumbers.hpp"
#include "EVTProtocols.hpp"
#include "EVTObject.hpp"

namespace EVT {
	using namespace evt;
	using namespace evt::utils;
	using namespace evt::numbers;
	using namespace evt::protocols;
}
