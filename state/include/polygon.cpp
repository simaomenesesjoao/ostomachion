#ifndef POLYGON_C
#define POLYGON_C

#include <list>
#include "points_vectors.cpp"
#include "intersection_algorithms.cpp"
#include <iostream>
#include <unordered_set>
#include <cassert>



template <typename Num>
class Polygon{

private:
    using Nod  = Node<Num>;
    using Poin = Point<Num>;
    using Ang  = Angle<Num>;

public:
    // std::list<Nod> nodes;
    LL_Node<Nod> *head;
    unsigned size_ll;
    bool area_positive;
    bool first;
    
    void set_first(bool value){
        first = value;
    }

    Polygon(std::vector<std::vector<int>> points):head{nullptr}, size_ll{0}, area_positive{false}, first{false}{
        // Turn the list of points into nodes, set them as the vertices of 
        // the polygon and calculate the angles between them

        if(points.size() == 0){
            return;
        }

        std::vector<int> last_point = points.back();
        int prev_x{last_point.at(0)}, prev_y{last_point.at(1)};
        LL_Node<Nod> *current{}; // deal with Warning about it being uninitialized

        size_ll = 0;

        // std::cout << "Creating polygon" << std::endl;
        for(auto& point: points){
            
            int x{point.at(0)}, y{point.at(1)};
            Ang angle(x-prev_x, y-prev_y);
            Nod node(Poin{prev_x,prev_y});
            node.update_start(angle);
            LL_Node<Nod> *ll_node = new LL_Node<Nod>(node);

            // std::cout << "xy" << x << " " << y << std::endl;
            if(size_ll == 0){
                head = ll_node;
                current = head;
            } else {
                current->next = ll_node;
                ll_node->prev = current;
                current = ll_node;
            }
            size_ll++;

            prev_x = x;
            prev_y = y;
        }

        assert(current != nullptr);

        current->next = head;
        head->prev = current;
        head = head->next;

        current = head;
        LL_Node<Nod> *prev = head->prev;
        for(unsigned i=0; i<size_ll; i++){
            current->data.update_end(-prev->data.angle_start);
            current->data.update_opening();
            prev = current;
            current = current->next;
        }

        area_positive = (get_area() > 0);
        
    }


    void build_this(Polygon const& other_poly){

        area_positive = other_poly.area_positive;
        if(other_poly.size_ll == 0){
            size_ll = 0;
            head = nullptr;
            return;
        }

        head = new LL_Node<Nod>(other_poly.head->data);
        LL_Node<Nod> *node = head;        
        LL_Node<Nod> *other_node = other_poly.head->next;

        for(unsigned i=1; i<other_poly.size_ll; i++){
            node->next = new LL_Node<Nod>(other_node->data);
            node->next->prev = node;

            other_node = other_node->next;
            node = node->next;

        }
        size_ll = other_poly.size_ll;
        node->next = head;
        head->prev = node;
    }

    void delete_this(){
        LL_Node<Nod> *current, *next;
        current = head;
        for(unsigned i = 0; i < size_ll; i++){
            next = current->next;
            delete current;
            current = next;
        }
    }

    Polygon(Polygon const& other_poly):first{false}{
        build_this(other_poly);
    }

    ~Polygon(){
        delete_this();
    }



    Polygon& operator=(Polygon const& other){
        if(&other == this)
            return *this;

        delete_this();
        build_this(other);
        return *this;
    }

    Num get_area(){
        std::vector<std::pair<Num, Num>> points;
        LL_Node<Nod> *current = head;
        for(unsigned i=0; i<size_ll; i++){
            Num const& x = current->data.position.get_x();
            Num const& y = current->data.position.get_y();
            points.push_back({x,y});
            current = current->next;
        }

        return shoelace_area<Num>(points);
    }

    void translate(Poin const& dr){
        LL_Node<Nod> *ll_node = head;
        for(unsigned i=0; i<size_ll; i++){
            ll_node->data.position = ll_node->data.position + dr;
            ll_node = ll_node->next;
        }
    }

