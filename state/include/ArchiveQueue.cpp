#include <mutex>
#include <chrono>

template <typename T>
class Container {
public:
    using DataType = T;
    virtual int insert(const std::vector<T>& states) = 0;
    virtual std::optional<T> get_next() = 0;
    virtual void clear() = 0;
    virtual std::vector<T> get_solutions() = 0;
};


template <typename T>
class Stack : public Container<T> {
private:
    std::mutex mtx;

public:
    std::vector<T> container;
    std::vector<T> final_container;
    
    Stack(){};
    Stack(Stack&& other):
        mtx(std::mutex()),
        container{std::move(other.container)}{}

    int insert(const std::vector<T>& states) override {
        std::lock_guard<std::mutex> lock(mtx);

        for(auto& state: states){
            container.push_back(state);
        }

        return states.size();
    }

    unsigned size(){
        return container.size();
    }

    std::optional<T> get_next() override {
        std::lock_guard<std::mutex> lock(mtx);
        
        if(container.size() == 0)
            return std::nullopt;

        auto state = container.back();
        if(state.size() == 14)
            final_container.push_back(state);

        container.pop_back();

        return state;
    }

    std::vector<T> get_solutions() override {
        return final_container;
    }


    void clear() override {
        container.clear();
    }

};


template <typename T>
class Hash : public Container<T> {
private:
    struct HashStruct {
        size_t operator()(const T& inner_state) const {
            return inner_state.get_hash();
        }
    };
    std::mutex mtx;

public:
    std::unordered_set<T, HashStruct> container, last;

    Hash(){};
    Hash(Hash&& other):
    mtx(std::mutex()),
    container{std::move(other.container)},
    last{std::move(other.last)}{}


    int insert(const std::vector<T>& states) override {
        std::lock_guard<std::mutex> lock(mtx);

        int count = 0;

        for(auto& state: states){
            auto [it, success] = container.insert(state);
            if(success)
                count++;
        }

        return count;
    }

    unsigned size(){
        return container.size();
    }

    std::optional<T> get_next() override {
        std::lock_guard<std::mutex> lock(mtx);
        
        if(container.size() == 0)
            return std::nullopt;

        auto it = container.begin();
        T state = *it;
        
        if(state.size() == 14)
            last.insert(state);

        container.erase(it);

        return state;
    }

    std::vector<T> get_solutions() override{
        std::vector<T> solutions;

        for(auto& state: last)
            solutions.push_back(state);

        return solutions;
    }

    void clear() override {
        container.clear();
    }

};
