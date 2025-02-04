#include "ostomachion.cpp"
#include "polygon.cpp"
#include <format>
#include <cstdlib>
#include <gmpxx.h>
#include <string>
#include "state.cpp"

template <typename T>
std::string get_name(){
    return "";
}

template <>
std::string get_name<mpz_class>(){
    return "mpz_class";
}

template <>
std::string get_name<int>(){
    return "int";

}
// using T = int;
// using T = mpz_class;

// bool PlotPython(Poly const& poly){
    // std::string python_argument = "positions: [";
    
    // LL_Node<Nod> *current = poly.head;
    // for(unsigned i=0; i< poly.size_ll; i++){
    //     double x = (double)current->data.position.get_x();
    //     double y = (double)current->data.position.get_y();
    //     python_argument += std::format("({:.2f},{:.2f}) ", x, y);
    //     current = current->next;
    // }
    // python_argument += "]";

    // std::string command = "python3 visualizer.py \"" + python_argument + "\"";
    // system(command.c_str());
//     return true;

// }

template <typename T>
void test(bool target1, bool target2){
    std::cout << "Testing polygon overlap for type " << get_name<T>() << std::endl;

    using Num = Number<T,2,5,13,17>;
    using Poly = Polygon<Num>;
    using Nod = Node<Num>;

    
    Poly frame(polygons<T>::frame);
    Nod& obtusest_node = frame.head->data;

    // Find the node from the polygon that will attach to that node
    Poly poly1(polygons<T>::polyset.at(0));
    Nod& proposed_node1 = poly1.head->data;
    poly1.translate(obtusest_node.position - proposed_node1.position);
    poly1.rotate(obtusest_node.angle_start - proposed_node1.angle_end, obtusest_node.position);
    
    Poly poly2(polygons<T>::polyset.at(1));
    Nod& proposed_node2 = poly2.head->data;
    poly2.translate(proposed_node1.position - proposed_node2.position);
    poly2.rotate(proposed_node1.angle_start - proposed_node2.angle_end, obtusest_node.position);
    
    bool intersects = edges_intersect(  poly2.head->next->data.position, 
                                        {100,101}, 
                                        poly1.head->next->data.position, 
                                        poly1.head->data.position);

    assert(intersects == target1);
    assert(poly2.overlaps(poly1) == target2);
    // std::cout << "intersects?:" << intersects << std::endl;
    // std::cout << "overlap?:" << poly2.overlaps(poly1) << std::endl;
}

int main(){

    // Testing numerical precision
    // test<int>(true, true);
    // test<mpz_class>(false, false);

    {
        using T = int;
        using Num = Number<T,2,5,13,17>;
        using Poly = Polygon<Num>;
        using Nod = Node<Num>;

        State<Num> state;
        // Tracker<2,3> tracker;
        std::vector<State<Num>> next_states;

        next_states = state.find_next_states();
        state = next_states.at(6);
        auto frame = state.current_polygon;
        unsigned obtusest_node_index = frame.get_obtusest_index();
        Nod& obtusest_node = frame.ll_node_from_index(obtusest_node_index)->data;

        // Nod& obtusest_node = frame.head->data;
        std::cout << "obtusest:" << std::endl;
        obtusest_node.print();
        std::cout << "." << std::endl;

        // Find the node from the polygon that will attach to that node
        Poly poly1(polygons<T>::polyset.at(3));
        poly1.print();
        Nod& proposed_node1 = poly1.head->data;
        poly1.translate(obtusest_node.position - proposed_node1.position);
        poly1.rotate(obtusest_node.angle_start - proposed_node1.angle_end, obtusest_node.position);

        poly1.print();
        
        // Poly poly2(polygons<T>::polyset.at(1));
        // Nod& proposed_node2 = poly2.head->data;
        // poly2.translate(proposed_node1.position - proposed_node2.position);
        // poly2.rotate(proposed_node1.angle_start - proposed_node2.angle_end, obtusest_node.position);
        
        // bool intersects = edges_intersect(  frame.head->next->data.position, 
        //                                     {100,101}, 
        //                                     poly1.head->next->data.position, 
        //                                     poly1.head->data.position);

        // std::cout << "intersects? " << intersects << std::endl;
        std::cout << "overlaps? " << frame.overlaps(poly1) << std::endl;
        std::cout << "overlaps? " << poly1.overlaps(frame) << std::endl;
        }
    return 0;

}