    void rotate(Ang const& angle, Poin const& rot_center){
        // Rotate the polygon by angle around the rotation center

        LL_Node<Nod> *ll_node = head;
        for(unsigned i=0; i<size_ll; i++){
            Nod& node = ll_node->data;
            node.angle_end = node.angle_end + angle;
            node.angle_start = node.angle_start + angle;
            node.position = rot_center + (node.position - rot_center).rotate(angle);
            ll_node = ll_node->next;
        }
    }

    // void rotate90(Poin const& rot_center){
    //     // Rotate the polygon by angle around the rotation center
    //     Ang const& angle, 
    //     rotate(angle)
    // }

    void flip_x(){
        // Flip the polygon along x, but preserve orientation. In practice, 
        // this means changing the sign of every quantity which depends on x
        LL_Node<Nod> *current{head}, *temp;

        for(unsigned i=0; i<size_ll; i++){
            temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current->data.position = {-current->data.position.get_x(), current->data.position.get_y()};

            auto start = current->data.angle_start;
            auto end = current->data.angle_end;
            current->data.angle_start = Ang{-end.get_cos(), end.get_sin()};
            current->data.angle_end = Ang{-start.get_cos(), start.get_sin()};
            
            current = temp;
        }
        
    }


    void flip_y(){
        // Flip the polygon along y, but preserve orientation. In practice, 
        // this means changing the sign of every quantity which depends on y
        LL_Node<Nod> *current{head}, *temp;

        for(unsigned i=0; i<size_ll; i++){
            temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current->data.position = {current->data.position.get_x(), -current->data.position.get_y()};

            auto start = current->data.angle_start;
            auto end = current->data.angle_end;
            current->data.angle_start = Ang{end.get_cos(), -end.get_sin()};
            current->data.angle_end = Ang{start.get_cos(), -start.get_sin()};
            
            current = temp;
        }   
    }

    void merge(LL_Node<Nod> *this_node, Polygon & other_poly, LL_Node<Nod> *other_node){

        // Connect the two linked lists
        LL_Node<Nod> *temp = other_node->prev;
        
        other_node->prev = this_node->prev;
        other_node->prev->next = other_node;

        this_node->prev = temp;
        this_node->prev->next = this_node;

        // Delete the linked list from one of the polygons to 
        // avoid deleting twice in the destructor
        unsigned new_size = other_poly.size_ll + this->size_ll;
        this->size_ll = new_size;
        other_poly.size_ll = 0;
        other_poly.head = nullptr;

        // Update the angles
        this_node->data.angle_end = -this_node->prev->data.angle_start;
        other_node->data.angle_end = -other_node->prev->data.angle_start;

        this_node->data.update_opening();
        other_node->data.update_opening();
    }

