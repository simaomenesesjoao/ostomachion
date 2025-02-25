#include "customFloat.cpp"
#include "numbers.cpp"
#include "state.cpp"

template <typename T>
Polygon<T> put_poly_in_place(State<T> const& state, unsigned i, unsigned jj){
    
    unsigned obtusest_node_index = state.current_polygon.get_obtusest_index();
    Node<T>& obtusest_node = state.current_polygon.ll_node_from_index(obtusest_node_index)->data;
    // std::cout << "Current poly:\n";
    // current_polygon.print();
    // std::cout << "\n";


    Polygon<T> poly = Polygon<T>(polygons<T>::polyset.at(i));
    LL_Node<Node<T>> *current = poly.head;
    
    for(unsigned j = 0; j < jj; j++){
        current = current->next;
    }

    Node<T>& proposed_node = current->data;
    poly.translate(obtusest_node.position - proposed_node.position);
    poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, obtusest_node.position);

    return poly;
}


int main(){

    using T = Number<mpz_class, 2, 5, 13, 17>;
    using U = Float<double>;
    
    State<T> stateT;
    State<U> stateU;

    auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0, 0, 0};
    // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 2};
    // [10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0]

    for(unsigned index: indices){
        // std::cout << "------------------------ MPZ ------------------------\n";
        auto next_statesT = stateT.find_next_states();
        unsigned sizeT = next_statesT.size();
        // std::cout << "------------------------ double ------------------------\n";
        auto next_statesU = stateU.find_next_states();
        unsigned sizeU = next_statesU.size();
        std::cout << "index:" << index << "sizes: " << sizeT << " " << sizeU;
        std::cout << "size_ll: " << stateT.current_polygon.size_ll << "\n";
        if(sizeT != sizeU){
            std::cout << "different\n";
            
            break;
        }

        stateT = next_statesT.at(index);
        stateU = next_statesU.at(index);
    }

    // unsigned i = 10;
    // unsigned j = 1;
    // Polygon<T> polyT = put_poly_in_place(stateT, i, j);
    // Polygon<U> polyU = put_poly_in_place(stateU, i, j);

    // std::cout << "------- current polygon double ----------\n";
    // stateT.current_polygon.print();

    // std::cout << "------------------------ MPZ ------------------------\n";
    // polyT.print();
    // std::cout << "------------------------ double ------------------------\n";
    // polyU.print();
    // bool overT, overU;
    // std::cout << "Overlap:\n";
    // overT = stateT.current_polygon.overlaps(polyT);
    // overU = stateU.current_polygon.overlaps(polyU);
    // std::cout << overT << " " << overU << "\n";
    
    

    // overT = stateT.current_polygon.edge_node_intersection(polyT);
    // overU = stateU.current_polygon.edge_node_intersection(polyU);
    // std::cout << overT << " " << overU << "\n";

    // overT = polyT.edge_node_intersection(stateT.current_polygon);
    // overU = polyU.edge_node_intersection(stateU.current_polygon);
    // std::cout << overT << " " << overU << "\n";


    // Select nodes for is_inner_vertex and point_on_edge
    // i = 2;
    // LL_Node<Node<T>> *currentT{polyT.head};
    // LL_Node<Node<U>> *currentU{polyU.head};
    // for(unsigned ii=0; ii<i; ii++){
    //     currentT = currentT->next;
    //     currentU = currentU->next;
    // }

    // Point<T> const& PT = currentT->data.position;
    // Point<T> const& QT = currentT->prev->data.position;
    // Point<U> const& PU = currentU->data.position;
    // Point<U> const& QU = currentU->prev->data.position;
    
    // std::cout << "PT: " << PT.get_x().number_double << " " << PT.get_y().number_double << "\n";
    // std::cout << "QT: " << QT.get_x().number_double << " " << QT.get_y().number_double << "\n";
    // std::cout << "PU: " << PU.get_x() << " " << PU.get_y() << "\n";
    // std::cout << "QU: " << QU.get_x() << " " << QU.get_y() << "\n";
    
    // j = 3;
    // LL_Node<Node<T>> *other_currentT{stateT.current_polygon.head};
    // LL_Node<Node<U>> *other_currentU{stateU.current_polygon.head};
    // for(unsigned jj=0; jj<j; jj++){
    //     other_currentT = other_currentT->next;
    //     other_currentU = other_currentU->next;
    // }

    // std::cout << "otherT: " << other_currentT->data.position.get_x() << " " << other_currentT->data.position.get_y() << "\n";
    // std::cout << "otherU: " << other_currentU->data.position.get_x() << " " << other_currentU->data.position.get_y() << "\n";

    // bool boolT = point_on_edge(PT, QT, other_currentT->data.position);
    // bool boolU = point_on_edge(PU, QU, other_currentU->data.position);
    // std::cout << "point on edge?:" << boolT << " " << boolU << "\n";

    // boolT = is_inner_vertex(PT, QT, other_currentT);
    // boolU = is_inner_vertex(PU, QU, other_currentU);
    // std::cout << "is_inner_vertex?:" << boolT << " " << boolU << "\n";

    // Point<T> qT{QT - PT};
    // Point<U> qU{QU - PU};

    // auto NT = qT.cross(other_currentT->next->data.position - other_currentT->data.position);
    // auto NU = qU.cross(other_currentU->next->data.position - other_currentU->data.position);
    // std::cout << NT << " " << NU << "\n";
    // std::cout << "NT<=0? " << (NT <= 0) << "\n";
    // std::cout << "NU<=0? " << (NU <= 0) << "\n";

    // NT = qT.cross(other_currentT->prev->data.position - other_currentT->data.position);
    // NU = qU.cross(other_currentU->prev->data.position - other_currentU->data.position);
    // std::cout << NT << " " << NU << "\n";

}