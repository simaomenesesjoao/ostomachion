#include "polygon.cpp"
#include <cassert>


int main(){

    using Poly = Polygon<2,5,13,17>;
    using Nod = Node<2,5,13, 17>;


    // Testing the merge
    {
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});
        Poly poly3({{0,0}, {0,6}, {6,6}, {0,0}, {0,12}, {12,12}});
   
        LL_Node<Nod> *node1 = poly1.head;
        LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(node2, poly1, node1);
        // poly2.print();
        // poly3.print();

        assert(poly2 == poly3);
    }


    return 1;
    // Testing the merge
    {
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});

        // LL_Node<Nod> *node1 = poly1.head;
        // LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(poly2.head, poly1, poly1.head);
        
        // LL_Node<Nod> *node = poly2.head;
        // for(unsigned i=0; i<poly2.size_ll*2; i++){
        //     node->data.print();
        //     node = node->next;
        // }

        // node = poly2.head;
        // for(unsigned i=0; i<poly2.size_ll*2; i++){
        //     node->data.print();
        //     node = node->prev;
        // }

    }


    // return 1;

    {
        // std::cout << "------- Test 3 -------" << std::endl;
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
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,3}, {0,3}});
        Poly poly3({{12,3}, {0,3}, {0,12}, {12,12}});
   

        LL_Node<Nod> *temp1 = poly1.head;
        LL_Node<Nod> *temp2 = poly2.head;
        poly2.merge(poly2.head, poly1, poly1.head);
        poly2.prune_LL({temp1, temp2});

        // std::cout << "------ polygon 2 after prune" << std::endl;
        poly2.print();
        assert(poly2 == poly3);
    }




    {
        // std::cout << "------- Test 4 -------" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,12}, {0,12}});

        LL_Node<Nod> *temp1 = poly1.head;
        LL_Node<Nod> *temp2 = poly2.head;
        poly2.merge(poly2.head, poly1, poly1.head);

        // std::cout << "polygon 2 after merge" << std::endl;
        // poly2.print();
        // std::cout << "polygon 1 after merge" << std::endl;
        // poly1.print();

        // std::cout << "prunning" << std::endl;
        poly2.prune_LL({temp1, temp2});

        
        // std::cout << "------ polygon 2 after prune" << std::endl;
        // poly2.print();

        assert(poly2.head == nullptr);  
    }


    // {
    //     using Poly = Polygon<2,5,13,17>;
    //     using Nod = Node<2,5,13, 17>;
    //     Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    //     Poly poly2({{0,0}, {3,0}, {2,4}});
    //     // Poly poly3({{0,0}, {4,8}, {2,10}});
    //     // Poly poly3{poly2};

    //     std::cout << "size1: " << poly1.size_ll << std::endl;
    //     std::cout << "polygon 1:" << std::endl;
    //     poly1.print();

    //     std::cout << "polygon 2:" << std::endl;
    //     poly2.print();



    //     LL_Node<Nod> *temp1 = poly1.head;
    //     LL_Node<Nod> *temp2 = poly2.head;
    //     poly2.merge(poly2.head, poly1, poly1.head);

    //     std::cout << "polygon 2 after merge" << std::endl;
    //     poly2.print();
    //     std::cout << "polygon 1 after merge" << std::endl;
    //     poly1.print();

    //     std::cout << "prunning" << std::endl;

    //     // std::unordered_set<LL_Node<Nod>*> update{temp1, temp2};
    //     poly2.prune_LL({temp1, temp2});

    //     std::cout << "polygon 2 after prune" << std::endl;
    //     poly2.print();
    // }

    return 0;
}