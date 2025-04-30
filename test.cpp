#include <iostream>

#include "intersection_algorithms.cpp"
#include "polygon.cpp"
#include "customFloat.cpp"

int main(){
// {
//     //using Num = Number<int, 2, 3>;
//     using Num = Float<double>;
//     Angle<Num> A{0,1}, B{-1,1}, C{-1,-1}; // 90, 135, 225
//     std::cout << A.get_cos() << " " << A.get_sin() << "\n";
//     std::cout << B.get_cos() << " " << B.get_sin() << "\n";
//     std::cout << C.get_cos() << " " << C.get_sin() << "\n";

//     assert(angles_compatible(A,B) == true);
//     assert(angles_compatible(B,A) == true);
//     assert(angles_compatible(C,C) == false);
//     assert(angles_compatible(A,C) == true);
//     assert(angles_compatible(B,C) == true);
// }

// {
//     using Num = Float<double>;
//     using P = std::vector<std::vector<int>>;
//     using V = Vertex<Num>;

//     // std::cout << "FlipX\n";
//     // Polygon::ContigPoly<Num> poly(P({{1,2},{2,3},{3,1}}));
//     // poly.print();
//     // poly.flip_x();
//     // poly.print();
//     auto getter = [](unsigned int){return 0;}; // SIMAO: permitir seleccionar getter

//     std::cout << "ContigPoly\n";
//     Polygon::ContigPoly<Num> poly1(P({{1,2},{2,3},{3,1}}));
//     Polygon::ContigPoly<Num> poly2(P({{1,2},{1,4},{2,3}}));

//     poly1.print();

//     std::cout << poly1.get_head() << "\n";
//     for(const auto& vertex: poly1.get_vertices())
//         std::cout << &vertex << " ";
//     std::cout << "\n";


//     std::cout << "ContigPoly\n";
//     V *head1 = poly1.get_head();
//     V *head2 = poly2.get_head();
//     auto modified = poly1.merge(head1, poly2, head2);
//     std::cout << "modified:\n";
//     for(auto& m: modified){
//         std::cout << m << " ";
//         m->print();
//     }
//     std::cout << "done showing modified.\n";
//     poly1.print();
//     poly1.prune_LL(modified, getter);
//     std::cout << "pruned ContigLPoly\n";
//     poly1.print();

//     std::cout << "LLPoly\n";
//     Polygon::LLPoly<Num> poly3(P({{1,2},{2,3},{3,1}}));
//     Polygon::LLPoly<Num> poly4(P({{1,2},{1,4},{2,3}}));
//     V *head3 = poly3.get_head();
//     V *head4 = poly4.get_head();
//     auto modified2 = poly3.merge(head3, poly4, head4);
//     poly3.print();
//     poly3.prune_LL(modified2, getter);
//     std::cout << "pruned LLPoly\n";
//     poly3.print();
// }




// {
//     using Num = Float<double>;
//     using P = std::vector<std::vector<int>>;
//     using V = Vertex<Num>;

//     auto getter = [](unsigned int){return 0;}; // SIMAO: permitir seleccionar getter

    
//     std::cout << "Testing PruneLL\n";

//     Polygon::ContigPoly<Num> poly1(P({{1,2},{2,3},{3,1}}));
//     Polygon::ContigPoly<Num> poly2(P({{1,2},{3,1},{2,3}}));



//     std::cout << "ContigPoly\n";
//     V *head1 = poly1.get_head();
//     V *head2 = poly2.get_head();
//     auto modified = poly1.merge(head1, poly2, head2);
//     poly1.prune_LL(modified, getter);
//     std::cout << "pruned ContigLPoly\n";
//     poly1.print();

//     assert(poly1.get_head() == nullptr);
//     assert(poly1.get_vertices().size() == 0);

//     std::cout << "LLPoly\n";
//     Polygon::LLPoly<Num> poly3(P({{1,2},{2,3},{3,1}}));
//     Polygon::LLPoly<Num> poly4(P({{1,2},{3,1},{2,3}}));
//     V *head3 = poly3.get_head();
//     V *head4 = poly4.get_head();
//     auto modified2 = poly3.merge(head3, poly4, head4);
//     poly3.prune_LL(modified2, getter);
//     std::cout << "pruned LLPoly\n";
//     poly3.print();

//     assert(poly3.get_head() == nullptr);
// }

{
    
    // using Num = Number<int, 2, 5, 13, 17>;
    //using Poly = Polygon::LLPoly<Num>;
    using Num = Float<double>;
    using Poly = Polygon::ContigPoly<Num>;
    
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


}