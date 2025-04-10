#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <tuple>
#include <typeinfo>

// === 1. Define template templates (T types) ===

template <typename T>
class T1 {
public:
    void work() { std::cout << "T1<" << typeid(T).name() << ">\n"; }
};

template <typename T>
class T2 {
public:
    void work() { std::cout << "T2<" << typeid(T).name() << ">\n"; }
};

// === 2. Define Q types ===

struct Q1 {};
struct Q2 {};
struct Q3 {};

// === 3. MyClass and Wrapper ===

template <template<typename> class T, typename Q>
class MyClass {
public:
    void run() {
        T<Q> impl;
        impl.work();
    }
};

struct IWrapper {
    virtual ~IWrapper() = default;
    virtual void run() = 0;
};

template <template<typename> class T, typename Q>
class Wrapper : public IWrapper {
public:
    void run() override {
        MyClass<T, Q>().run();
    }
};

// === 4. Template and Type Lists ===

template <template<typename> class... Ts>
struct TemplateList {};

template <typename... Qs>
struct TypeList {};

// === 5. Registry Builder ===

template <typename TL, typename QL>
struct RegistryBuilder;

template <template<typename> class... Ts, typename... Qs>
struct RegistryBuilder<TemplateList<Ts...>, TypeList<Qs...>> {
    static std::map<std::string, std::unique_ptr<IWrapper>> build() {
        std::map<std::string, std::unique_ptr<IWrapper>> reg;

        // Fold over T types
        ([&] {
            // Fold over Q types
            ([&] {
                std::string key = std::string("T") + std::to_string(getTIndex<Ts>()) +
                                  "+Q" + std::to_string(getQIndex<Qs>());
                reg[key] = std::make_unique<Wrapper<Ts, Qs>>();
            }(), ...);
        }(), ...);

        return reg;
    }

    // Simple helpers for nicer names (T1, T2) and (Q1, Q2, ...)
    template <template<typename> class T>
    static constexpr int getTIndex() {
        return getTemplateIndex<T, Ts...>();
    }

    template <typename Q>
    static constexpr int getQIndex() {
        return getTypeIndex<Q, Qs...>();
    }

    template <template<typename> class T, template<typename> class First, template<typename> class... Rest>
    static constexpr int getTemplateIndex(int i = 1) {
        if constexpr (std::is_same<T, First>::value) return i;
        else return getTemplateIndex<T, Rest...>(i + 1);
    }

    template <typename Q, typename First, typename... Rest>
    static constexpr int getTypeIndex(int i = 1) {
        if constexpr (std::is_same<Q, First>::value) return i;
        else return getTypeIndex<Q, Rest...>(i + 1);
    }
};

// === 6. Main ===

int main() {
    using TTypes = TemplateList<T1, T2>;
    using QTypes = TypeList<Q1, Q2, Q3>;

    auto registry = RegistryBuilder<TTypes, QTypes>::build();

    for (const auto& [key, wrapper] : registry) {
        std::cout << "Running " << key << ": ";
        wrapper->run();
    }

    return 0;
}
