#include <mutex>
#include <iostream>
#include <chrono>
#include <format>

struct Analytics {
    std::chrono::duration<double> lock_insert, duration_insert, lock_next, duration_next, duration;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> start;
    unsigned long max_items, total_items, total_final, num_misses;
    std::string info;
    
    Analytics():
        lock_insert{0}, duration_insert{0}, lock_next{0}, duration_next{0}, duration{0}, start{},
        max_items{0}, total_items{0}, total_final{0}, num_misses{0}, info{""}{}


    void clock_start(){
        start = std::chrono::high_resolution_clock::now();
    }

    void clock_end(){
        duration = std::chrono::high_resolution_clock::now() - start;
    }

    void print() const {
        std::cout << "Analytics " << info << ":\n";
        std::cout << "   lock next: " << lock_next << "\n";
        std::cout << "   duration next: " << duration_next << "\n";
        std::cout << "   lock insert: " << lock_insert << "\n";
        std::cout << "   duration insert: " << duration_insert << "\n";
        std::cout << "   max_items: " << max_items << "\n";
        std::cout << "   total_items: " << total_items << "\n";
        std::cout << "   num_misses: " << num_misses << "\n";
        std::cout << "\n";
    }

    void print_single_line() const {
        std::cout << std::format("Analytics {} - max: {:<5} total: {:<6} final: {:<6} duration: {:.2f}\n", 
                info, max_items, total_items, total_final, duration.count());
    }
};

template <typename T>
class Container {

protected:   
    std::mutex mtx;

public:
    
    Analytics analytics;
    virtual int insert(const std::vector<T>& states) = 0;
    virtual std::optional<T> get_next() = 0;
    virtual void clear() = 0;
    virtual std::vector<T> get_solutions() = 0;

    Container(): analytics{}{}

    Container(Container&& other){}

    void print_analytics() const {
        analytics.print();
    }
};


template <typename T>
class Stack : public Container<T> {

public:
    static std::string name;
    std::vector<T> container;
    std::vector<T> final_container;
    
    Stack(){}
    Stack(Stack&& other):
        container{std::move(other.container)},
        final_container{std::move(other.final_container)}{}

    int insert(const std::vector<T>& states) override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();

        for(auto& state: states){
            container.push_back(state);
        }

        this->analytics.max_items = std::max(this->analytics.max_items, container.size());
        this->analytics.total_items += states.size();

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;
        this->analytics.duration_insert += s3 - s2;

        return states.size();
    }

    unsigned long size() const {
        return container.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        
        if(container.size() == 0)
            return std::nullopt;

        auto state = container.back();
        if(state.size() == T::In::get_max_size())
            final_container.push_back(state);

        container.pop_back();
        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;
        this->analytics.duration_next += s3 - s2;

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
std::string Stack<T>::name = "Stack";


template <typename T>
class Hash : public Container<T> {
private:
    struct HashStruct {
        size_t operator()(const T& x) const {
            return x.get_hash();
        }
    };
    

public:
    static std::string name;
    std::unordered_set<T, HashStruct> container;
    std::vector<T> final_container;
    
    Hash(){};
    Hash(Hash&& other):
        container{std::move(other.container)},
        final_container{std::move(other.final_container)}{}

    int insert(const std::vector<T>& states) override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();

        int count = 0;
        for(auto& state: states){
            auto [it, success] = container.insert(state);
            if(success)
                count++;
        }
        this->analytics.total_items += count;
        this->analytics.max_items = std::max(this->analytics.max_items, container.size());

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;
        this->analytics.duration_insert += s3 - s2;
        
        return count;
    }

    unsigned long size() const {
        return container.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        
        if(container.size() == 0)
            return std::nullopt;

        auto it = container.begin();
        T state = *it;
        
        if(state.size() == T::In::get_max_size())
            final_container.push_back(state);

        
        container.erase(it); 

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;
        this->analytics.duration_next += s3 - s2;

        return state;
    }

    std::vector<T> get_solutions() override{
        return final_container;
    }

    void clear() override {
        container.clear();
    }


};

template <typename T>
std::string Hash<T>::name = "Hash";




template <typename T>
class HashLevel : public Container<T> {
private:
    struct HashStruct {
        size_t operator()(const T& x) const {
            return x.get_hash();
        }
    };
    
    std::unordered_set<T, HashStruct> container_in, container_out;

public:
    static std::string name;
    std::vector<T> final_container;
    
    HashLevel(){};
    HashLevel(HashLevel&& other):
        container_in{std::move(other.container_in)},
        container_out{std::move(other.container_out)},
        final_container{std::move(other.final_container)}{}

    int insert(const std::vector<T>& states) override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();

        int count = 0;
        for(auto& state: states){
            auto [it, success] = container_out.insert(state);
            if(success)
                count++;
        }
        this->analytics.total_items += count;
        this->analytics.max_items = std::max(this->analytics.max_items, container_in.size() + container_out.size());

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;
        this->analytics.duration_insert += s3 - s2;
        
        return count;
    }

    unsigned long size() const {
        return container_in.size() + container_out.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        
        if(container_in.size() == 0 and container_out.size() == 0)
            return std::nullopt;

        if(container_in.size() == 0){
            container_in = container_out;
            container_out.clear();
        }

        auto it = container_in.begin();
        T state = *it;
        container_in.erase(it); // isto devia estar aqui?
        
        if(state.size() == T::In::get_max_size())
            final_container.push_back(state);

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;
        this->analytics.duration_next += s3 - s2;

        return state;
    }

    std::vector<T> get_solutions() override{
        return final_container;
    }

    void clear() override {
        container_in.clear();
        container_out.clear();
    }


};

template <typename T>
std::string HashLevel<T>::name = "HashL";