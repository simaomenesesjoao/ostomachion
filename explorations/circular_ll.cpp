#include <ostream>
#include <iostream>

template <typename T>
class Node{
private:
public:

    Node *next, *prev;
    T data;
    Node(T const& data):next{nullptr}, prev{nullptr}, data{data}{}
    Node(T const& data, const Node& prev):next{nullptr}, prev{prev}, data{data}{}
    Node* get_next() const{
        return next;
    }
};

template <typename T>
class CircularLL{
private:
    using Nod = Node<T>;    

    int size;
    
public:
    Nod *tail, *head;
    CircularLL():head{nullptr}, tail{nullptr},size{0}{}
    ~CircularLL(){

        if(tail)
            tail->next = nullptr;

        Nod *node, *next;
        node = head;
        next = nullptr;

        while(node){

            if(node->next){
                next = node->next;
            } else {
                next = nullptr;
            }
                

            delete node;

            node = next;
        }
    }
    
    Nod* get_head(){
        return head;
    }

    Nod* get_tail(){
        return tail;
    }

    int get_size(){
        return size;
    }


    void push_back(T const& new_element){
        Nod* node = new Node(new_element);

        if(!head){
            head = node;
            tail = node;
            head->next = node;
            head->prev = node;            
        } else{
            node->next = tail->next;
            node->prev = tail;
            node->prev->next = node;
            node->next->prev = node;
            tail = node;
        }

        size++;
    }

    void join(Nod *node, CircularLL && other_list, Nod *other_node){
        if(other_list.size == 0) return;

        Nod* temp = node->prev;

        node->prev = other_node->prev;
        other_node->prev = temp;
        node->prev->next = node;
        other_node->prev->next = other_node;
        
        tail = head->prev;
        size += other_list.size;

        // Delete the other list
        other_list.tail = nullptr;
        other_list.head = nullptr;
        other_list.size = 0;
    }

    void remove(Nod *& node){
        if(true){
            node->prev->next = node->next;
            node->next->prev = node->prev;
            delete node;
            size--;
            node = nullptr;
        }
        
    }


    friend std::ostream& operator<<(std::ostream & stream, CircularLL<T> const& list){
        if(list.size == 0) return stream << "Empty";

        Nod* current = list.head;
        do{
            stream << current->data << " -> ";
            current = current->next;
        } while(current != list.head);
        
        return stream;
    }

    void print_memory_locations(){
        if(size == 0) std::cout << "Empty";

        Nod* current = head;
        
        do{
            std::cout << current->prev << " -> " << current << " -> " << current->next << std::endl;
            current = current->next;
        } while(current != head);

        std::cout << "head: " << head << std::endl;
        std::cout << "tail: " << tail << std::endl;

    }

};


