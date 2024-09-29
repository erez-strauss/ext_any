## ext::any Available at:
The ext::any github project: https://github.com/erez-strauss/ext_any/

The CppCon 2024 Open Content presentation slides:
[ext_any_presentation_CppCon2024_OpenContent_u20240930.pdf](https://github.com/user-attachments/files/17180136/ext_any_presentation_CppCon2024_OpenContent_u20240930.pdf)

Example on compiler explorer: https://compiler-explorer.com/z/5ErEPbEah

## Readme: ext::any\<size_t N, template\<typename> class ... Features>



ext::any<N, Features ...> - is an owning container type that holds any type the programmers place into it.
 if the placed item fits into N bytes, it will be stored in place without dynamic memory allocation,
 otherwise it will be stored on the heap.

The standard std::any provides a similar storing and accessing capabilities, which were the base for the ext::any<> type definitions.
The standard std::any requires the programmer to get the specific type in order to act on the stored value.

The ext::any::any<> provides the framework to use efficiently type erasure to act on the stored value and do generic operations on the
stored values without knowing which specific type is stored in the ext::any<>

Different features can be activated as part of the specific ext::any, there are few defined features,
while the user can define their own new features to be included in the ext::any types.


for example the following code works:

```c++
// example1.cpp
ext::any::any<16, Streamed> a0 (125);
std::cout << "a0: " << a0 << '\n';  // prints: a0: 125
a0 = "hello world";
std::cout << "a0: " << a0 << '\n';  // prints: a0: hello world
```
Usage example code


```c++
#include <any/any.h>

using namespace ext::any;

using any_one = any_type<16, af_streamed, af_strict_hash, af_strict_eq>; // Movable, Copyable - default.

any_one a0 {};
any_one a1 { std::type_inplace<double>, 111};  // holds a double.
any_one a2 { a0 };
any_one a3 { std::move ( a1 )}; // a3 double with value 111, a1 has no value.
// any_one a4 = { 1, 2.3, "bla bla" }; // a4 holds vector of any_one, each initialized. a4.size() return the vector size. -- tobe implemented.

auto a5 = a4;  // copy the vector
auto a6 = std::move(a4); // moves the content of the a4 vector, a4 has no value.
```


## RTTI - Run Time Type Information

C++ supports runtime type information, available at compile time through typeid().

The ext::any supports the type() function when RTTI is available, which is the default on most systems

On systems without RTTI, for example when compiling the code with '-fno-rtti' the ext::any will provide all
its functionality which do not depend on the RTTI availability, including the cast_any<T>().
If you need this functionality, see the no-rtti preset config.


## std::any

The std::any is used only to store and retrieve object of different type, without a simple way to add actions to the type.

## ext::any

The ext::any::any<> provides the following features:

1. ext::af_streamed – adds support for operator '<<' -- relaxed, print nothing if type is not printable
1. ext::af_strict_streamed - does not accept values that do not support ‘<<’ -- all objects inside the any<> must have the '<<' operator on them.
1. ext::af_strict_less – add support for less ‘<’ compare --  all object in any<> have '<' less than operator, so the any<> can be used in std::map<> or std::set<>
1. ext::af_strict_eq - add support for ‘==’
1. ext::af_strict_inplace - prevents using dynamic memory allocation
1. ext::af_strict_hash – support hash value -- all objects inside the any<> can generate hash value, so this any<> variables can be used as key
   for std::unordered_set<> and std::unordered_map<>
1. ext::af_variant - restrict the values to specific types
1. ext::af_func - support operator ’()’ with different Args, not implemented yet
1. ext::af_strict_add - the plus '+' operator -- the type in the any<> must have plus operator defined 

The following are features that not yet implemented 
1. ext::af_allocator<T> - use specific allocator to allocate the object types in case we need dynamic heap allocation.
2. ext::af_vector - the any<> can contain a vector of any<> - so it can hold sub element, and has the function size() which returns the vector size
3. ext::af_func<Args> - enable the any to hold functions and have the function call operator.
4. ext::af_map - Add the insert(K,V) - so an any<> can hold a map of elements, a pair of key and value which are also any<>


Than with a compile time size of the small object optimization.
ext::any<8> - maximum size of 8 bytes of inplace stored data.
ext::any<32> - maximum size of the inplace stored data is 32.
the size of the ext::any<N> is N+8 bytes, as any need to hold a pointer to const struct with the support operations.
The alignment requirements of the ext::any is by default 8 bytes.

Pay attention to std::in_place_type - as constructor in place for std::any 

# Standard std::any API - basic functionality - base functionality for ext::any
  https://en.cppreference.com/w/cpp/utility/any
1. constructors - empty, with value, with tag, with tag and value(s)
2. assignment '=' from T types, or from Any, of the same template parameters, from Any of different template parameters.
3. destructor - should call the appropriate type destructor
3. emplace
4. reset
5. swap
6. has_value
7. type - returns typeid
8. any_cast - friend - two cases, with pointer null if wrong type, T value throw if now correct type.
1. constructor with any value type
2. Constructor with type tag and optional value
3. any_cast<T> - returns a reference to the type or throw exception
4. any_cast<T*> - returns a pointer to the value or nullptr
5. Is there? a similar to visit call one of given functors in an object selected by the content type?
6. not required to provide SSO, with any given size.
7. std::make_any<T>()

Basic new functionality, before the Features ...
1. type_name
2. is_dynamic() / is_inplace()

# Programming Level ext::any tasks:

1. add test, compare to std::any, verify / define af_* features.
2. Add more google-tests, check for memory leaks, using sanitizers.
3. add internal vector\<ext::any\> for std::initializer_list
4. add Small String optimization zstring. to hold upto (N-1) null terminated string
5. assignment from "literal string" - to convert to std::string
6. copy / move, constructor / assignment from other ext::any<M, ...> 


# Requirement / Goals for the 
1. All functionality of std::any (with cast_any() returning reference instead of value)
2. SSO for size N.
3. Additional functionality for different features:
   5. formattable - support format ("this is the value in any: {}", a0);
   6. comparison
   7. initialisation - from initializer list - where each item is an any by itself ...
   8. Vector container - add size function which tell if there is single data item or multiple any items. ( or other items? )
   9. Map 
   10. Func<R, ()>
   11. user defined features.


# Project level todo:

1. Change the type name of the ext::any , please suggest in the github discussions. 
1. Volunteers are welcome
2. Write emplace() that support `std::in_place_type`
2. Add more compiler-explorer examples, for example: https://compiler-explorer.com/z/5ErEPbEah 
2. Update this README.md 
1. Define ext::any goals and more use cases
2. write more examples
3. Test / port to different platforms, Windows ...
2. As it is not just the base std::any API:
      3. constructor with value
      4. constructor with tagged_type - the standard one and our own.
      5. bool has_value()
      6. reset
      7. non const access to the value_type
      8. type() - return type_id
    
8. What features of std::variant we want to provide?
  visitor type access. ? problematic as at compile time no effective way to know what types are expected
9. packaging a vcpkg 

# list of functionality to be tested
```
Testing Basic Standard Functionality:
Default constructor - empty: has_value() - false, casting is failing, and type() returns typeinfo<void>
 std::any
With small int
With larger data type.
 
any::~any

Observers
any::has_value
any::type

any::operator=  -- replace value , verify no memory leaks with valgrind.
   assign a value, and assign another any.
   
Modifiers
   any::emplace
   any::reset
   any::swap

Non-member functions
   swap(std::any)
   any_cast
   make_any
   Helper classes
   bad_any_cast
```
 

## List other any implementations:
   1. Boost Any -- https://www.boost.org/doc/libs/1_85_0/doc/html/any.html
   2. poco Any -- https://docs.pocoproject.org/current/Poco.Any.html
   3. llvm libc++ any -- https://github.com/llvm/llvm-project/blob/main/libcxx/include/any
   4. gcc libstdc++ any -- https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/any

#