    template <typename Getter>
    void prune_LL(std::vector<LL_Node<Nod>*> update, const Getter& getter, std::ostream& stream = std::cout){
        // Remove points from the LL which are coincident or have
        // an opening angle of 0. This is repeated until no more points
        // can be removed. It may happen that all points are removed, in
        // which case the LL head becomes a nullptr
        // std::unordered_set<LL_Node<Nod>*> visited;

        stream << "Entered prune_LL with the following frame ";
        LL_Node<Nod> *current = head;
        for(unsigned i=0; i<size_ll; i++){
            stream << current->data.position << " ";
            current = current->next;
        }
        stream << "\n";
        
        


        while(update.size()){
            
            if(size_ll == 1){
                delete head;
                head = nullptr;
                size_ll = 0;
                break;
            }


            // auto it = update.end() - 1;
            // if(first){
            // }

            int i = getter.get(update.size());
            auto it = update.begin()+i;
            

            LL_Node<Nod> *node = *it;
            update.erase(it);

            if(std::find(update.begin(), update.end(), node) != update.end())
                continue;
            // auto [itt, successful] = visited.insert(node);
            // if(!successful){
            //     continue;
            // }

            Nod& curr = node->data;
            LL_Node<Nod> *prev_node = node->prev;
            LL_Node<Nod> *next_node = node->next;
            Nod& prev = node->prev->data;
            Nod& next = node->next->data;
            bool modified = true;

            stream << "   i=" << i;
            stream << " cpn: " << curr.position << " " << prev.position << " " << next.position << "\n";
            
            if(curr.position == prev.position){
                prev.angle_start = curr.angle_start;
                prev.update_opening();

            } else if(curr.position == next.position){
                next.angle_end = curr.angle_end;
                next.update_opening();


            } else if(curr.angle_opening.is_zero()){

                // Check which neighbour is closest
                Poin vec_prev = curr.position - prev.position;
                Num dist_prev2 = vec_prev.get_x()*vec_prev.get_x() + vec_prev.get_y()*vec_prev.get_y();

                Poin vec_next = curr.position - next.position;
                Num dist_next2 = vec_next.get_x()*vec_next.get_x() + vec_next.get_y()*vec_next.get_y();

                if(dist_prev2 < dist_next2){
                    prev.angle_start = -prev.angle_start;
                    prev.update_opening();

                } else {
                    next.angle_end = -next.angle_end;
                    next.update_opening();
                }
            } else if(curr.angle_opening.is_180()){
                // Nothing to do in this case
            } else {
                modified = false;
            }

            if(modified){
                stream << "   modified\n";

                next_node->prev = prev_node;
                prev_node->next = next_node;
                if(node == head){
                    head = node->next;
                }

                delete node;
                size_ll--;
                update.push_back(next_node);
                update.push_back(prev_node);
            }   
        }


        stream << "Left prune_LL with the following frame ";
        current = head;
        for(unsigned i=0; i<size_ll; i++){
            stream << current->data.position << " ";
            current = current->next;
        }
        stream << "\n";

    }

    bool is_canonical() const {
        // SIMAO
        // Check if the polygon doesn't have 180 degrees, 0 degrees or coincident points
        // Optionally, I can also check if the angles make sense using doubles
        return false;
    }


    std::size_t get_hash() const {
        std::size_t h = 0;
        LL_Node<Nod> *current = head;
        for(unsigned i=0; i<size_ll; i++){
            auto& pos = current->data.position;
            h += pos.get_x().get_hash()*4872191 + pos.get_y().get_hash()*991911983;

            h += current->data.angle_start.get_sin().get_hash();
            h += current->data.angle_end.get_sin().get_hash();
            h += current->data.angle_opening.get_sin().get_hash();
            
            h += current->data.angle_start.get_cos().get_hash();
            h += current->data.angle_end.get_cos().get_hash();
            h += current->data.angle_opening.get_cos().get_hash();
            h = h%230984193;
            current = current->next;
        }

        return h;
    }

    bool operator==(Polygon const& other_poly) const{
        LL_Node<Nod> *list1{this->head}, *list2{other_poly.head};

        if(size_ll != other_poly.size_ll)
            return false;

        // The polygons might be identical even though the heads are different
        for(unsigned i=0; i<size_ll; i++){

            if(list1->data.position == list2->data.position)
                break;

            list2 = list2->next;
        }


        for(unsigned i=0; i<size_ll; i++){
            if(!(list1->data.position == list2->data.position))
                return false;

            if(list1->data.angle_end != list2->data.angle_end)
                return false;

            if(list1->data.angle_start != list2->data.angle_start)
                return false;

            if(list1->data.angle_opening != list2->data.angle_opening)
                return false;

            list1 = list1->next;
            list2 = list2->next;
        }

        return true;
    }

    void print() const{
        LL_Node<Nod> *ll_node = head;
        for(unsigned i=0; i<size_ll; i++){
            Nod& node = ll_node->data;
            ll_node = ll_node->next;
            
            node.print();
        }
    }

