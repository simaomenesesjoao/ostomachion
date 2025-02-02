#include "polygon.cpp"
#include "ostomachion.cpp"
#include <cassert>


int main(){

    using Poly = Polygon<2,5,13,17>;
    using Nod = Node<2,5,13, 17>;
    // using Poin = Point<2,5,13, 17>;

    {
        std::cout << "Testing polygon constructor" << std::endl;
        Poly poly{{}};
        assert(poly.head == nullptr);
        
    }

    {
        std::cout << "Testing polygon copy" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2(poly1);
   
        assert(poly2 == poly1);
    }

    {
        std::cout << "Testing polygon flip_x" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {-12,12}, {0,12}});
        poly1.flip_x();
        assert(poly2 == poly1);
    }


    {
        std::cout << "Testing polygon get_obtusest" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {-12,12}, {0,12}});
        Poly poly3({{0,0}, {0,12}, {3,0}});

        // std::cout << poly1 << std::endl;
        // std::cout << poly1 << std::endl;
        // std::cout << poly1 << std::endl;


        unsigned index1 = poly1.get_obtusest_index();
        unsigned index2 = poly2.get_obtusest_index();
        unsigned index3 = poly3.get_obtusest_index();

        Nod const& node1 = poly1.ll_node_from_index(index1)->data;
        Nod const& node2 = poly2.ll_node_from_index(index2)->data;
        Nod const& node3 = poly3.ll_node_from_index(index3)->data;
        // Nod const& node1 = poly1.get_obtusest_node();
        std::cout << node1.position << " " << (float)node1.angle_opening << std::endl;
        std::cout << node2.position << " " << (float)node2.angle_opening << std::endl;
        std::cout << node3.position << " " << (float)node3.angle_opening << std::endl;
        
    }


