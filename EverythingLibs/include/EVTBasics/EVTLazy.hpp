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

#include <functional>
#include <stdexcept>

#if (__cplusplus > 201103L)
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

// Based on "James Hugard" Lazy class example: https://stackoverflow.com/a/17642967/6303785

namespace evt {
	
	// Exception thrown on attempt to access an uninitialized Lazy
	struct uninitializedLazy: public std::runtime_error {
		uninitializedLazy(): std::runtime_error("Uninitialized lazy value") {}
	};

	template<typename Type>
	class Lazy {
		
	private:
		
		mutable std::unique_ptr<Type> value_ {nullptr};
		mutable bool initialized {false};
		std::function<Type()> initializer;
		std::function<void(Type&)> deinitializer;
		
		CONSTEXPR void construct(const Type& value) const {
			destroy();
			this->value_ = std::move(std::unique_ptr<Type>(new Type{value}));
			this->initialized = true;
		}
		
		CONSTEXPR void destroy() const {
			if (initialized) {
				deinitializer(*value_);
				initialized = false;
			}
		}
		
		// Throw exception on attempt to access uninitialized lazy
		static Type defaultInitializer() {
			throw uninitializedLazy();
		}
		
		static void defaultDeinitializer(Type&) {}
		
	public:
		
		CONSTEXPR Lazy(): initializer(defaultInitializer), deinitializer(defaultDeinitializer) {}
		
		CONSTEXPR Lazy(const Type& v): initializer(defaultInitializer), deinitializer(defaultDeinitializer) {
			construct(v);
		}
		
		CONSTEXPR Lazy(std::function<Type()> initializer, std::function<void(Type&)> deinitializer = defaultDeinitializer)
		: initializer(initializer), deinitializer(deinitializer) {}
		
		CONSTEXPR Lazy(const Lazy& otherLazy): initializer(otherLazy.initializer), deinitializer(otherLazy.deinitializer) {
			if (otherLazy.initialized) {
				construct(*otherLazy.value_);
			}
		}
		
		CONSTEXPR Lazy& operator=(const Lazy<Type>& otherLazy) {
			destroy();
			initializer = otherLazy.initializer;
			deinitializer = otherLazy.deinitializer;
			
			if (otherLazy.initialized) {
				construct(*otherLazy.value_);
			}
			return *this;
		}
		
		CONSTEXPR Type& operator=(const Type& value ) {
			construct(value);
			return *value_;
		}
		
		// Implicitly force initialization and answer with value
		CONSTEXPR operator Type&() const {
			return force();
		}
		
		// Get pointer to storage of T, regardless of initialized state
		CONSTEXPR Type* operator &() const {
			return value_;
		}
		
		CONSTEXPR bool isInitialized() const {
			return initialized;
		}
		
		// Force initialization state to true, e.g. if value initialized directly via pointer
		CONSTEXPR void forceInitialized() {
			initialized = true;
		}
		
		// Force initialization, if not already done, and answer with the value
		// Throws exception if not implicitly or explicitly initialized
		CONSTEXPR Type& force() const {
			if (!initialized) {
				construct(initializer());
			}
			return *value_;
		}
		
		~Lazy() {
			destroy();
		}
	};
}
