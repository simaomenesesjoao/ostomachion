#include <thread>
#include <gmpxx.h>
#include <random>
#include "state.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "ArchiveQueue.cpp"
#include <mutex>
#include "../../solver/src/graphics.cpp"

struct Getter{
private:
    bool first;
public:
    Getter(bool ff):first{ff}{}

    int operator()(int last){
        if(first)
            return 0;
        else
            return last-1;
    }
};

std::mutex mtx2;

template <typename Archive, typename Queue>
void process(Archive& archive, 
             Queue& queue, 
             std::ostream& stream, 
             Getter& getter,
             std::vector<bool>& threads_status, 
             int thread_index){

    bool allow_reflection = true;
    while(true){

        auto maybe_state = queue.get_next_pointer(0);
        if(not maybe_state)
            break;
        
        auto next_states = maybe_state->find_next_states(allow_reflection, getter, stream);
        std::vector<bool> mask = archive.insert(next_states);
        queue.insert(next_states, mask);
    }
}

template <typename Num, bool hash, bool comp>
Archive<State<Num, InnerState<Num, hash, comp>>> get_(std::vector<unsigned int> indices, Getter& getter, unsigned int num_threads, std::ostream& stream){
    using Inner = InnerState<Num, hash, comp>;
    using Stat = State<Num, Inner>;
    bool allow_reflection = true;

    State<Num, Inner> first_state;
    first_state = first_state.iterate(indices, allow_reflection, getter, stream);
    std::vector<Stat> next_states{first_state};

    Archive<Stat> archive;
    Queue<Stat> queue;
    std::vector<bool> mask = archive.insert(next_states);
    queue.insert(next_states, mask);
    std::vector<bool> status;

    process<Archive<Stat>, Queue<Stat>>(archive, queue, stream, getter, status, 0);

    return archive;
}

int main(int argc, char** argv){
    
    int index = 0;
    if(argc>0)
        index = atoi(argv[1]);
    std::cout << "argc: " << argc << " index: " << index << "\n";

    std::vector<unsigned int> indices{68, 28};
    // 68, 28, 49,  1,  0, 37, 11,  1, 14,  2,  1,  0,  0,  0},
    using Num1 = Float<double>;
    
    std::ofstream file1, file2;
    file1.open("out1");
    file2.open("out2");
    Getter getter1{true};
    Getter getter2{false};
    
    auto archive1 = get_<Num1, true, true>(indices, getter1, 1, file1); // first
    auto archive2 = get_<Num1, true, true>(indices, getter2, 1, file2); // last

    for(unsigned int i = 0; i < polygons<Float<double>>::num_polygons+2; i++){
        auto s1 = archive1.find_states_with_size(i);
        auto s2 = archive2.find_states_with_size(i);
        std::cout << i << " " << s1.size() << " " << s2.size() << "\n";
    }

    file1.close();
    file2.close();

    auto s1 = archive1.find_states_with_size(polygons<Float<double>>::num_polygons);
    auto s2 = archive2.find_states_with_size(polygons<Float<double>>::num_polygons);

    // unsigned int ii = 8;
    // auto s1 = archive1.find_states_with_size(ii);
    // auto s2 = archive2.find_states_with_size(ii);

    
    // std::cout << s1.size() << " " << s2.size() << "\n";
    
    // Graphics graphics;
    // auto used_polys1 = s1.at(0).used_polys_as_vector();
    // graphics.draw_state(used_polys, first_state.current_polygon->as_vector());
    // auto used_polys2 = s.used_polys_as_vector();
    // graphics.draw_state(used_polys, first_state.current_polygon->as_vector());

    // // Find states in 1 that are not in 2
    // std::vector<unsigned int> original1, original2;
    // for(unsigned int i = 0; i < s1.size(); i++){
    //     auto v1 = s1.at(i).used_polys->used_polys_as_vector();
    //     bool found = false;
    //     unsigned int found_index = 99999;
    //     for(unsigned int j = 0; j < s2.size(); j++){
    //         auto v2 = s2.at(j).used_polys->used_polys_as_vector();
    //         bool b = (*s1.at(i).used_polys == *s2.at(j).used_polys);
    //         if(b){
    //             found = true;
                
    //             // break;
    //         }
    //         std::cout << b << "" ;
    //     }
    //     if(!found){
    //         original1.push_back(i);
    //     }
    //     std::cout << "\n";
    // }



    // Graphics graphics;
    // std::cout << "s1\n";
    // for(auto& s: s1){
    //     std::cout << s << "\n";
    //     graphics.draw_state(s.used_polys->used_polys_as_vector(), s.current_polygon->as_vector());
    // }

    // std::cout << "s2\n";
    // for(auto& s: s2){
    //     std::cout << s << "\n";
    //     // graphics.draw_state(s.used_polys->used_polys_as_vector(), s.current_polygon->as_vector());
    // }


    // std::cout << "------------------\n";


    // std::ofstream file3;
    // file3.open("out3");
    // std::vector<unsigned> indices2{68, 28, 49, 1, 0, 37, 1, 1, 12, 1, 2, 0, 0, 0};
    // State<Num1, InnerState<Num1, false, true>> first_state;
    // first_state = first_state.iterate(indices2, true, getter2, file3);
    // // first_state = first_state.iterate(indices, true, getter2, file3);
    // file3.close();
    // std::cout << first_state << "\n";

    // Graphics graphics;
    // for(auto& i: original1){
    //     auto& s = s1.at(i);
    //     std::cout << "i: " << i << "\n";
    //     std::cout << s << "\n";
    //     graphics.draw_state(s.used_polys->used_polys_as_vector(), s.current_polygon->as_vector());
    // }

    // for(unsigned i=6; i < polygons<Float<double>>::num_polygons; i++){
        

    //     auto s1 = archive.find_states_with_size(i);

        // std::cout << "comparing hash\n";
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         std::cout << (s1.at(i).get_hash() == s1.at(j).get_hash()) << " ";
        //     }
        //     std::cout << "\n";
        // }

        
        // std::cout << "comparing equality\n";
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         std::cout << (s1.at(i) == s1.at(j)) << " ";
        //     }
        //     std::cout << "\n";
        // }

        // std::cout << "equal but with different hash\n";
        // int ii{-1}, jj{-1};
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     bool found = false;
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         if((s1.at(i) == s1.at(j)) and s1.at(i).get_hash() != s1.at(j).get_hash()){
        //             found = true;
        //             ii = i;
        //             jj = j;
        //             break;
        //         }
        //     }
        //     if(found)
        //         break;
        //     std::cout << "\n";
        // }

        // if(ii >= 0 and jj >= 0){
        //     std::cout << "These have different hash but are identical\n";
        //     std::cout << s1.at(ii).get_hash() << "\n" << s1.at(ii) << "\n";
        //     std::cout << s1.at(jj).get_hash() << "\n" << s1.at(jj) << "\n";
        // }
        
        // for(auto kk: std::vector<unsigned int>{ii, jj}){
        //     auto s = s1.at(kk);
        //     auto used_polys = s.used_polys_as_vector();
        //     graphics.draw_state(used_polys, first_state.current_polygon->as_vector());
        // }

    //     auto u1 = archive.find_uniques(s1);
    //     std::cout << "size " << i << ": " << s1.size() << " " << u1.size() << "\n"; // 30

    // }
    // std::cout << "\n";

    // auto s1 = archive.find_states_with_size(polygons<Float<double>>::num_polygons);
    // auto u1 = archive.find_uniques(s1);
    // std::cout << archive.size() << "\n"; // 26222
    // std::cout << s1.size() << "\n"; // 30
    


}