// return 0;
    {
        std::cout << "Testing is_point_inside" << std::endl;
        Poly poly({{0,0}, {12,0}, {12,12}, {6,6}, {0,12}}); // Anti-clockwise polygon
        Poly poly2({{0,12}, {6,6}, {12,12}, {12,0}, {0,0}}); // Clockwise polygon

        assert(poly.is_point_inside({10,2 }) == true);  // inside
        assert(poly.is_point_inside({12,10}) == false); // on the border
        assert(poly.is_point_inside({13,10}) == false); // outside


        assert(poly2.is_point_inside({10,2 }) == false);  // inside
        assert(poly2.is_point_inside({12,10}) == false); // on the border
        assert(poly2.is_point_inside({13,10}) == true); // outside

        assert(poly.is_point_inside({1,1}, {-1,-1}) == true);  // inside, cross vertex
        assert(poly.is_point_inside({5,5}, {13,13}) == true);  // inside, coincident edge
        assert(poly.is_point_inside({-1,0}, {13,0}) == false); // outside, coincident edge
        

    }
    

    {
        std::cout << "Testing polygon intersection" << std::endl;

        Poly poly1({{0,0}, {3,0}, {3,3}, {0,3}});
        Poly poly2({{1,1}, {2,1}, {2,2}, {1,2}});
        Poly poly3({{2,2}, {4,2}, {4,4}, {2,4}});
        Poly poly4({{1,0}, {2,0}, {2,1}, {1,1}});
        Poly poly5({{3,0}, {6,3}, {3,6}, {0,3}});
        Poly poly6({{0,0}, {0,1}, {1,1}, {2,2}, {2,0}, {1,-1}});
        Poly poly7({{0,0}, {1,1}, {2,0}});
        Poly poly8({{1,0}, {3,1}, {2,3}, {0,2}});
        Poly poly9({{1,0}, {2,0}, {2,3}, {1,3}});
        Poly poly10({{0,0}, {4,0}, {2,3}, {1,2}});


        assert(poly1.points_inside(poly2) == true);
        assert(poly2.points_inside(poly1) == false);
        assert(poly1.points_inside(poly3) == true);
        assert(poly3.points_inside(poly1) == true);

        assert(poly1.points_inside(poly1) == false);
        assert(poly4.points_inside(poly3) == false);

        assert(poly1.edge_edge_intersection(poly2) == false);
        assert(poly1.edge_edge_intersection(poly3) == true);
        assert(poly1.edge_edge_intersection(poly4) == false);

        assert(poly1.node_node_intersection(poly5) == true);
        assert(poly1.node_node_intersection(poly4) == false);
        assert(poly6.node_node_intersection(poly7) == true);

        assert(poly1.edge_node_intersection(poly8) == true);
        assert(poly8.edge_node_intersection(poly1) == false);
        assert(poly1.edge_node_intersection(poly9) == true);
        assert(poly9.edge_node_intersection(poly1) == false);

        poly1.overlaps(poly2);
        poly1.overlaps(poly3);
        poly1.overlaps(poly1);
        poly1.overlaps(poly5);
        poly6.overlaps(poly7);
        poly1.overlaps(poly8);
        poly1.overlaps(poly9);
        poly1.overlaps(poly10);



    }

    // {

    //     std::cout << "Testing polygon intersection, pt2" << std::endl;
    //     Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    //     Poly poly2({{0,12}, {0,8}, {3,8}});
    //     std::cout << poly1.points_inside(poly2) << std::endl;
    //     std::cout << poly2.points_inside(poly1) << std::endl;

    //     // int variable = 3;
    //     // std::cout << poly1.is_point_inside({3,8}) << std::endl;
    // }

    // return 0;






    {
        std::cout << "Testing simple merge" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});
        Poly poly3({{0,0}, {0,6}, {6,6}, {0,0}, {0,12}, {12,12}});
   
        poly2.merge(poly2.head, poly1, poly1.head);
        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing linked list after merge" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});

        poly2.merge(poly2.head, poly1, poly1.head);
        
        LL_Node<Nod> *node = poly2.head;
        for(unsigned i=0; i<poly2.size_ll*2-1; i++){
            node = node->next;
        }
        assert(node == poly2.head->prev);

        node = poly2.head;
        for(unsigned i=0; i<poly2.size_ll*2+1; i++){
            node = node->prev;
        }
        assert(node == poly2.head->prev);
    }

    {
        std::cout << "Testing pruning square+triangle" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {3,0}, {3,3}, {0,3}});
        Poly poly3({{3,0}, {3,3}, {0,3}, {0,12}, {12,12}, {12,0}});
   
        LL_Node<Nod> *node1 = poly1.head;
        LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(node2, poly1, node1);
        poly2.prune_LL({node1, node2});
        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing pruning two rectangles" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,3}, {0,3}});
        Poly poly3({{12,3}, {0,3}, {0,12}, {12,12}});
   
        LL_Node<Nod> *temp1 = poly1.head;
        LL_Node<Nod> *temp2 = poly2.head;
        poly2.merge(poly2.head, poly1, poly1.head);
        poly2.prune_LL({temp1, temp2});

        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing pruning two identical rectangles" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,12}, {0,12}});

        LL_Node<Nod> *node1 = poly1.head;
        LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(node2, poly1, node1);
        poly2.prune_LL({node1, node2});
        assert(poly2.head == nullptr);  
    }


    {
        std::cout << "Testing polygon translation and rotation" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{8,0}, {12,0}, {12,3}});
        // Poly poly3({{0,12}, {0,8}, {3,8}});
        Nod const& R1 = poly1.head->next->data;
        Nod const& R2 = poly2.head->data;
        poly2.translate(R1.position-R2.position);
        std::cout << poly2 << std::endl;
        std::cout << "overlaps? " << poly1.overlaps(poly2) << std::endl;
        poly2.rotate(R1.angle_end - R2.angle_start, R1.position);
        std::cout << poly2 << std::endl;
        // bool overlaps = poly1.overlaps(poly3);
        std::cout << "overlaps? " << poly1.overlaps(poly2) << std::endl;


    }



}