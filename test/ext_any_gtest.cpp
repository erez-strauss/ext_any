#include <ext/any.h>
#include <exception>
#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

template<typename A> void test_standard_any_features()
{
    std::cout << "testing type: " << ext::src_type_name<A>() << '\n';
    A a0{};

    EXPECT_EQ(a0.has_value(), false);

    {
        try
        {
            auto z = std::any_cast<int>(a0);
            std::cout << z << '\n'; // unreached code as it is expected to throw
        }
        catch (std::exception& e)
        {
            std::cout << "Got excepted exception1: " << e.what() << '\n';
        }
        auto p0 = any_cast<int>(&a0);
        EXPECT_EQ(p0, nullptr);
        EXPECT_EQ(std::type_index(typeid(void)), std::type_index(a0.type()));
        EXPECT_NE(std::type_index(typeid(int)), std::type_index(a0.type()));
    }

    a0 = 3;
    EXPECT_EQ(a0.has_value(), true);

    {
        try
        {
            auto z0 = std::any_cast<int>(a0);
            EXPECT_EQ(z0, 3);
            auto z1 = std::any_cast<long>(a0);
        }
        catch (std::exception& e)
        {
            std::cout << "Got excepted exception2: " << e.what() << '\n';
        }
        auto p0 = any_cast<int>(&a0);
        EXPECT_EQ(*p0, 3);
        EXPECT_EQ(std::type_index(typeid(int)), std::type_index(a0.type()));
    }

    struct Data
    {
        long ldata[20]{};
    };
    Data d0{1, 2, 3, 4};
    a0 = d0;
    EXPECT_EQ(a0.has_value(), true);
    if constexpr (requires { a0.properties(); })
    {
        std::cout << "Properties:\n" << *a0.properties() << '\n';
    }
    EXPECT_EQ(std::type_index(typeid(Data)), std::type_index(a0.type()));
    EXPECT_NE(std::type_index(typeid(int)), std::type_index(a0.type()));
    const auto& a2{any_cast<Data>(a0)};
    EXPECT_EQ(a2.ldata[0], 1);
    EXPECT_EQ(a2.ldata[3], 4);
    {
        try
        {
            auto z0 = std::any_cast<int>(a0);
        }
        catch (std::exception& e)
        {
            std::cout << "Got excepted exception3: " << e.what() << '\n';
        }
        EXPECT_EQ(std::type_index(typeid(Data)), std::type_index(a0.type()));
    }

    A a4{a0};            // copy constructor
    A a5{std::move(a0)}; // move constructor
    a4 = a5;             // clone_assignment
    a4 = std::move(a5);  // move_assignment
}

TEST(TestStdAny, StandardFeatures0)
{
test_standard_any_features<std::any>();
}

TEST(TestXtndAny, StandardFeatures0)
{
test_standard_any_features<ext::any<16>>();
test_standard_any_features<ext::any<32>>();
test_standard_any_features<ext::any<48>>();
test_standard_any_features<ext::any<64>>();
}
TEST(TestXtndStreamableAny, StandardFeatures0)
{
test_standard_any_features<ext::any<16, ext::af_streamed>>();
test_standard_any_features<ext::any<24, ext::af_streamed>>();
test_standard_any_features<ext::any<32, ext::af_streamed>>();
test_standard_any_features<ext::any<48, ext::af_streamed>>();
test_standard_any_features<ext::any<56, ext::af_streamed>>();
}

template<typename A> void test_any_less()
{
    A a{3};
    EXPECT_EQ(a.inplace(), true);
    A b{10};
    bool r{a < b};

    EXPECT_EQ(r, true);

    A c{"test"};
    EXPECT_EQ(c.inplace(), false);
    try
    {
        bool r0{a < c};
    }
    catch (std::exception& e)
    {
        std::cout << "Got expected exception: " << e.what() << '\n';
    }
}

TEST(TestXtndStreamableAny, AnyLess)
{
test_any_less<ext::any<16, ext::af_strict_less, ext::af_strict_streamed>>();
}

template<typename A> void test_any_eq()
{
    A a{3};
    EXPECT_EQ(a.inplace(), true);
    A b{10};
    bool r{a == b};

    EXPECT_EQ(r, false);

    b = 3;
    bool r1{a == b};
    EXPECT_EQ(r1, true);

    A c{"test"};
    try
    {
        bool r0{a == c};
    }
    catch (std::exception& e)
    {
        std::cout << "Got expected exception: " << e.what() << '\n';
    }
}

TEST(TestXtndStreamableAny, AnyEQ)
{
test_any_eq<ext::any<16, ext::af_strict_eq, ext::af_strict_less, ext::af_strict_streamed>>();
}

template<typename A> void test_any_hash()
{
    A a{3};
    EXPECT_EQ(a.inplace(), true);
    EXPECT_EQ(a.has_value(), true);
    std::unordered_map<A, std::string> um{};

    um.insert(std::pair{a, std::string{"yes this is ok"}});
    auto iter = um.find(a);
    if (iter != um.end())
    {
        std::cout << iter->second << '\n';
        EXPECT_EQ(true, true);
    }
    else
        EXPECT_EQ(true, false);
    A c{};
    try
    {
        um.insert(std::pair{c, std::string{"can't work with empty ext::any<>"}});
        iter = um.find(c);
        if (iter != um.end())
            std::cout << iter->second << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << "Got expected unordered map exception: " << e.what() << '\n';
    }
}

TEST(TestXtndStreamableAny, AnyHash)
{
test_any_hash<ext::any<16, ext::af_strict_hash, ext::af_strict_eq, ext::af_strict_streamed>>();
}

struct NonCopyable
{
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&) = default;
    ~NonCopyable() = default;
};
struct NonMovable
{
    NonMovable() = default;
    NonMovable(const NonMovable&) = delete;
    NonMovable(NonMovable&&) = default;
    NonMovable& operator=(const NonMovable&) = delete;
    NonMovable& operator=(NonMovable&) = default;
    ~NonMovable() = default;
};

TEST(TestAny, NONCOPYABLE)
{
ext::any<8, ext::af_strict_hash, ext::af_strict_eq, ext::af_strict_streamed> a0{};
// a0 = NonCopyable{};
// a0 = NonMovable{};
}

TEST(TestAny, Variant)
{
ext::any<0, ext::af_variant<int, long, std::string>::template types> a0{};
std::cout << "inplace capacity: " << a0.in_place_capacity() << '\n';
a0 = 456;
a0 = std::string{"this is a string"};
// a0 = 5.6; // - fails to compile.
}
TEST(TestAny, NoHeap)
{
struct Data2 { uint16_t data; };
struct Data32 { uint64_t data[4]; };

ext::any<8, ext::af_strict_inplace> a0{};
std::cout << "inplace capacity: " << a0.in_place_capacity() << '\n';
a0 = 456;
a0 = 5.6; // - fails to compile.
// a0 = std::string{"this is a string"};  // -- fails to compile as expected.
a0 = Data2{};
// a0 = Data32{};  // -- fails to compile as expected.
}

TEST(TestAny, func)
{
ext::any<16> a0{ +[]() { std::cout << "hello from within any"; }};

auto fp = any_cast<void (*)()>(&a0);
if (fp)
(*fp)();
else
std::cout << "null func pointer\n";

//    a0. operator() ();
}
// template<typename F, typename R, typename ... Args>
// struct XYZ {
//     R operator()(Args&& ... args) {
//         if (has_value() && )
//     }
// };
