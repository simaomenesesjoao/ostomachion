#pragma once

#include "polygon.cpp"
#include <functional>
#include <vector>

template <typename Poly>
class PolySet {
private:
    unsigned int size_;
    unsigned long long hash_;

public:
    virtual void insert(unsigned int i, const Poly& poly) = 0;

    unsigned int get_size(){
        return size_;
    }
    
    virtual bool operator==(const PolySet& other) = 0;
    virtual bool calculate_hash() = 0;

    bool get_hash(){
        return hash_;
    };

};

template <typename Poly>
class Ostomachion: public PolySet<Poly> {

};



template <typename Poly>
class Ostomini: public PolySet<Poly> {
private:
    static std::vector<std::pair<Poly, unsigned int>> polyset;
    static Poly frame;
    std::vector<std::vector<Poly>> polygons;

    bool equals(const PolySet& other, std::function<Poly(const Poly&)> transform){
        // Compare the positions of each set of polygons. Returns true if all
        // of them are identical. Allows a transformation to be applied to the 
        // first set, so that two sets can be identical if they differ by 
        // rotation or reflection

        if(this->size_ != other.size_)
            return false;

        for(unsigned int i = 0; i < num_distinct_polys; i++){
            auto& type = polygons.at(i);
            auto& other_type = other.polygons.at(i);

            if(type.size() != other_type.size())
                return false;

            for(auto& poly: type){
                bool found = false;
                auto transformed = transform(poly);
                for(auto& other_poly: other_type){
                    if(transformed == other_poly){
                        found = true;
                        break;
                    }
                }

                if(not found)
                    return false;
            }
        }
        return true;
    }

    
    Poly identity(const Poly& poly){
        return poly;
    };

    Poly rotation90(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({1,0},{6,6});
        return poly2;
    };

    // auto rotation180 = [](const Poly& poly){
    //     Poly poly2 = poly;
    //     poly2.rotate({0,-1},{6,6});
    //     return poly2;
    // };

    // auto rotation270 = [](const Poly& poly){
    //     Poly poly2 = poly;
    //     poly2.rotate({-1,0},{6,6});
    //     return poly2;
    // };

    // auto flip_x = [](const Poly& poly){
    //     Poly poly2 = poly;
    //     poly2.flip_x();
    //     poly2.translate({4,0});
    //     return poly2;
    // };

    // auto flip_y = [](const Poly& poly){
    //     Poly poly2 = poly;
    //     poly2.flip_y();
    //     poly2.translate({0,4});
    //     return poly2;
    // };


    std::vector<std::function<Poly(const Poly&)>> transformations{identity, rotation90};

public:
    static unsigned int max_size;
    static unsigned int num_distinct_polys;

    

    bool operator==(const PolySet& other){




        return equals(other, identity) or equals(other, rotation180);

    }

    unsigned long long get_hash(){
        // Calculates a hash that is invariant under rotation and reflection
        long long h = 0;
        for(auto& polyrow: polyset){

        }
    }
};


template <typename Poly> 
Poly Ostomini<Poly>::frame{{0,0}, {0,4}, {4,4}, {4,0}};

template <typename Poly> 
std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly>::polyset{
    {{{0,0}, {2,4}, {0,4}}, 2}, 
    {{{0,0}, {1,0}, {1,2}}, 2},
    {{{1,0}, {3,0}, {3,2}, {3,1}}, 1},
    {{{1,2}, {3,2}, {2,4}}}, 1};

template <typename Poly>
unsigned int Ostomini<Poly>::max_size = 6;
template <typename Poly>
unsigned int Ostomini<Poly>::num_distinct_polys = 4;




template <typename Num, bool enable_hash = true, bool enable_comparison = true>
class InnerState{
    using Poi = Point<Num>;
    using Nod = Node<Num>;
public:
    InnerState():
        state_{std::vector<std::vector<Poi>>(polygons<Num>::num_polygons)}, 
        size_{0}{
            calculate_hash();
        }

    bool is_set(unsigned i){
        return state_.at(i).size() != 0;
    }

    void set_poly(unsigned i, const Polygon<Num> & polygon){

        LL_Node<Nod> *poly_node = polygon.head;
        for(unsigned k = 0; k < polygon.size_ll; k++){
            state_.at(i).push_back(poly_node->data.position);
            poly_node = poly_node->next;
        }
        calculate_hash();
        size_++;
    }

    void calculate_hash(){
        if(!enable_hash){
            hash_ = 0;
            return;
        }

        long h = 0;
        for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
            auto& poly = state_.at(i);
            for(auto& point: poly){
                double x = (double)point.get_x();
                double y = (double)point.get_y();
                h += (long)(x*94833373);
                h = h%100003;
                h += (long)(y*94373);
                h = h%100003;
            }
        }
        hash_ = h;
    }

    std::size_t get_hash() const {
        return hash_;
    }
    
    unsigned size() const {
        return size_;
    }

    unsigned max_size() const {
        return state_.size();
    }

    std::vector<Poi> at(unsigned i) const{
        return state_.at(i);
    }



    template <typename T>
    bool are_polys_same(const T& other) const {
        if(size_ != other.size())
            return false;

        for(unsigned i=0; i<max_size(); i++){
            if(i == 5 or i == 6 or i == 7 or i == 8){
                continue;
            }

            if(at(i) != other.at(i))
                return false;
            
        }

        auto a_list = std::vector<unsigned>{5, 6}; 
        auto b_list = std::vector<unsigned>{8, 7};

        for(unsigned k=0; k<2; k++){
            unsigned a = a_list.at(k);
            unsigned b = b_list.at(k);
            bool b1 = (at(a) == other.at(a));
            bool b2 = (at(b) == other.at(b));
            bool b3 = (at(b) == other.at(a));
            bool b4 = (at(a) == other.at(b));

            if(!(b1 and b2) and !(b3 and b4))
                return false;
        }


        return true;
    }

    bool operator==(const InnerState& other) const {
        if(!enable_comparison)
            return false;
        return are_polys_same(other);
    }
    
    std::vector<std::vector<std::vector<double>>> used_polys_as_vector() const {
        std::vector<std::vector<std::vector<double>>> vector;
        for(auto& poly: state_){
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

private:
    std::vector<std::vector<Poi>> state_;
    unsigned size_;
    std::size_t hash_;
};


template <typename Num, bool a, bool b>
std::ostream& operator<<(std::ostream& stream, const InnerState<Num, a, b> & used_polys){

    for(unsigned i=0; i<used_polys.max_size(); i++){
        stream << "[";
        for(auto point: used_polys.at(i)){
            double x = (double)point.get_x();
            double y = (double)point.get_y();
            stream << "(" << x << "," << y << ") ";
        }
        stream << " ]";
    }

    stream << std::endl;
    return stream;    
}
