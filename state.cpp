#ifndef STATE_C
#define STATE_C

#include "polygon.cpp"
#include <set>
#include "ostomachion.cpp"

template <int... Ints>
class State{
    using Poly = Polygon<Ints...>;
    using Nod = Node<Ints...>;
    using Num = Number<Ints...>;
    // Characterizes an ostomachion st  ate by the current polygon 
    // (through the positions of the Nodes) and unused polygons
    std::vector<std::vector<Point>> used_polys;
    Poly current_polygon;

    State(): current_polygon{polygons::frame}{
    }

    State(State const& other){

    }

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

    
    find_next_state(){
        // Find the smallest internal angle

        // 
        for(int i = 0; i < polygons::num_polygons; i++){
            auto& poly = current_polygon.at(i);
            if(poly.size() != 0) continue;

            LL_Node<*Nod> current = polygons::polyset.at(i).head;
            for()

            

        }
    }


};


#endif // STATE_C