//
//  main.cpp
//  Utils
//
//  Created by Daniel Illescas Romero on 07/07/2017.
//  Copyright © 2017 Daniel Illescas Romero. All rights reserved.
//

#include <iostream>
#include "include/EVT.hpp"
#include "fullClassExample/Car.hpp"
#include <thread>
#include <vector>

using namespace std;
using namespace EVT;

namespace evt {
	class Human: public Object {
	public:
		
		int age = 10;
		ReadOnly<string, Human> name = "Daniel";
		
		Human() {
			name = "test";
		}
		
		// Necessary for the "Array" class to extract a string from this object
		string toString() const override {
			return "Age: " + std::to_string(age) + "\nName: " + string(name); // You can use: evt::toString in c++17
		}
	};
}

class Stuff: public Object {
public:
	int a = 10, b = 90;
	
	Stuff() {}
	Stuff(int a, int b): a(a), b(b) {}
	
	string toString() const override {
		return "Things: " + std::to_string(a) + " " + std::to_string(b);
	}
};

class Test: public Stuff {
public:
	int c = 103, d = 89;
	
	typedef Stuff super;
	Test() {}
	
	string toString() const override {
		return super::toString() + " and " + std::to_string(c) + " " + std::to_string(d);
	}
};

class LazyThings {
public:
	
	size_t iterations = 1;
	size_t iterations2 = [&]{ return iterations*2; }(); // Non-lazy initialization
	
	LazyVar<float> lazyBenchmark { LazyVar<float>([&]{ // lazy initialization
		return benchmark([&]{
			Array<int> numbers;
			repeat(30000000, [&](const size_t& i){ numbers.append(int(i)); });
		}, iterations);
	})};
	
	LazyThings(size_t iterations_ = 1): iterations(iterations_) {}
};

class Person: public Comparable<Person>, public Object {
	
	static void setAge(uint& age, const uint& newAge) {
		if (newAge > 0 && newAge <= 150) (age = newAge);
	}
	
public:
	
	ReadOnly<string,Person> name;
	VarSetter<uint,Person> age {Person::setAge};
	
	const Var<uint> doubleAge {[&]{
		return this->age * 2;
	}};
	
	Person(){}
	Person(uint age, string name = "") {
		this->age = age;
		this->name = name;
	}
	
	bool operator==(const Person& person) const override {
		return this->age == person.age;
	}
	
	bool operator <(const Person& person) const override {
		return this->age < person.age;
	}
	
	string toString() const override {
		return "Person(age: " + std::to_string(age) + ")";
	}
};

void testFunction(UInt8 number) {
	print(number);
}