    bool is_point_inside(Poin const& P, Poin const& Q = {100,101}) const{
        // Checks whether point P is inside the polygon. 
        // Returns false if it lies on the border or on a vertex
        // std::cout << "inside is_point_inside" << std::endl;

        // Poin Q(Num{} + Fraction{10000,101}, Num{} + Fraction{20000,199});
        // Poin Q(100,101);
        // std::cout << "Q: " << (double)Q.get_x() << " " << (double)Q.get_y() << std::endl;
        // std::cout << "P: " << (double)P.get_x() << " " << (double)P.get_y() << std::endl;
        
        // Check if the point P is part of a vertex or edge
        {           
            LL_Node<Nod> *current = head;
            for(unsigned i = 0; i < size_ll; i++){
                if(P == current->data.position)
                    return false;

                if(point_on_edge(current->data.position, current->prev->data.position, P))
                    return false;

                current = current->next;
            }
        }        
        
        
        LL_Node<Nod> *current = head;
        int num_intersections{0};
        for(unsigned i = 0; i < size_ll; i++){
            // std::cout << "edge: " << current->data.position << " " << current->prev->data.position << std::endl;

            if(edges_intersect(P, Q, current->data.position, current->prev->data.position)){
                num_intersections++;
                // std::cout << "There's intersection" << std::endl;
            } 
                
            if(point_on_edge(P, Q, current->data.position)){

                // std::cout << "point is on the edge" << std::endl;
                if(edge_splits_vertex(P, Q, current)){
                    num_intersections++;
                    // std::cout << "vertex split" << std::endl;
                }

            }


            if(edge1_includes_edge2(P, Q, current->data.position, current->prev->data.position)){
                // std::cout << "edges coincide" << std::endl;
                if(coincident_edges_diverge(P, Q, current, current->prev)){
                    num_intersections++;
                    // std::cout << "edges open to different sides" << std::endl;

                }

            }
            

            current = current->next;            
        }

        // The number of intersections will be odd if it's clockwise
        // clockwise = negative area
        if(num_intersections%2 == area_positive)
            return true;

        return false;

    }

    bool edge_edge_intersection(Polygon const& other) const{
        LL_Node<Nod> *current{head};

        // Check for edge-edge intersections
        for(unsigned i=0; i<size_ll; i++){
            Poin& A = current->data.position;
            Poin& B = current->prev->data.position;
            LL_Node<Nod> *other_current{other.head};

            for(unsigned j=0; j<other.size_ll; j++){
                Poin& C = other_current->data.position;
                Poin& D = other_current->prev->data.position;

                if(edges_intersect(A,B,C,D)){
                    return true;
                }

                
                other_current = other_current->next;
            }

            current = current->next;
        }

        return false;
    }

    bool edge_node_intersection(Polygon const& other) const{
        // Check if an edge of 'this' polygon intersects a vertex of the 'other'


        LL_Node<Nod> *current{head};
        for(unsigned i=0; i<size_ll; i++){

            Poin const& P = current->data.position;
            Poin const& Q = current->prev->data.position;

            LL_Node<Nod> *other_current{other.head};
            for(unsigned j=0; j<other.size_ll; j++){

                if(point_on_edge(P, Q, other_current->data.position)){
                    if(is_inner_vertex(P, Q, other_current)){
                        // std::cout << "ij: " << i << " " << j << "\n";
                        // std::cout << "PQ: " << P << " " << Q << " " << "\n";
                        // std::cout << "other:" << other_current->data.position << "\n\n";
                        
                        return true;
                    }
                }
                other_current = other_current->next;
            }
            current = current->next;
        }

        return false;
    }

    bool node_node_intersection(Polygon const& other) const{
        

        LL_Node<Nod> *current{head};
        for(unsigned i=0; i<size_ll; i++){
            Nod& A = current->data;

            LL_Node<Nod> *other_current{other.head};
            for(unsigned j=0; j<other.size_ll; j++){
                Nod const& B = other_current->data;

                if(A.position == B.position){
                    // std::cout << "positions " << A.position << " are identical" << std::endl; 
                    if(not nodes_compatible(A,B)){
                        // std::cout << "nodes are not compatible" << std::endl;
                        return true;
                    }
                }
                other_current = other_current->next;
            }
            current = current->next;
        }

        return false;
    }

