//
//  EVTProtocols.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 30/08/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include <type_traits>

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
	
	namespace protocols {
	
		template <typename Type>
		class Equatable {
		public:
			virtual bool operator==(const Type& rhs) const = 0;
			virtual bool operator!=(const Type& rhs) const { return !(this->operator==(rhs)); };
		};
		
		template <typename Type>
		class Iterable {
		public:
			virtual Type* begin() const = 0;
			virtual Type* end() const = 0;
		};
		
		template <typename Type>
		class Comparable {
		public:
			
			virtual bool operator==(const Type& rhs) const = 0;
			virtual bool operator<(const Type& rhs) const = 0;
			
			virtual bool operator<=(const Type& rhs) const { return this->operator<(rhs) or this->operator==(rhs); };
			virtual bool operator>(const Type& rhs) const { return !this->operator<(rhs) and !this->operator==(rhs); };
			virtual bool operator>=(const Type& rhs) const { return this->operator>(rhs) or this->operator==(rhs); };
			virtual bool operator!=(const Type& rhs) const { return !this->operator==(rhs) and !this->operator<(rhs) and !this->operator>(rhs); };
		};
		
		class CustomStringConvertible {
		public:
			
			virtual std::string toString() const = 0;
			
			friend std::ostream& operator<<(std::ostream& os, const CustomStringConvertible& object) noexcept {
				return os << object.toString();
			}
		};
		
		//
		
		template <typename Type>
		struct isEquatable {
			static CONSTEXPRVar bool value = std::is_base_of<Equatable<Type>,Type>::value || std::is_base_of<Comparable<Type>,Type>::value;
		};
		
		template <typename Type>
		struct isComparable {
			static CONSTEXPRVar bool value = std::is_base_of<Comparable<Type>,Type>::value;
		};
		
		#define HAS_METHOD_DEF(xxx)                            \
		template<typename T>                                   \
		static constexpr auto has_ ## xxx ## _method(int)      \
		-> decltype(std::declval<T>(). xxx (), bool())         \
		{ return true; }                                       \
		template<typename T>                                   \
		static constexpr bool has_ ## xxx ## _method(...)      \
		{ return false; }
		
		template <typename Type, typename TypeBase = void>
		struct isIterable {
			
		private:
			HAS_METHOD_DEF(begin);
			HAS_METHOD_DEF(end);
			
		public:
			static CONSTEXPRVar bool value = std::is_void<TypeBase>::value ?
			(has_begin_method<Type>(1) && has_end_method<Type>(1)) :
			std::is_base_of<Iterable<Type>,TypeBase>::value;
		};
		
		#undef HAS_METHOD_DEF
	}
}
