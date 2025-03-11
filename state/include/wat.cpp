void prune_LL(std::unordered_set<LL_Node<Nod>*> update, std::ostream& stream = std::cout){
    // Remove points from the LL which are coincident or have
    // an opening angle of 0. This is repeated until no more points
    // can be removed. It may happen that all points are removed, in
    // which case the LL head becomes a nullptr

    while(update.size()){
        // stream << "oooooooooooooo inside prune with frame \n " << *this << "\n";
        
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
        
            next.update_opening();

        if(modified){
            // stream << "was modified\n";

            next_node->prev = prev_node;
            prev_node->next = next_node;
            if(node == head){
                // stream << "node being erased is head" << std::endl;
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