    bool points_inside(Polygon const& other) const {
        // Checks if any vertex from polygon 'other' is inside this polygon


        LL_Node<Nod> *current{other.head};
        for(unsigned i=0; i<size_ll; i++){
            Point<Num>& A = current->data.position;
            // std::cout << "point" << A << std::endl;
            if(is_point_inside(A)){
                // std::cout << "is inside" << std::endl;
                return true;
            }

            current = current->next;
        }

        return false;
    }

    bool overlaps(Polygon const& other) const{

        // bool cond1 = edge_edge_intersection(other);
        // bool cond2 = node_node_intersection(other);
        // bool cond3 = edge_node_intersection(other);
        // bool cond4 = other.edge_node_intersection(*this);
        // bool cond5 = points_inside(other);
        // bool cond6 = other.points_inside(*this);

        // std::cout << "____\nedge edge: "  << cond1 << std::endl;
        // std::cout << "node node: "  << cond2 << std::endl;
        // std::cout << "edge node: "  << cond3 << std::endl;
        // std::cout << "node edge: "  << cond4 << std::endl;
        // std::cout << "points in1: " << cond5 << std::endl;
        // std::cout << "points in2: " << cond6 << std::endl;
        // return cond1 or cond2 or cond3 or cond4 or cond5 or cond6;

        // return edge_edge_intersection(other);
        return edge_edge_intersection(other) or  node_node_intersection(other) 
        or edge_node_intersection(other) or other.edge_node_intersection(*this) 
             or points_inside(other) or other.points_inside(*this);
    }

    LL_Node<Nod>* ll_node_from_index(unsigned index) const{
        LL_Node<Nod> *current = head;
        for(unsigned i=0; i<index; i++){
            current = current->next;
        }
        return current;
    }

    unsigned get_obtusest_node() const {
        // Get the node with the largest internal opening
        LL_Node<Nod> *current = head;
        LL_Node<Nod> *largest = head;
        unsigned index_largest = 0;
        for(unsigned i = 0; i < size_ll; i++){
            if(largest->data.angle_opening < current->data.angle_opening){
                largest = current;
                index_largest = i;
            }
            current = current->next;
        }
        return index_largest;
    }

    unsigned get_leftest_node() const {
        // Get the left-most node, then bottom-most
        LL_Node<Node<Num>> *current = head;
        LL_Node<Node<Num>> *leftest = head;
        unsigned index_leftest = 0;
        for(unsigned i = 0; i < size_ll; i++){
            if(current->data.position.get_x() < leftest->data.position.get_x() or 
                (current->data.position.get_x() == leftest->data.position.get_x() and 
                 current->data.position.get_y() <  leftest->data.position.get_y())){

                    leftest = current;
                    index_leftest = i;
            }

            current = current->next;
        }
        return index_leftest;
        
    }


    std::vector<std::vector<double>> as_vector() const {
        std::vector<std::vector<double>> vector;
        LL_Node<Nod>* current{head}; 
        for(unsigned i=0; i<size_ll; i++){
            double x = (double)current->data.position.get_x();
            double y = (double)current->data.position.get_y();
            std::vector<double> point{x,y};
            vector.push_back(point);
            current = current->next;
        }
        
        return vector;
    }


};

template <typename Num>
std::ostream& operator<<(std::ostream& stream, Polygon<Num> const& poly){
    LL_Node<Node<Num>> *ll_node = poly.head;
    
    for(unsigned i = 0; i < poly.size_ll; i++){
        stream << ll_node->data << "\n";
        ll_node = ll_node->next;
    }

    return stream;
}

#endif // POLYGON_C