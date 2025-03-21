#include "customFloat.cpp"
#include "numbers.cpp"
#include "state.cpp"
#include "graphics.cpp"

using T = Number<mpz_class, 2, 5, 13, 17>;
// using T = Float<double>;
using Stat = State<T>;
using Poi = Point<T>;
using PolyRay = std::vector<std::vector<Poi>>;

std::vector<std::vector<std::vector<double>>> used_polys_to_vector(const PolyRay& used_polys) {
    std::vector<std::vector<std::vector<double>>> vector;
    for(auto& poly: used_polys){
        std::vector<std::vector<double>> pol;
        for(auto& point: poly){
            double x = (double)point.get_x();
            double y = (double)point.get_y();
            std::vector<double> P{x,y};
            pol.push_back(P);
        }
        vector.push_back(pol);

    }
    return vector;
}


template <typename T>
Polygon<T> put_poly_in_place(State<T> const& state, unsigned i, unsigned jj){
    
    unsigned obtusest_node_index = state.current_polygon->get_obtusest_index();
    Node<T>& obtusest_node = state.current_polygon->ll_node_from_index(obtusest_node_index)->data;
    std::cout << "Obtusest node is ";
    obtusest_node.print();
    std::cout << "\n";


    Polygon<T> poly = Polygon<T>(polygons<T>::polyset.at(i));
    std::cout << "before\n";
    poly.print();
    std::cout << "after\n";
    poly.flip_x();
    poly.print();
    std::cout << "flipped\n";
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

    bool allow_reflection = true;
    
    State<T> state;

    // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 2, 9, 0, 0, 1, 0, 0, 0};
    auto indices = std::vector<unsigned>{10, 4};

    for(unsigned index: indices){
        auto next_states = state.find_next_states(allow_reflection);
        state = next_states.at(index);
    }

    unsigned i = 10;
    unsigned j = 1;
    Polygon<T> poly = put_poly_in_place(state, i, j);

    std::cout << "------- frame ----------\n";
    state.current_polygon->print();

    
    std::cout << "------------------------ suggested poly ------------------------\n";
    poly.print();

    
    auto used_polys = used_polys_to_vector(*state.used_polys);
    auto frame = state.frame_as_vector();
    Graphics graphics;
    graphics.draw_state(used_polys, frame);

    exit(1);
    bool over = state.current_polygon->overlaps(poly);
    std::cout << "Overlap: " << over << "\n";
    
    

    over = state.current_polygon->edge_node_intersection(poly);
    std::cout << "edge_node_intersection 1: " << over <<"\n";

    over = poly.edge_node_intersection(*state.current_polygon);
    std::cout << "edge_node_intersection 2: " << over << "\n";


    // Select nodes for is_inner_vertex and point_on_edge
    i = 2;
    LL_Node<Node<T>> *current{poly.head};
    for(unsigned ii=0; ii<i; ii++){
        current = current->next;
    }

    Point<T> const& P = current->data.position;
    Point<T> const& Q = current->prev->data.position;

    std::cout << "P: " << P.get_x().number_double << " " << P.get_y().number_double << "\n";
    std::cout << "Q: " << Q.get_x().number_double << " " << Q.get_y().number_double << "\n";
    
    
    j = 3;
    LL_Node<Node<T>> *other_current{state.current_polygon->head};
    for(unsigned jj=0; jj<j; jj++){
        other_current = other_current->next;
    }

    std::cout << "other: " << other_current->data.position.get_x().number_double 
    << " " << other_current->data.position.get_y().number_double << "\n";

    bool bo = point_on_edge(P, Q, other_current->data.position);
    std::cout << "point on edge?:" << bo << "\n";

    bo = is_inner_vertex(P, Q, other_current);
    std::cout << "is_inner_vertex?:" << bo << "\n";


}