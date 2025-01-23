#ifndef STATE_C
#define STATE_C

#include "polygon.cpp"
#include <set>

template <int... Ints>
class State{
    using Poly = Polygon<Ints...>;
    using Nod = Node<Ints...>;
    using Num = Number<Ints...>;
    // Characterizes an ostomachion st  ate by the current polygon 
    // (through the positions of the Nodes) and unused polygons
    std::set<Poly*> unused_polys;
    Poly current_polygon;



    long get_hash(){
        // Hashes the state. This cannot depend on the order of the 
        // nodes in the polygon


        long hash{};

        LL_Node<Nod> *current{current_polygon.head};
        for(int i=0; i < current_polygon.size_ll; i++){
            auto& [x, y] current->data.position;
            current = current->next;

            // Unfold x, y into their components and make hash from it

            hash += x*256 + y;
        }

        return hash;

    }


    static bool equals(State const& state1, State const& state2){


    }

    

};


#endif // STATE_C