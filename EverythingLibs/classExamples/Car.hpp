//
//  Car.hpp
//  EverythingLibs
//
//  Created by Daniel Illescas Romero on 12/07/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#pragma once

#include "../include/EVT.hpp"

class Car: public evt::EVTObject {
public:
	
	std::string model {};
	evt::ReadOnly<size_t,Car> wheels {4};
	bool forSale {true};
	evt::Optional<std::string> owner;
	
	Car() { }
	Car(std::string model, evt::Optional<std::string> owner = nullptr, size_t wheels = 4) {
		
		this->model = model;
		this->owner = owner;
		
		if (wheels > 0 and wheels < 12) {
			this->wheels = wheels;
		}
	}
	
	std::string toString() const override {
		std::string finalOwner = (this->owner).orEmpty().empty() ? "" : "\nOwner: " + this->owner.value();
		return "Model: " + model + "\nWheels: " + std::to_string(wheels) + finalOwner;
	}
};
