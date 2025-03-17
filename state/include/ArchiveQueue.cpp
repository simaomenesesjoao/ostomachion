#include <mutex>
#include <chrono>

template <typename T>
class Container {
public:
    using DataType = T;
    virtual int insert(const std::vector<T>& states) = 0;
    virtual std::optional<T> get_next() = 0;
    // virtual void clear() = 0;
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

    // std::vector<Inner> find_uniques(const std::vector<Inner>& completes){

    //     std::vector<Inner> uniques;
    //     for(unsigned i=0; i<completes.size(); i++){
    //         auto& state1 = completes.at(i);
    //         bool unique = true;

    //         for(unsigned j=i+1; j<completes.size(); j++){
    //             auto& state2 = completes.at(j);

    //             if(state2.are_polys_same(state1)){
    //                 unique = false;
    //                 break;
    //             }
    //         }

    //         if(unique){
    //             uniques.push_back(state1);
    //         }
    //     }
    //     return uniques;

    // }
    // void reset() {
    //     archive.clear();
    // }

};


template <typename Inner>
class Hash : public Container<Inner> {
private:
    struct HashStruct {
        size_t operator()(const Inner& inner_state) const {
            return inner_state.get_hash();
        }
    };
    std::mutex mtx;
    // double counter;

public:
    std::unordered_set<Inner, HashStruct> archive;
    Hash(){};
    Hash(Hash&& other):
        archive{std::move(other.archive)},
        mtx(std::mutex( )){}

    template <typename State>
    std::vector<bool> insert(const std::vector<State>& states){
        std::lock_guard<std::mutex> lock(mtx);

        std::vector<bool> mask;

        for(auto& state: states){
            auto [it, success] = archive.insert(state);
            mask.push_back(success);
        }

        return mask;
    }

    unsigned size(){
        return archive.size();
    }


    std::optional<Inner> get_next_pointer(){
        std::lock_guard<std::mutex> lock(mtx);
        

        int N = archive.size();
        if(N == 0)
            return std::nullopt;

        // auto it = queue.rbegin() + std::min(N-1, i);
        //int n = N-1-i;

        auto it = archive.begin();// + std::max(n, 0);
        // std::cout << n << " ";
        
        Inner last_state = *it;
        //queue.pop_back();
        archive.erase(it);

        return last_state;
    }

    // std::vector<Inner> find_uniques(const std::vector<Inner>& completes){

    //     std::vector<Inner> uniques;
    //     for(unsigned i=0; i<completes.size(); i++){
    //         auto& state1 = completes.at(i);
    //         bool unique = true;

    //         for(unsigned j=i+1; j<completes.size(); j++){
    //             auto& state2 = completes.at(j);

    //             if(state2.are_polys_same(state1)){
    //                 unique = false;
    //                 break;
    //             }
    //         }

    //         if(unique){
    //             uniques.push_back(state1);
    //         }
    //     }
    //     return uniques;

    // }
    void reset() override {
        archive.clear();
    }

};

// template <typename State>
// class Queue{
// private:
//     std::mutex mtx;

// public:
//     std::vector<State> queue;
//     void insert(std::vector<State>& states, const std::vector<bool>& mask){
//         std::lock_guard<std::mutex> lock(mtx);
//         assert(states.size() == mask.size());

//         for(unsigned i=0; i<states.size(); i++){
//             if(mask.at(i))
//                 queue.push_back(std::move(states.at(i)));
//         }

//     }

//     std::optional<State> get_next_pointer(int i){

//         std::lock_guard<std::mutex> lock(mtx);
        

//         int N = queue.size();
//         if(N == 0)
//             return std::nullopt;

//         // auto it = queue.rbegin() + std::min(N-1, i);
//         int n = N-1-i;

//         auto it = queue.begin() + std::max(n, 0);
//         // std::cout << n << " ";
        
//         State last_state = *it;
//         //queue.pop_back();
//         queue.erase(it);

//         return last_state;
//     }

//     unsigned size() const {
//         return queue.size();
//     }

//     void reset() {
//         queue.clear();
//     }
    
//     // Queue& operator=(const Queue& other){

//     // }
// };


// class ThreadPool{};