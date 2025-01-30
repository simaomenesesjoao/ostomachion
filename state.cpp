#ifndef STATE_C
#define STATE_C

#include "polygon.cpp"
#include <set>
#include "ostomachion.cpp"

template <int... Ints>
class Tracker{

                        // auto hash = next_state.get_hash();
};

template <int... Ints>
class State{
    using Poly = Polygon<Ints...>;
    using Nod = Node<Ints...>;
    using Num = Number<Ints...>;
    using Ang = Angle<Ints...>;
    using Poi = Point<Ints...>;
    // Characterizes an ostomachion st  ate by the current polygon 
    // (through the positions of the Nodes) and unused polygons
    std::vector<std::vector<Point>> used_polys;
    Poly current_polygon;
    Tracker& tracker;

    State(Tracker& tracker): current_polygon{polygons::frame}, tracker{tracker}{
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
        // Find the node with smallest internal angle
        Nod& acutest_node = current_polygon.get_acutest_node();

        // Find which polygons haven't been used yet
        for(int i = 0; i < polygons::num_polygons; i++){
            auto& point_set = used_polys.at(i);
            if(point_set.size() != 0) continue;

            auto& poly = polygons::polyset.at(i);
            LL_Node<*Nod> current = poly.head;
            for(int j = 0; j < poly.size_ll; j++){
                Nod& proposed_node = current->data.angle_opening;

                if(angles_compatible(proposed_node.angle_opening, acutest_node.angle_opening)){
                    poly.translate(acutest_node.position - proposed_node.position);
                    poly.rotate(acutest_node.angle_start - proposed_node.angle_end, acutest_node.position)
                    if(not current_polygon.overlaps(poly)){

                        // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                        State next_state(current_polygon, poly, i);
                        if tracker.add(next_state)
                            next_state.find_next_state();
                    }
                }
                
                current = current->next;
            }

            

        }
    }


};


#endif // STATE_C