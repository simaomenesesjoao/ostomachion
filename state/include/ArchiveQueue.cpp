#include <mutex>
#include <iostream>
#include <chrono>
#include <condition_variable>
#include <format>
#include <atomic>
#include <list>

struct Analytics {
    std::chrono::duration<double> lock_insert, duration_insert, lock_next, duration_next, duration;
    std::chrono::duration<double> d1, d2, d3, d4;
    int count1, count2;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> start;
    unsigned long max_items, total_items, total_final, num_misses;
    std::string info;
    
    Analytics():
        lock_insert{0}, duration_insert{0}, lock_next{0}, duration_next{0}, duration{0}, 
        d1{0}, d2{0}, d3{0}, d4{0},
        count1{0}, count2{0}, start{},
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
        std::cout << "   duration: " << duration.count() << "\n";
        std::cout << "   max_items: " << max_items << "\n";
        std::cout << "   total_items: " << total_items << "\n";
        std::cout << "   total_final: " << total_final << "\n";
        std::cout << "   num_misses: " << num_misses << "\n";
        std::cout << "   d1: " << d1 << "\n";
        std::cout << "   d2: " << d2 << "\n";
        std::cout << "   count1: " << count1 << "\n";
        std::cout << "   count2: " << count2 << "\n";
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
    std::condition_variable cv;
    std::atomic<int> num_processes;

public:
    
    Analytics analytics;
    virtual void insert(std::vector<T>& states) = 0;
    virtual std::optional<T> get_next() = 0;
    // virtual std::optional<T> get_next_impl() = 0;
    virtual void clear() = 0;
    virtual std::vector<T> get_solutions() = 0;

    Container(): num_processes{1}, analytics{}{}

    Container(Container&& other){}

    void print_analytics() const {
        analytics.print();
    }
};


template <typename T>
class Stack : public Container<T> {
private:
    std::list<T> container;
    std::list<T> final_container;

public:
    static std::string name;
    
    Stack(){}
    Stack(Stack&& other):
        container{std::move(other.container)},
        final_container{std::move(other.final_container)}{}

    void insert(std::vector<T>& states) override {
        
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;

        for(auto& state: states){            

            if(state.size() == T::In::get_max_size()){

                auto p1 = std::chrono::high_resolution_clock::now();
                final_container.push_back(std::move(state));
                auto p2 = std::chrono::high_resolution_clock::now();
                this->analytics.d1 += p2-p1;
                this->analytics.count1++;
            } else {
                auto p1 = std::chrono::high_resolution_clock::now();
                container.push_back(std::move(state));
                auto p2 = std::chrono::high_resolution_clock::now();
                this->analytics.d2 += p2-p1;
                this->analytics.count2++;
            }
        }

        this->num_processes--;
        this->cv.notify_all();

        this->analytics.max_items = std::max(this->analytics.max_items, container.size());
        this->analytics.total_items += states.size();

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.duration_insert += s3 - s2;

    }

    unsigned long size() const {
        return container.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(this->mtx);
        
        while(true){
            if(container.size() == 0 and this->num_processes != 0)
                this->cv.wait(lock);
            else
                break;
        }
        
        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;

        if(container.size() == 0 and this->num_processes == 0)
            return std::nullopt;

        auto state = std::move(container.back());
        container.pop_back();

        this->num_processes++;
        lock.unlock();
        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.duration_next += s3 - s2;
        return state;
    }

    std::vector<T> get_solutions()  {
        return std::vector<T>(final_container.begin(), final_container.end());
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
    
    std::unordered_set<T, HashStruct> visited;
    std::list<T> container;
    std::list<T> final_container;

public:
    static std::string name;
    
    Hash(){};
    Hash(Hash&& other):
        container{std::move(other.container)},
        final_container{std::move(other.final_container)}{}

    void insert(std::vector<T>& states) override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;

        int count = 0;
        for(auto& state: states){
            auto [it, is_new_state] = visited.insert(state);
            
            if(is_new_state){
                count++;
                if(state.size() == T::In::get_max_size()){
                    final_container.push_back(std::move(state));
                } else {
                    container.push_back(std::move(state));
                }
            }
        }

        this->num_processes--;
        this->cv.notify_all();

        this->analytics.total_items += count;
        this->analytics.max_items = std::max(this->analytics.max_items, size());

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.duration_insert += s3 - s2;
    }

    unsigned long size() const {
        return container.size() + visited.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(this->mtx);
        
        while(true){
            if(container.size() == 0 and this->num_processes != 0)
                this->cv.wait(lock);
            else
                break;
        }
        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;
        
        if(container.size() == 0 and this->num_processes == 0)
            return std::nullopt;

        auto it = container.begin();
        auto state = std::move(*it);
        container.erase(it);

        this->num_processes++;
        lock.unlock();
        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.duration_next += s3 - s2;
        return state;
    }

    std::vector<T> get_solutions() override{
        return std::vector<T>(final_container.begin(), final_container.end());
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
    std::list<T> final_container;

public:
    static std::string name;
    
    HashLevel(){};
    
    HashLevel(HashLevel&& other):
        container_in{std::move(other.container_in)},
        container_out{std::move(other.container_out)},
        final_container{std::move(other.final_container)}{}

    void insert(std::vector<T>& states) override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_insert += s2 - s1;

        int count = 0;
        for(auto& state: states){
            auto [it, success] = container_out.insert(state);
            if(success){
                count++;

                if(state.size() == T::In::get_max_size())
                    final_container.push_back(std::move(state));
            }
        }

        this->num_processes--;
        this->cv.notify_all();

        this->analytics.total_items += count;
        this->analytics.max_items = std::max(this->analytics.max_items, container_in.size() + container_out.size());

        auto s3 = std::chrono::high_resolution_clock::now();
        this->analytics.duration_insert += s3 - s2;
    }

    unsigned long size() const {
        return container_in.size() + container_out.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(this->mtx);


        while(true){
            if(container_in.size() == 0 and container_out.size() == 0 and this->num_processes != 0)
                this->cv.wait(lock);
            else
                break;
        }

        auto s2 = std::chrono::high_resolution_clock::now();
        this->analytics.lock_next += s2 - s1;
        
        
        if(container_in.size() == 0 and container_out.size() == 0 and this->num_processes == 0)
            return std::nullopt;


        if(container_in.size() == 0){
            container_in = std::move(container_out);
            container_out.clear();
        }

        auto it = container_in.begin();
        auto state = std::move(*it);
        container_in.erase(it);
        
        auto s3 = std::chrono::high_resolution_clock::now();
        this->num_processes++;
        lock.unlock();
        this->analytics.duration_next += s3 - s2;

        return state;
    }

    std::vector<T> get_solutions() override{
        return std::vector<T>(final_container.begin(), final_container.end());
    }

    void clear() override {
        container_in.clear();
        container_out.clear();
    }


};

template <typename T>
std::string HashLevel<T>::name = "HashL";