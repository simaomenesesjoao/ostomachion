#ifndef POLYGON_C
#define POLYGON_C

#include <list>
#include "points_vectors.cpp"
#include <iostream>
#include <unordered_set>


template <int... Ints>
class Node{
    using Ang = Angle<Ints...>;
    using Num = Number<Ints...>;
    using Poin = Point<Ints...>;

public:
    Node(Poin const& P): position{P}{};



    void update_opening(){
        if(end_init and start_init)
            angle_opening = angle_end - angle_start;
        
    }

    void update_end(Ang const& A){
        angle_end = A;
        end_init = true;
        // update_opening();
    }

    void update_start(Ang const& A){
        angle_start = A;
        start_init = true;
        // update_opening();
    }

    void print(){
        Poin pos = position;
        Num x = pos.get_x();
        Num y = pos.get_y();
        Ang ang_i = angle_start;
        Ang ang_f = angle_end;
        Ang ang = angle_opening;
        std::cout << "(x,y)=(" << (float)x << "," << (float)y << ") " 
                    << (float)ang_i << " " << (float)ang_f << " " << (float)ang << std::endl;

    }

    Poin position;

    template <int...Args>
    friend std::ostream& operator<<(std::ostream& os, Node<Args...> const& node);

    Ang angle_start, angle_end, angle_opening;
// private:
    bool end_init, start_init;
};


template <typename T>
struct LL_Node{
    T data;
    LL_Node *prev, *next;

    LL_Node(T node): data(node){
        prev = nullptr;
        next = nullptr;
    }
    

};



template <int... Ints>
class Polygon{

private:
    using Num  = Number<Ints...>;
    using Nod  = Node<Ints...>;
    using Poin = Point<Ints... >;
    using Ang  = Angle<Ints... >;

public:
    // std::list<Nod> nodes;
    LL_Node<Nod> *head;
    unsigned size_ll;
    

    Polygon(std::vector<std::vector<int>> points){
        // Turn the list of points into nodes, set them as the vertices of 
        // the polygon and calculate the angles between them


        std::vector<int> last_point = points.back();
        int prev_x{last_point.at(0)}, prev_y{last_point.at(1)};
        LL_Node<Nod> *current;

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
    }

    Polygon(Polygon const& other_poly){

        LL_Node<Nod> *node, *other_node;
        other_node = other_poly.head->next;

        head = new LL_Node<Nod>(other_poly.head->data);
        node = head;
        

        for(unsigned i=1; i<other_poly.size_ll; i++){
            node->next = new LL_Node<Nod>(other_node->data);
            node->next->prev = node;

            other_node = other_node->next;
            node = node->next;

        }
        size_ll = other_poly.size_ll;

    }

    ~Polygon(){
        LL_Node<Nod> *current, *next;
        current = head;
        // std::cout << "started deleting polygon" << std::endl;
        
        for(unsigned i = 0; i < size_ll; i++){
            // std::cout << "attempting to delete " << std::endl;
            // current->data.print();
            next = current->next;
            
            delete current;
            current = next;
        }
        // std::cout << "deleted polygon" << std::endl;
    }

    void translate(Poin const& dr){
        LL_Node<Nod> *ll_node = head;
        for(unsigned i=0; i<size_ll; i++){
            ll_node->data->position += dr;
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


    void prune_LL(std::unordered_set<LL_Node<Nod>*> update){
        // Remove points from the LL which are coincident or have
        // an opening angle of 0. This is repeated until no more points
        // can be removed. It may happen that all points are removed, in
        // which case the LL head becomes a nullptr
        // std::cout << "inside prune" << std::endl;

        while(update.size()){
            // std::cout << "new iteration. size update, ll: " << update.size() << " " << size_ll << std::endl;

            if(size_ll == 1){
                // std::cout << "deleting head" << std::endl;
                delete head;
                head = nullptr;
                size_ll = 0;
                break;
            }

            // std::cout << "this is inside update:" << std::endl;
            // for(auto& ll_node: update){
            //     ll_node->data.print();
            // }

            // std::cout << "done" << std::endl;

            auto it = update.begin();
            LL_Node<Nod> *node = *it;
            update.erase(it);

            Nod& curr = node->data;
            // std::cout << "Considering node" << std::endl;
            // curr.print();

            LL_Node<Nod> *prev_node = node->prev;
            LL_Node<Nod> *next_node = node->next;
            Nod& prev = node->prev->data;
            Nod& next = node->next->data;

            // std::cout << "positions: curr, prev, next: " << std::endl;
            // curr.position.print();
            // prev.position.print();
            // next.position.print();

            bool modified = true;
            
            if(curr.position == prev.position){
                // std::cout << "current position is identical to previous" << std::endl;
                prev.angle_start = curr.angle_start;
                prev.update_opening();

            } else if(curr.position == next.position){
                // std::cout << "current position is identical to next" << std::endl;
                next.angle_end = curr.angle_end;
                next.update_opening();


            } else if(curr.angle_opening.is_zero()){
                // std::cout << "opening angle is zero" << std::endl;

                // Check which neighbour is closest
                Poin vec_prev = curr.position - prev.position;
                Num dist_prev2 = vec_prev.get_x()*vec_prev.get_x() + vec_prev.get_y()*vec_prev.get_y();

                Poin vec_next = curr.position - next.position;
                Num dist_next2 = vec_next.get_x()*vec_next.get_x() + vec_next.get_y()*vec_next.get_y();

                if(dist_prev2 < dist_next2){
                    // std::cout << "prev is closer than next" << std::endl;
                    prev.angle_start = -prev.angle_start;
                    prev.update_opening();

                } else {
                    // std::cout << "next is closer than prev" << std::endl;
                    next.angle_end = -next.angle_end;
                    next.update_opening();
                }

            } else {
                // std::cout << "Nothing to do with this node" << std::endl;
                modified = false;
            }

            if(modified){

                next_node->prev = prev_node;
                prev_node->next = next_node;
                if(node == head){
                    // std::cout << "node being erased is head" << std::endl;
                    head = node->next;
                }
                
                // std::cout << "deleting" << std::endl;
                // node->data.print();
                delete node;
                size_ll--;
                update.insert(next_node);
                update.insert(prev_node);
            }
            
            // std::cout << "polygon after this iteration of prune:" << std::endl;
            // this->print();
        }

        // std::cout << "finished prune" << std::endl;
        
    }

    bool operator==(Polygon const& other_poly) const{
        LL_Node<Nod> *list1{this->head}, *list2{other_poly.head};
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

};

template <int... Ints>
std::ostream& operator<<(std::ostream& stream, Polygon<Ints...> const& poly){
    LL_Node<Node<Ints...>> *ll_node = poly.head;
    
    for(unsigned i = 0; i < poly.size_ll; i++){
        stream << ll_node->data << "\n";
        ll_node = ll_node->next;
    }

    return stream;
}

template <int... Ints>
std::ostream & operator<<(std::ostream& stream, Node<Ints...> const& node){
    stream <<    "( " << node.position << ", " 
                      << node.angle_start << ", "
                      << node.angle_end << ")";
    return stream;
}

#endif // POLYGON_C