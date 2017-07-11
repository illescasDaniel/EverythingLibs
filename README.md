# Everything Libraries

[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/EverythingLibs/blob/master/LICENCE)

A few libraries that will make the programmer's life easier 😉.

Libraries:
----------

- **EVTUtils**: few simple, yet useful, functions.

```c++
cplusplusVersion (macro)
cplusplus1z (macro)
cplusplus14 (macro)
cplusplus11 (macro)

static inline bool isOdd(const Type x)
static inline bool isEven(const Type x)

uint64_t randomNumber(uint64_t upperLimit = std::mt19937_64::max())
uint64_t randomDistributedNumber(uint64_t lowerBound = std::mt19937_64::min(), uint64_t upperBound = std::mt19937_64::max())

size_t arrayLengthOf(const Array& array)

inline void leftTrim(std::string &s)
inline void rightTrim(std::string &s)
inline void trim(std::string &s)
inline std::string trimmed(std::string s)

std::string operator "" _upper(char const * str, [[maybe_unused]] std::size_t size)
std::string operator "" _lower(char const * str, [[maybe_unused]] std::size_t size)
std::string toUpper(std::string str)
std::string toLower(std::string str)
Array<std::string> toUpperContainer(const Array<std::string>& strings)
Array<std::string> toLowerContainer(const Array<std::string>& strings)

Type readLine()

std::string quoted(std::string str)

float benchmark(const Function& function, size_t iterations = 1)
```

- **EVTBasics**: a set of classes that makes easier to manage some data.

```c++
Any // Manage any type of data
Arguments // Makes easier to use the program arguments by using an array
Array // An array implementation that is faster and prettier that std::vector
File // Read & write files in text or binary mode easily
Optional // Manage optional values that can be null or the internal type
Pointer // Easier and powerful way to use pointer is C++
Print // Print any container (including classic arrays, matrices, pointer arrays...)
ReadOnly // Manage values that can only be modified inside the class but can be accesed outside of it
```

Github links to some of the libraries:
- Array.hpp: https://github.com/illescasDaniel/Array.hpp
- File: https://github.com/illescasDaniel/File
- Optional: https://github.com/illescasDaniel/Optional
- Print: https://github.com/illescasDaniel/print.hpp
- Var & ReadOnly: https://github.com/illescasDaniel/Var-ReadOnly
<br>

- **EVTNumbers**: classes related to numbers and mathematics.

```c++
Int128 // Easier way to use __int128_t (integer of 128bits)
Number // You can assign any arithmetical type to an object of this type
```

License [[MIT]](https://github.com/illescasDaniel/EverythingLibs/blob/master/LICENCE):
----------

You are allowed to modify, distribute, make commercial or private use of all of these libraries.<br>
The libraries are provied without warranties of any kind.<br>
The only requierement is the preservation of copyright and license notices.<br>
