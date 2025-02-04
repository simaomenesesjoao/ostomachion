#include "circular_ll.cpp"
#include <iostream>

int main(){
    CircularLL<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    std::cout << list << std::endl;
    // list.print_memory_locations();

    CircularLL<int> list2;
    list2.push_back(4);
    list2.push_back(5);
    list2.push_back(6);
    std::cout << list2 << std::endl;
    // list2.print_memory_locations();

    auto node1 = list.get_head()->get_next();
    auto node2 = list2.get_head()->get_next();

    list.join(node1, std::move(list2), node2);

    std::cout << list << std::endl;

    node1 = list.head->get_next()->get_next();
    list.remove(node1); // careful! the argument may not belong to the object
    // list.remove(node1);
    std::cout << list << std::endl;

    node1 = list.head;
    for(int i=0; i<20; i++){
        std::cout << node1->data;
        node1 = node1->get_next();
    }

    std::cout << std::endl;

    CircularLL<int> list3;
    list3.push_back(3);
    list3.push_back(4);
    list3.push_back(8);
    auto head = list3.get_head()->get_next();
    std::cout << "head: " << head->data << std::endl;

    // Iterate using do-while
    {
        auto node = head;
        do {
            if(!node) break;
            std::cout << node->data;
            node = node->get_next();
            
        } while(node != head);
    }

    // Iterate using information about the list's length
    {
        auto node = head;
        for(int i=0; i<list3.get_size(); i++){
            std::cout << node->data;
            node = node->get_next();
        }
    }

    
}