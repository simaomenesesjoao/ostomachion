#include <mutex>
#include <chrono>

template <typename T>
class Container {

protected:
    std::chrono::duration<double> lock_insert, duration_insert, lock_next, duration_next;
    unsigned long max_items, total_items, num_misses;     
    std::mutex mtx;

public:
    virtual int insert(const std::vector<T>& states) = 0;
    virtual std::optional<T> get_next() = 0;
    virtual void clear() = 0;
    virtual std::vector<T> get_solutions() = 0;

    Container():
        lock_insert{0}, duration_insert{0}, lock_next{0}, duration_next{0},
        max_items{0}, total_items{0}, num_misses{0}{}

    Container(Container&& other){}

    void print_analytics() const {
        std::cout << "Analytics:\n";
        std::cout << "   lock next: " << lock_next << "\n";
        std::cout << "   duration next: " << duration_next << "\n";
        std::cout << "   lock insert: " << lock_insert << "\n";
        std::cout << "   duration insert: " << duration_insert << "\n";
        std::cout << "   max_items: " << max_items << "\n";
        std::cout << "   total_items: " << total_items << "\n";
        std::cout << "   num_misses: " << num_misses << "\n";
        std::cout << "\n";
    }
};


template <typename T>
class Stack : public Container<T> {

public:
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

        this->max_items = std::max(this->max_items, container.size());
        this->total_items += states.size();

        auto s3 = std::chrono::high_resolution_clock::now();
        this->lock_insert += s2 - s1;
        this->duration_insert += s3 - s2;

        return states.size();
    }

    unsigned size(){
        return container.size();
    }

    std::optional<T> get_next() override {
        auto s1 = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lock(this->mtx);
        auto s2 = std::chrono::high_resolution_clock::now();
        
        if(container.size() == 0)
            return std::nullopt;

        auto state = container.back();
        if(state.size() == 14)
            final_container.push_back(state);

        container.pop_back();
        auto s3 = std::chrono::high_resolution_clock::now();
        this->lock_next += s2 - s1;
        this->duration_next += s3 - s2;

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
        size_t operator()(const T& x) const {
            return x.get_hash();
        }
    };
    

public:
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
        this->total_items += count;
        this->max_items = std::max(this->max_items, container.size());

        auto s3 = std::chrono::high_resolution_clock::now();
        this->lock_insert += s2 - s1;
        this->duration_insert += s3 - s2;
        
        return count;
    }

    unsigned size(){
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
        
        if(state.size() == 14)
            final_container.push_back(state);


        container.erase(it);

        auto s3 = std::chrono::high_resolution_clock::now();
        this->lock_next += s2 - s1;
        this->duration_next += s3 - s2;

        return state;
    }

    std::vector<T> get_solutions() override{
        return final_container;
    }
    //     std::vector<T> solutions;

    //     for(auto& state: last)
    //         solutions.push_back(state);

    //     return solutions;
    // }

    void clear() override {
        container.clear();
    }


};
