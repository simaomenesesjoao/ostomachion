#include <mutex>
// #include "state.cpp"
#include <chrono>


template <typename Inner>
class Archive{
private:
    struct HashStruct {
        size_t operator()(const Inner& inner_state) const {
            return inner_state.get_hash();
        }
    };
    std::unordered_set<Inner, HashStruct> archive;
    std::mutex mtx;
    // double counter;

public:
    Archive(){};
    Archive(Archive&& other):
        archive{std::move(other.archive)},
        mtx(std::mutex( )){}

    template <typename State>
    std::vector<bool> insert(const std::vector<State>& states){
        std::lock_guard<std::mutex> lock(mtx);

        std::vector<bool> mask;

        for(auto& state: states){
            auto [it, success] = archive.insert(*state.used_polys);
            mask.push_back(success);
        }

        return mask;
    }

    unsigned size(){
        return archive.size();
    }

    std::vector<Inner> find_states_with_size(unsigned i){

        std::vector<Inner> completes;
        for(auto& state: archive){

            if(state.size() == i){
                completes.push_back(state);            
            }
        }
        return completes;
    }

    std::vector<Inner> find_completes(){

        std::vector<Inner> completes;
        for(auto& state: archive){

            if(state.size() == polygons<Float<double>>::num_polygons){
                completes.push_back(state);            
            }
        }
        return completes;
    }

    std::vector<Inner> find_uniques(const std::vector<Inner>& completes){

        std::vector<Inner> uniques;
        for(unsigned i=0; i<completes.size(); i++){
            auto& state1 = completes.at(i);
            bool unique = true;

            for(unsigned j=i+1; j<completes.size(); j++){
                auto& state2 = completes.at(j);

                if(state2.are_polys_same(state1)){
                    unique = false;
                    break;
                }
            }

            if(unique){
                uniques.push_back(state1);
            }
        }
        return uniques;

    }

};

template <typename State>
class Queue{
private:
    std::vector<State> queue;
    std::mutex mtx;

public:
    void insert(std::vector<State>& states, const std::vector<bool>& mask){
        std::lock_guard<std::mutex> lock(mtx);
        assert(states.size() == mask.size());

        for(unsigned i=0; i<states.size(); i++){
            if(mask.at(i))
                queue.push_back(std::move(states.at(i)));
        }

    }

    std::optional<State> get_next_pointer(int i){

        std::lock_guard<std::mutex> lock(mtx);
        

        int N = queue.size();
        if(N == 0)
            return std::nullopt;

        // auto it = queue.rbegin() + std::min(N-1, i);
        int n = N-1-i;

        auto it = queue.begin() + std::max(n, 0);
        // std::cout << n << " ";
        
        State last_state = *it;
        //queue.pop_back();
        queue.erase(it);

        return last_state;
    }
    
};


class ThreadPool{};