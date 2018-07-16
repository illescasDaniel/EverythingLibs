//
//  EVTReflection.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 06/05/2018.
//  Copyright © 2018 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include <cstdint>

namespace evt {
	namespace reflection {
		
		// Parameters count: https://stackoverflow.com/a/36798101/6303785
		template <class R, class... ARGS>
		struct function_ripper {
			static constexpr std::size_t n_args = sizeof...(ARGS);
		};
		
		
		template <class R, class... ARGS>
		auto constexpr make_ripper(R (ARGS...) ) {
			return function_ripper<R, ARGS...>();
		}
		
		void foo(int, double, const char*);
		
		template <typename Type>
		constexpr std::size_t parametersCountOf(Type function) {
			return decltype(make_ripper(function))::n_args;
		}
		
		// Type name: https://stackoverflow.com/a/24784469/6303785
		template <const char* str, int len, char... suffix>
		struct append {
			static constexpr const char* value() {
				return append<str, len-1, str[len-1], suffix...>::value();
			}
		};
		
		template <const char* str, char... suffix>
		struct append<str, 0, suffix...> {
			static const char value_str[];
			static constexpr const char* value() {
				return value_str;
			}
		};
		
		template <const char* str, char... suffix>
		const char append<str, 0, suffix...>::value_str[] = { suffix..., 0 };
		
		
		template <typename T>
		struct base_typename_struct;
		
		template <> struct base_typename_struct<std::size_t> { static constexpr const char name[] = "size_t"; };
		template <> struct base_typename_struct<std::uint8_t> { static constexpr const char name[] = "uint8_t"; };
		template <> struct base_typename_struct<std::uint16_t> { static constexpr const char name[] = "uint16_t"; };
		template <> struct base_typename_struct<std::uint32_t> { static constexpr const char name[] = "uint32_t"; };
		template <> struct base_typename_struct<std::uint64_t> { static constexpr const char name[] = "uint64_t"; };
		template <> struct base_typename_struct<std::int8_t> { static constexpr const char name[] = "int8_t"; };
		template <> struct base_typename_struct<std::int16_t> { static constexpr const char name[] = "int16_t"; };
		template <> struct base_typename_struct<std::int32_t> { static constexpr const char name[] = "int32_t"; };
		template <> struct base_typename_struct<std::int64_t> { static constexpr const char name[] = "int64_t"; };
		template <> struct base_typename_struct<float> { static constexpr const char name[] = "float"; };
		template <> struct base_typename_struct<double> { static constexpr const char name[] = "double"; };
		template <> struct base_typename_struct<long double> { static constexpr const char name[] = "long double"; };
		template <> struct base_typename_struct<char> { static constexpr const char name[] = "char"; };
		template <> struct base_typename_struct<const char> { static constexpr const char name[] = "const char"; };
		template <> struct base_typename_struct<char16_t> { static constexpr const char name[] = "char16_t"; };
		template <> struct base_typename_struct<char32_t> { static constexpr const char name[] = "char32_t"; };
		template <> struct base_typename_struct<std::string> { static constexpr const char name[] = "std::string"; };
		
#if __has_include(<string_view>)
		template <> struct base_typename_struct<std::string_view> { static constexpr const char name[] = "std::string_view"; };
#endif
		
#if __has_include("EVTNumbers/EVTSafeNumber.hpp") ||  __has_include("EVTSafeNumber.hpp")
		template <> struct base_typename_struct<numbers::safe::size_t> { static constexpr const char name[] = "size_t"; };
		template <> struct base_typename_struct<numbers::safe::uint8_t> { static constexpr const char name[] = "uint8_t"; };
		template <> struct base_typename_struct<numbers::safe::uint16_t> { static constexpr const char name[] = "uint16_t"; };
		template <> struct base_typename_struct<numbers::safe::uint32_t> { static constexpr const char name[] = "uint32_t"; };
		template <> struct base_typename_struct<numbers::safe::uint64_t> { static constexpr const char name[] = "uint64_t"; };
		template <> struct base_typename_struct<numbers::safe::int8_t> { static constexpr const char name[] = "int8_t"; };
		template <> struct base_typename_struct<numbers::safe::int16_t> { static constexpr const char name[] = "int16_t"; };
		template <> struct base_typename_struct<numbers::safe::int32_t> { static constexpr const char name[] = "int32_t"; };
		template <> struct base_typename_struct<numbers::safe::int64_t> { static constexpr const char name[] = "int64_t"; };
#endif
		
		template <typename T, char... suffix>
		struct typename_struct {
			typedef base_typename_struct<T> base;
			static const char* name() {
				return append<base::name, sizeof(base::name)-1, suffix...>::value();
			}
		};
		
		template <typename T, char... suffix>
		struct typename_struct<T*, suffix...>:
		public typename_struct<T, '*', suffix...> {
		};
		//
	}
	
	template <typename Type>
	constexpr const char* typeOf(const Type) {
		return reflection::typename_struct<Type>::name();
	}
		// Alternative less powerful than the above
		/*template <typename Type>
		constexpr const char* typeOf(const Type value) {
			
			if constexpr(std::is_same<Type, size_t>()) {
				return "size_t";
			} else if constexpr(std::is_same<Type, uint8_t>()) {
				return "uint8_t";
			} else if constexpr(std::is_same<Type, uint16_t>()) {
				return "uint16_t";
			} else if constexpr(std::is_same<Type, uint32_t>()) {
				return "uint32_t";
			} else if constexpr(std::is_same<Type, uint64_t>()) {
				return "uint64_t";
			} else if constexpr(std::is_same<Type, int8_t>()) {
				return "int8_t";
			} else if constexpr(std::is_same<Type, int16_t>()) {
				return "int16_t";
			} else if constexpr(std::is_same<Type, int32_t>()) {
				return "int32_t";
			} else if constexpr(std::is_same<Type, int64_t>()) {
				return "int64_t";
			} else if constexpr(std::is_same<Type, float>()) {
				return "float";
			} else if constexpr(std::is_same<Type, double>()) {
				return "double";
			} else if constexpr(std::is_same<Type, long double>()) {
				return "long double";
			} else if constexpr(std::is_same<Type, char>()) {
				return "char";
			} else if constexpr(std::is_same<Type, unsigned char>()) {
				return "unsigned char";
			} else if constexpr(std::is_same<Type, char16_t>()) {
				return "char16_t";
			} else if constexpr(std::is_same<Type, char32_t>()) {
				return "char32_t";
			} else if constexpr(std::is_same<Type, std::string>()) {
				return "string";
			} else if constexpr(std::is_pointer<Type>()) {
				return typeOf(*value);
				//return "pointer";
			}
			
			return "Unknown";
		}*/
	
}
