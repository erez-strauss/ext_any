// This example code is a simplified baseic draft implementation of std::any
//
#include <iostream>
#include <any> // for bad_type_cast exception.
// Simple example any step 1

class any1; // forward declaration

class any_properties_base {
public:
    virtual const std::type_info& type_ () const = 0;
    virtual void clone_(any1&, void* otherData) const = 0;
    virtual void destroy_(any1&) const = 0;
};

// forward declaration
template<typename T>
constexpr const any_properties_base& get_any_type_properies() noexcept;

class any1
{
    void* data_ptr_{nullptr};
    const any_properties_base* properties_ {nullptr};
public:
    constexpr bool has_value() const noexcept { return properties_ != nullptr && data_ptr_ != nullptr; }
    constexpr any1() {}

    void reset() noexcept {
        if (properties_)
            properties_->destroy_(*this);
        data_ptr_ = nullptr;
        properties_ = nullptr;
    }
    ~any1() {
        reset();
    }
    void swap(any1& other) noexcept {
        std::swap(properties_, other.properties_);
        std::swap(data_ptr_, other.data_ptr_);
    }
    template<typename T>
    explicit any1(T&& value);

    const std::type_info& type() const {
        if (properties_)
            return properties_->type_();
        return typeid(void);
    }
    // any1& operator=(T&&) -- two cases, with any1 and non-any1.

    template<typename T>
    friend class any_properties;

    template<typename T>
    T& any_cast(any1& a);
    template<typename T>
    const T& any_cast(const any1& a);

    template<typename T>
    T& any_cast(any1* a);
    template<typename T>
    const T& any_cast(const any1* a);
};

template<typename T>
class any_properties : public any_properties_base
{
public:
    const std::type_info& type_ () const override {
        return typeid(T);
    }
    void clone_(any1&, void* ) const override {

    }
    void destroy_(any1& a) const override {
        auto p = reinterpret_cast<T*>(a.data_ptr_);
        delete p;
        a.data_ptr_ = nullptr;
    }
};

template<typename T>
constexpr const any_properties_base& get_any_type_properies() noexcept
{
    static const any_properties<T> type_properties{};
    return type_properties;
}

// need to check that value is not from type any1.
template<typename T>
any1::any1(T&& value) : data_ptr_(new T (std::move(value))),
            properties_(&get_any_type_properies<T>()) {
}


template<typename T>
T& any_cast(any1& a)
{
    if (typeid(T) == a.type())
    {
        return *static_cast<T*>(a.data_ptr_);
    }
    throw std::bad_any_cast{};
}


template<typename T>
T* any_cast(any1* a)
{
    if (typeid(T) == a->type())
    {
        return *static_cast<T*>(a->data_ptr_);
    }
    return nullptr;
}



any1 a0;
any1 a1{10};
any1 s0 {std::string{"hello world"}};

int main()
{
    std::cout << "empty: " << a0.type().name() << '\n';
    std::cout << "int: " << a1.type().name() << '\n';
    std::cout << "string: " << s0.type().name() << '\n';
    return 0;
}