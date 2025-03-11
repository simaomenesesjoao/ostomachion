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
    int count;
public:
    Getter(int start):count{start}{
        
        // std::random_device rd;
        // std::mt19937 gen(rd());  // Initialize the generator with a random seed
        // std::uniform_int_distribution<> dis(0, 100);  // Distribute values between 0 and vector size - 1
    }

    int operator()(){
        count++;
        count = count * 2;
        count+= 10;
        count = count%100;
        return count;
    }
};

template <typename Num, bool hash, bool comp>
Archive<State<Num, InnerState<Num, hash, comp>>> get_(std::vector<unsigned int> indices, std::ostream& stream, Getter& getter){
    using Inner = InnerState<Num, hash, comp>;
    using Stat = State<Num, Inner>;
    bool allow_reflection = true;

    State<Num, Inner> first_state;
    first_state = first_state.iterate(indices, allow_reflection);
    std::vector<Stat> next_states{first_state};

    Archive<Stat> archive;
    Queue<Stat> queue;

    std::vector<bool> mask = archive.insert(next_states);
    queue.insert(next_states, mask);

    while(true){

        auto maybe_state = queue.get_next_pointer(0);
        if(!maybe_state)
            break;
        auto next_states = maybe_state->find_next_states(allow_reflection, getter, stream);
        std::vector<bool> mask = archive.insert(next_states);
        queue.insert(next_states, mask);
    }

    return archive;
}

int main(){
    Getter getter1{0};
    Getter getter2{1};
    Getter getter3{0};

    std::vector<unsigned int> indices{68, 28, 49, 1, 0, 37, 11};
    using Num1 = Float<double>;
    // using Num1 = Number<mpz_class, 2, 5, 13, 17>;
    std::ofstream file1, file2, file3, file4;
    file1.open("out1");
    file2.open("out2");
    file3.open("out3");
    file4.open("out4");
    
    auto archive1 = get_<Num1, false, true>(indices, file1, getter1);
    auto archive2 = get_<Num1, false, true>(indices, file2, getter1);
    auto archive3 = get_<Num1, false, true>(indices, file3, getter2);
    auto archive4 = get_<Num1, false, true>(indices, file4, getter3);

    for(unsigned int i = 0; i < polygons<Float<double>>::num_polygons; i++){
        auto s1 = archive1.find_states_with_size(i);
        auto s2 = archive2.find_states_with_size(i);
        auto s3 = archive3.find_states_with_size(i);
        auto s4 = archive4.find_states_with_size(i);
        std::cout << i << " " << s1.size() << " " << s2.size() << " " << s3.size() << " " << s4.size() << "\n";
    }

    file1.close();
    file2.close();
    file3.close();
    file4.close();


}