//
//  main.cpp
//  Utils
//
//  Created by Daniel Illescas Romero on 07/07/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#include <iostream>
#include "include/EVT.hpp"
#include "classExamples/Car.hpp"

using namespace std;
using namespace evt;
using namespace evt::utils;
using namespace evt::numbers;

namespace evt {
	class Human: public EVTObject {
	public:
		
		int age = 10;
		ReadOnly<string, Human> name = "Daniel";
		
		Human() {
			name = "test";
		}
		
		// Necessary for the "Array" class to extract a string from this object
		string toString() const override {
			return "Age: " + std::to_string(age) + "\nName: " + string(name);
		}
	};
}

int main(int argc, char* argv[]) {
	
	boolalpha(cout);
	
	#if (cplusplusVersion >= cplusplus1z)
	
		string name2 = "  Daniel2  ";
		String name3 = name2;
		constexpr String testName = "YEP"_sv;
		// static_assert(testName == "YEP"_sv, "Not equal strings"); [in gcc operator== is not constexpr]
		cout << testName << endl;
	
		cout << name3.find_first_not_of(" ") << endl;
		cout << name3.count() - name3.findLastOf(" ") << endl;
	
		name3.trim(String::TrimMode::start);
		cout << name3 << endl;
		cout << name3.trimmed() << endl;
		cout << name3.trimmed() << endl;
	
		cout << name3.contains("ani") << endl;
	
		cout << sizeof(name2) << endl;
		cout << sizeof(name3) << endl;
	
	#endif
	
	ThinPointer<int> ptest(100);
	ThinPointer<int> ptest2(400);
	ptest = ptest2;
	if (!ptest.isNull()) {
		cout << *ptest << endl;
	}
	
	Car toyotaCelica;
	toyotaCelica.model = "Toyota Celica";
	// NOPE -> toyotaCelica.wheels = 10;
	toyotaCelica.owner = "Daniel";
	print(toyotaCelica.wheels);
	print(toyotaCelica.toString());
	
	Car nissanGT("Nissan GT");
	print(nissanGT);
	
	Pointer<int[]> numbers001 {1,2,3,4,5,6};
	Pointer<int[]> numbers002 (numbers001.capacity() + 1);
	int extraNumber = 7;
	
	numbers002.moveValuesFrom(numbers001); // move(numbers001.begin(), numbers001.end(), numbers002.begin());
	numbers002[numbers002.capacity()-1] = extraNumber;	
	numbers001 = numbers002;
	
	cout << numbers001.capacity() << endl;
	cout << numbers001[numbers001.capacity()-1] << endl;
	cout << numbers001[1] << endl;
	
	Arguments args(argc, argv);
	print(args.size());
	print(args);
	
	Array<Human> humansTest;
	humansTest.append(Human());
	print(Human());
	print(humansTest);
	
	int a = 400;
	xAssert(a > 80, "bad");
	
	Human daniel;
	cout << daniel.name << endl;
	print(daniel);
	
	// NOPE -> daniel.name = "Test";
	
	cout << randomNumber(100) << endl;
	cout << randomDistributedNumber(90, 200) << endl;
	
	int numbers[3] {1,2,8};
	cout << arrayLengthOf(numbers) << endl;
	
	cout << trimmed(" hi ") << endl;
	
	print("My name is", to_string(numbers));
	
	Array<string> names {"daniel", "john", "mary"};
	Array<string> copyNames = names;
	print(copyNames);
	//print(copyNames.sorted());
	
	Array<double> numbersD {1.2, 8.6, 8};
	print(numbersD);
	print(numbersD.sorted());
	
	Array<int> numbersDD {4,2,4};
	print(numbersDD.sorted([](int& a, int& b){ return a >= b; }));
	
	print("Names: ", names);
	
	Optional<Array<string>> optionalNames(Array<string>{"daniel", "test"});
	print("Names2: ", optionalNames);
	
	print("Uppercased names:", toUpperContainer(names));
	
	string otherNames[] = {"aCa", "pOp"};
	print("Uppercased names:", toUpperContainer(otherNames));
	print("Uppercased names:", toUpperContainer({"aa", "bb"}));
	
	print("Hello everyone!"_upper);
	
	Optional<string> otherName;
	if (otherName) {
		cout << otherName << endl;
	}
	string namesss = otherName;
	
	print(cplusplusVersion);
	print(cplusplus1z);
	
	#if (cplusplusVersion >= cplusplus1z)
		if (Optional<string> test("something"); test.orEmpty().length() > 2) {
			print("Test is not null, value:", test);
		}
	#endif
	
	cout << benchmark([]{
		
		Array<size_t> numbers;
		
		for (size_t i = 0; i < 1000000; i++) {
			numbers.append(i);
		}
		
	}) << endl;
	
	Pointer<int> number(900);
	cout << *number << endl;
	
	Pointer<int[]> pNumbers(3);
	pNumbers[0] = 1;
	pNumbers[1] = 2;
	pNumbers[2] = 3;
	
	print(pNumbers.capacity());
	
	for (const auto& number: pNumbers) {
		print("Number:", number);
	}
	Pointer<int[]> pNumbers2({1,2,3,4});
	cout << pNumbers2[2] << endl;
	
	pNumbers = pNumbers2; // Now pNumbers2 is empty (because it moved all its content to the other Pointer object)
	
	cout << pNumbers.capacity() << " " << pNumbers2.capacity() << endl;
	cout << pNumbers[2] << endl;
	
	print(to_string(pNumbers));
	
	Int128 bigNumber = 234567876543256343;
	cout << (bigNumber + 1) << endl;
	cout << (2 * bigNumber) << endl;
 
	Number test (8745.54);
	cout << test.multiply<double>(10.0) << endl;
	
	test = 10;
	cout << test.as<int>() << endl;
	
	test = 5000000000;
	cout << test.as<size_t>() << endl;
	
	test = -100;
	cout << test.as<int>() << endl;
	
	test = Int128(3465789543789654378);
	cout << test.as<Int128>() << endl;
	
	UInt128 reallyBigNumber = 0;
	reallyBigNumber -= 2;
	
	test = reallyBigNumber;
	cout << test.as<UInt128>() << endl;
	
	Any something ("hello");
	cout << something.as<string>() << endl;
	
	something = 10.90;
	cout << something.as<double>() * 2 << endl;
	
	something = 80;
	cout << something.as<int>() << endl;
	
	guard(something.as<int>() == 80) else {
		return 1;
	}
	
	cout << isPrime(8078756) << endl;
	
	const char* testChar = "hello!!";
	cout << quoted(testChar) << endl;
	
	//int number2 = readLine<int>();
	//print(number2);*/
}
