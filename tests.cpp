#include "ostomachion.cpp"
#include "polygon.cpp"
#include <format>
#include <cstdlib>

using Poly = Polygon<2,5,13,17>;
using Nod = Node<2,5,13,17>;

bool PlotPython(Poly const& poly){
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
    return true;

}

int main(){



    {
        std::cout << "Test 1" << std::endl;
        Polygon<2,5,13,17> frame(polygons::frame);
        Nod& obtusest_node = frame.head->data;

        // Find the node from the polygon that will attach to that node
        Poly poly1(polygons::polyset.at(0));
        Nod& proposed_node1 = poly1.head->data;
        poly1.translate(obtusest_node.position - proposed_node1.position);
        poly1.rotate(obtusest_node.angle_start - proposed_node1.angle_end, obtusest_node.position);
        
        Poly poly2(polygons::polyset.at(1));
        Nod& proposed_node2 = poly2.head->data;
        poly2.translate(proposed_node1.position - proposed_node2.position);
        poly2.rotate(proposed_node1.angle_start - proposed_node2.angle_end, obtusest_node.position);
        
        bool intersects = edges_intersect(  poly2.head->next->data.position, 
                                            {100,101}, 
                                            poly1.head->next->data.position, 
                                            poly1.head->data.position);

        std::cout << "intersects?:" << intersects << std::endl;
        std::cout << "overlap?:" << poly2.overlaps(poly1) << std::endl;

        // PlotPython(poly1);
        // PlotPython(poly2);
    }
    return 0;


    {
        std::cout << "Test 2" << std::endl;
        Polygon<2,5,13,17> frame(polygons::frame);
        // Polygon<2,5,13,17> poly1(polygons::poly1);
        // Polygon<2,5,13,17> poly2(polygons::poly2);
        // Polygon<2,5,13,17> poly3(polygons::poly3);

        std::vector<unsigned> indices{0,1,2};
        for(auto& index: indices){
            Poly poly(polygons::polyset.at(index));

            // Find the node that will be attached to in the frame
            unsigned obtusest_node_index = frame.get_obtusest_index();
            Nod& obtusest_node = frame.ll_node_from_index(obtusest_node_index)->data;

            // Find the node from the polygon that will attach to that node
            LL_Node<Nod> *current = poly.head;
            Nod& proposed_node = current->data;

            // Move the polygon to the correct place
            assert(angles_compatible(proposed_node.angle_opening, obtusest_node.angle_opening));
            poly.translate(obtusest_node.position - proposed_node.position);
            poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, obtusest_node.position);

            PlotPython(poly);
            std::cout << "polygon print:" << std::endl;
            poly.print();

            PlotPython(frame);
            std::cout << "frame print:" << std::endl;
            frame.print();
            assert(not frame.overlaps(poly));


            LL_Node<Nod> *node_frame = frame.ll_node_from_index(obtusest_node_index);
            LL_Node<Nod> *node_poly = poly.ll_node_from_index(0);
            frame.merge(node_frame, poly, node_poly);
            frame.prune_LL({node_frame, node_poly});


            PlotPython(frame);
            std::cout << "frame print:" << std::endl;
            frame.print();

        }


    }

    return 0;

}