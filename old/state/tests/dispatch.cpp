#include <iostream>
#include <map>
#include <optional>
#include <memory>

// Goal: Choose between a large number of templated objects in run time. 
//       These objects need to be instantiated and chosen
// Approach: define a function 'dispatcher' which receives a string (type) as input
//           and returns an object with that type



class Float{
public:
    static const std::string Type;
};
const std::string Float::Type = "Float";


class Double{
public:
    static const std::string Type;
};
const std::string Double::Type = "Double";


class IPoly{
public:
    virtual void execute() const = 0;
};

template <typename T> 
class Poly: public IPoly{
public:
    void execute() const override {
        std::cout << "executing " << T::Type << "\n";
    };
};


template <typename T, typename Q> 
class Poly2: public IPoly{
public:
    void execute() const override {
        std::cout << "executing " << T::Type << " " << Q::Type << "\n";
    };
};


template <typename Tuple, std::size_t... Is>
std::map<std::string, std::unique_ptr<IPoly>> factory_factory_impl(Tuple, std::index_sequence<Is...>) {
    std::map<std::string, std::unique_ptr<IPoly>> map;

    ([&](){
        using A = std::tuple_element_t<Is, Tuple>;
        map[A::Type] = std::make_unique<Poly<A>>();
    }(), ...);
    return map;
}

template <typename Tuple, std::size_t... Is>
std::map<std::string, std::unique_ptr<IPoly>> factory_factory() {
    using IndexT = std::make_index_sequence<std::tuple_size_v<Tuple>>;
    return factory_factory_impl(Tuple{}, IndexT{});
}



template <typename T, typename Tuple, std::size_t... Is>
std::map<std::string, std::unique_ptr<IPoly>> factory_factory3(T, Tuple, std::index_sequence<Is...>){
    std::map<std::string, std::unique_ptr<IPoly>> map;
    ([&](){

        using A = std::tuple_element_t<Is, Tuple>;
        const std::string name = A::Type + "-" + T::Type; 
        map[name] = std::make_unique<Poly2<A,T>>();
    }(), ...);
    return map;
}

template <typename Tuple, std::size_t... Is, typename Tuple2>
std::map<std::string, std::unique_ptr<IPoly>> factory_factory2(
    Tuple, std::index_sequence<Is...>, Tuple2) {

    std::map<std::string, std::unique_ptr<IPoly>> map;

    ([&](){

        using A = std::tuple_element_t<Is, Tuple>;
        using IndexT = std::make_index_sequence<std::tuple_size_v<Tuple2>>;

        auto new_map = factory_factory3(A{}, Tuple2{}, IndexT{});
        for(auto& [key, val]: new_map){
            map[key] = std::move(val);
        }        
    }(), ...);

    return map;
}


template <typename Tuple, typename Tuple2>
std::map<std::string, std::unique_ptr<IPoly>> factory_factory() {
    std::map<std::string, std::unique_ptr<IPoly>> map;
    using IndexT = std::make_index_sequence<std::tuple_size_v<Tuple>>;
    return factory_factory2(Tuple{}, IndexT{}, Tuple2{});
}


using Tuple = std::tuple<Double, Float>;

std::optional<std::unique_ptr<IPoly>> factory(const std::string& type){
    auto map1 = factory_factory<Tuple>();
    auto map2 = factory_factory<Tuple, Tuple>();

    std::map<std::string, std::unique_ptr<IPoly>> map;
    for(auto& [key, val]: map1){
        map[key] = std::move(val);
    }
    
    for(auto& [key, val]: map2){
        map[key] = std::move(val);
    }

    auto it = map.find(type);
    if(it != map.end()){
        return std::move(it->second);
    } else {
        return std::nullopt;
    }
}

int main() {
    auto poly = factory("Double-Double");
    if(poly)
        (*poly)->execute();


    poly = factory("Float");
    if(poly)
        (*poly)->execute();
    

    
    
}