int main(int argc, char* argv[]) {
	
	// OVERFLOW exception :)
	testFunction(-1);
	
	print(Architecture::is64Bits);
	
	#if defined(__clang__) && INTPTR_MAX == INT64_MAX
	
        print(Int128(Int128::max), Int64::max);
	
		Int128 test_t23 = 9876545678987654;
		print(test_t23 * 100);
		print(test_t23*1000);
		print(test_t23*10000);
		print(test_t23*10000000);
	#endif
	#if (__cplusplus >= 201406L)
		PrintSettings::enableHighFloatingPointPrecision();
		print(Float::pi);
	#endif
	
	Int32 numberA = 3;
	print(numberA ^ 4, numberA.remainderDividingBy(2));
	
	Float numberB = 5.1;
	print(numberB.remainderDividingBy(3));
	
#if defined(__clang__) && INTPTR_MAX == INT64_MAX
	print(UInt128(20) ^ 3);
	Int128 test66(98765432345678);
	test66 *= test66;
	print(Int128(Int128::min), Int128(Int128::max));
	print(test66, test66.random());
#endif
	Int8 t0003(94);
	print(t0003, "to 2:", t0003 ^ 2);
	print(Int8::random());
	
	Person daniel1(10, "daniel");
	daniel1.age = 2;
	
	Person danielPerson(20);
	Person otherPerson(20);
	Person otherPerson2(10);
	
	print(danielPerson == otherPerson);
	print(danielPerson == otherPerson2);
	print(danielPerson > otherPerson2);
	print(danielPerson >= otherPerson);
	print(danielPerson > otherPerson);
	
	cout << sizeof(Person) << ' ' << danielPerson << endl;
	
	cout << isEquatable<Person>::value << endl;
	
	print(sizeof(Array<int>));
	
	for (constVar i: RangeUntil(10)) {
		print("Hi!", i);
	}
	
	Int8 test00001(89);
	UInt8 test0002(54);
	print(test00001, test0002);
	
	print(test00001.dividedBy(4));
	
	cout << sizeof(Array<int>) << " " << sizeof(unique_ptr<int[]>) << endl;
	
	Array<string> someStrings {"daniel", "test", "daniel", "john", "null", "macOS"};
	print(someStrings.countOf("Daniel"_lower));
	print(someStrings.countOf([](const string& str){ return str.length() > 4; }));
	print(someStrings.findIf([](const string& str){ return str == "test"; }));
	
	RawPointer<int> number0001(10);
	RawPointer<int> number0002(100);
	
	number0001 = std::move(number0002); // Uses move operations, deletes the pointer and content of "number0002"
	// number0001 = number0002; // Copies the content of the pointer, doesn't destroy the original ("number0002")
	
	print("This:", number0001);
	
	if (number0002.isNotNull()) {
		print(number0002);
	}
	
	RawPointer<Stuff[]> stuffs(3);
	stuffs[0] = Stuff(1, 2);
	print(stuffs[0]);
	
	LazyThings once;
	LazyThings twice(10);
	LazyThings third(4);
	
	twice = third; // Updates the iterations done in "lazyBenchmark"
	print(twice.iterations, twice.iterations2);
	twice.iterations = 9876; // DOESN'T update iterations2
	print(twice.iterations, twice.iterations2);
	
	float time{};
	thread threadWithLazyVariable ([&]{ time = twice.lazyBenchmark; });
	
	Pointer<int> aP(10);
	Pointer<int> bP(aP);
	Pointer<Int> cP(80);
	print("Value of pointer:", *cP);
	
	if (bP.isNotNull()) {
		print(bP);
	}
	
#if (cplusplusVersion >= cplusplus1z)
	Array<Any> things {"hola", 10, 5.1};
	print(things[0].as<string>());
	print(things[1].as<int>());
	print(things[2].as<double>());
	
	if (Optional<Any> thing = things.at(2)) {
		print(thing.value().as<double>());
	}
#endif
	
	Optional<const int> testOpt{10};
	
	Array<int> numbersArr {0,2,3,4,5};
	if (numbersArr.find(3) != numbersArr.count()) {
		print("Found!!");
	}
	
	print(numbersArr.filter([](const int& number){
		return number >= 3;
	}));
	
	print(numbersArr.first([](const int& number) {
		return number % 2 == 0;
	}));
	
	Optional<int> firstOdd = numbersArr.last([](const int& n){
		return n % 2 == 1;
	});
	print("wiii:", firstOdd.valueOr(-1));
	
	if (const auto firstOdd = numbersArr.first([](const int& n) { return n % 2 == 1; })) { // Optional
		print(firstOdd);
	}
	
	if (const auto lastEven = numbersArr.last([](const int& n) { return n % 2 == 0; })) {
		print(lastEven);
	}
	
	print(numbersArr.first(), numbersArr.last());
	
	Array<Stuff> stuff;
	stuff.append(Stuff(10, 20));
	print(stuff);
	
	print(Test());
	
	
#if (cplusplusVersion >= cplusplus1z) && __has_include(<string_view>)
	
	string name2 = "  Daniel2  ";
	
	StringView name3 = "hola, daniel";
	
	print("HEYYY:", name3);
	print(name3 * 4);
	
	constexpr StringView testName = "YEP"_sv;
	// static_assert(testName == "YEP"_sv, "Not equal strings"); [in gcc operator== is not constexpr]
	cout << testName << endl;
	
	cout << name3.findFirstNotOf(" ") << endl;
	cout << name3.count() - name3.findLastOf(" ") << endl;
	
	name3.trim(StringView::TrimMode::trimStart);
	cout << name3 << endl;
	cout << name3.trimmed() << endl;
	cout << name3.trimmed() << endl;
	
	cout << name3.contains("ani") << endl;
	
	cout << sizeof(name2) << endl;
	cout << sizeof(name3) << endl;
	
	//print("Hi\n"_sv * 3);
#endif
	
	RawPointer<int> ptest(100);
	RawPointer<int> ptest2(400);
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
	print(Array<Car>{toyotaCelica, toyotaCelica});
	
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
	print("Arguments:", args);
	
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
	
	cout << "Random: " << randomIntegralNumber(-90, -10) << endl;
	Int randomIntNumber = Int::random(0,100);
	cout << randomIntNumber << endl;
	cout << randomRealNumber(-10.3, 10.86) << endl;
	
	int numbers[3] {1,2,8};
	cout << arrayLengthOf(numbers) << endl;
	
	cout << trimmed(" hi ") << endl;
	
#if (cplusplusVersion >= cplusplus1z) && defined(__clang__)
	print("My name is", evt::toString(numbers));
#else
	//print(to_string(numbers, 3));
#endif
	
	Array<string> names {"daniel", "john", "mary"};
	Array<string> copyNames = names;
	print(copyNames);
	//print(copyNames.sorted());
	
	Array<double> numbersD {1.2, 8.6, 8};
	//numbersD.removeElements({8.6, 8});
	print(numbersD.capacity());
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
	cout << (otherName == "hola") << endl;
	if (otherName) {
		cout << otherName << endl;
	}
	string namesss = otherName;
	
	print(cplusplusVersion);
	print(cplusplus1z);
	
#if (cplusplusVersion >= cplusplus1z)
#if defined(__clang__)
	if (Optional<string> test("something"); test.orEmpty().length() > 2) {
		print("Test is not null, value:", test);
	}
#else
	Optional<string> test_("something");
	if (test_.isNotNull() && test_.orEmpty().length() > 2) {
		print("Test is not null, value:", test_);
	}
	
#endif
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
	
	PrintSettings::terminator = " ";
	print("Numbers:");
	for (const auto& number: pNumbers) {
		print(number);
	}
	PrintSettings::terminator = PrintSettings::defaultTerminator;
	
	Pointer<int[]> pNumbers2({1,2,3,4});
	cout << pNumbers2[2] << endl;
	
	pNumbers = pNumbers2; // Now pNumbers2 is empty (because it moved all its content to the other Pointer object)
	
	cout << pNumbers.capacity() << " " << pNumbers2.capacity() << endl;
	cout << pNumbers[2] << endl;
	
#if (cplusplusVersion >= cplusplus1z) && defined(__clang__)
	print(toStringContainer(pNumbers));
#endif
	
#if defined(__clang__)
	Int128 bigNumber = 234567876543256343;
	cout << (bigNumber + 1) << endl;
	cout << (2 * bigNumber) << endl;
#endif
	
	AnyNumber test (8745.54);
	cout << test.multiply<double>(10.0) << endl;
	
	test = 10;
	cout << test.as<int>() << endl;
	
	test = 5000000000;
	cout << test.as<size_t>() << endl;
	
	test = -100;
	cout << test.as<int>() << endl;
	
#if defined(__clang__)
	test = Int128(3465789543789654378);
	cout << test.as<Int128>() << endl;
	
	UInt128 reallyBigNumber = 0;
	reallyBigNumber += 2;
	
	test = reallyBigNumber;
	cout << test.as<UInt128>() << endl;
#endif
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
	
	File text("sample.txt");
	text.write("blablabla");
	
	threadWithLazyVariable.join();
	print(time);
	
	//int number2 = readLine<int>();
	//print(number2);
	
	/*
	 int myAge = input<int>("Age?: ");
	 print(myAge);
	 
	 
	 // Equivalent to:
	 int myAge_;
	 cout << "Age?: ";
	 cin >> myAge_;
	 // ...
	 */
	
	//string yourName = input("What is your name?: ");
	//print(yourName);
	
	// Equivalent to:
	/*
	 string yourName_;
	 cout << "What is your name?: ";
	 getline(cin, yourName_);
	 cout << yourName_ << endl;
	 */
	
	//UInt value;
	//print(value);
	// [UNDERFLOW :D] value -= 2;

}
