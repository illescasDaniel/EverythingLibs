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
#include <iostream>
#include <vector>
#include <functional>

namespace evt {
	
	class UnitTest {
		
		std::string testName = "";
		
	public:
		
		UnitTest(const std::string& testName): testName(testName) { }
		
		void assert(bool condition, const std::string& errorMessage = "") {
			if (!condition) {
				if (errorMessage.empty() && this->testName.empty()) {
					std::cerr << "✗ TEST FAILED" << std::endl;
				}
				else if (errorMessage.empty()) {
					std::cerr << "✗ TEST FAILED: " << this->testName << std::endl;
				} else if (this->testName.empty()) {
					std::cout << "✗ TEST FAILED: \"" << errorMessage << "\"" << std::endl;
				} else {
					std::cout << "✗ TEST FAILED (\"" << this->testName << "\"): \"" << errorMessage << "\"" << std::endl;
				}
				
				exit(1);
			}
		}
		
		~UnitTest() {
			xTestPassed(this->testName);
		}
		
		static void testPassed(const std::string& testName = "") {
			if (testName.empty()) {
				std::cout << "\n✓ All TESTS PASSED" << std::endl;
			}
			else {
				std::cout << "\n✓ ALL TESTS PASSED [\"" << testName << "\"]\n" << std::endl;
			}
		}
		
		static void run(const std::string& testName, std::vector<std::function<void()>> tests) {
			for (const auto& test: tests) {
				test();
			}
			evt::UnitTest::testPassed(testName);
		}
		
		//
		
		static void xAssert(bool condition, const std::string& errorMessage = "", bool displayPassedTest = false) {
			if (!condition) {
				xTestFailed(errorMessage);
				exit(1);
			} else if (displayPassedTest) {
				xTestPassed();
			}
		}
		
		static void xTestPassed(const std::string& testName = "") {
			if (testName.empty()) {
				std::cout << "✓ TEST PASSED" << std::endl;
			} else {
				std::cout << "✓ TEST PASSED: \"" << testName << "\"" << std::endl;
			}
		}
		
		static void xTestFailed(const std::string& errorMessage = "") {
			if (errorMessage.empty()) {
				std::cerr << "✗ TEST FAILED" << std::endl;
			} else {
				std::cout << "✗ TEST FAILED: \"" << errorMessage << "\"" << std::endl;
			}
		}
	};
}
