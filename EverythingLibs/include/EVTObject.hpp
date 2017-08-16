//
//  EVTObject.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 12/07/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include <string>
#include <ostream>

namespace evt {
	
	class EVTObject {
	public:
		
		virtual std::string toString() const = 0;
		
		friend std::ostream& operator<<(std::ostream& os, const EVTObject& object) noexcept {
			return os << object.toString();
		}
	};
}
