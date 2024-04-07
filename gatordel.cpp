//gatordel.cpp

#include "gatordel.h"
#include <iostream>
#include<algorithm>

//definitng private functions

std::vector<std::vector<int>> GatorDel::get_orders_before_time(int current_system_time){

    //create a temporary order with given current system time
    auto temp = std::make_shared<Order>();
    temp->current_system_time = current_system_time;
    // adding one cause comparator function returns true when the value is less not equal.
    //creating res
    std::vector<std::vector<int>> res{};
    eta_tree.get_nodes_below(eta_tree.get_root(),temp,res,this->driver_time_base);
    this->driver_time_base = std::max(this->driver_time_base,current_system_time);
    return res;
}

std::shared_ptr<Order> GatorDel::get_order_just_after_priority(float priority){

    auto temp = std::make_shared<Order>(Order());
    temp->priority = priority;
    auto temp2 = std::make_shared<Order>(Order());

    return priority_tree.get_order_just_after_key(priority_tree.get_root(),temp,temp2);
}


// int GatorDel::get_eta(int current_system_time,int delivery_time,float priority){
//     int res = 0;
//     auto just_after = get_order_just_after_priority(priority);

//     if (!just_after || just_after->priority == -1) { // priority is -1
//         res = current_system_time + delivery_time;
//     } 
//     else { // Case where there is an order of priority before it.
//         res = std::max(just_after->eta + just_after->delivery_time, current_system_time) + delivery_time;
//     }
//     return res;
// }


// public functions

GatorDel::GatorDel()
    : compare_priority([](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b){
        if(a->priority == b->priority){
            return a->eta > b->eta;
        }
        return a->priority < b->priority;
    }),
    compare_eta([](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b){
        return a->eta < b->eta;
    }),
    priority_tree(compare_priority), eta_tree(compare_eta), normalized_const(50), value_wt(0.3), time_wt(0.7),driver_time_base(0) {}

void GatorDel::print(int order_id) {
    auto it = umap.find(order_id);
    if(it != umap.end()) {
        auto& gg = it->second; 
        std::cout << "[" << gg->order_id << ", " << gg->current_system_time << ", " 
                  << gg->order_value << ", " << gg->delivery_time << ". " << gg->eta << "]" << std::endl;
    } else {
        std::cout << "No such orderID exists" << std::endl;
    }
}

void GatorDel::print(int time1,int time2){
    std::vector<int> res{};
    auto o1 = std::make_shared<Order>(Order());
    o1->eta = time1;
    auto o2 = std::make_shared<Order>(Order());
    o2->eta = time2;
    eta_tree.order_bt12(eta_tree.get_root(),o1,o2,res);
    if(res.size() == 0){
        std::cout<<"There are no orders in that time period"<<std::endl;
    }
    else{
        std::cout<<"[";
        int sz = res.size();
        for(int i{};i<sz-1;++i){
            std::cout<<res.at(i)<<", ";
        }
        std::cout<<res.at(sz-1)<<" ]";
    }
    std::cout<<std::endl;
}

//should have been post order traversal of tree to find 
// void GatorDel::get_rank_of_order(int order_id) {
//     auto it = umap.find(order_id);
//     if(it != umap.end()){
//         // Access the shared_ptr<Order> directly
//         auto ord = it->second;
//         int num_jumps = eta_tree.node_depth(eta_tree.get_root(), ord);
//         std::cout << "Order " << order_id << " will be delivered after " << num_jumps << " orders" << std::endl;
//     } else {
//         std::cout << "No such order found" << std::endl;
//     }
// }

//use eta tree to find.
void GatorDel::get_rank_of_order(int order_id){

    auto it = umap.find(order_id);
    int cnt{};
    if(it!=umap.end()){
       eta_tree.get_nodes_below(eta_tree.get_root(),umap[order_id],cnt);
    }
    std::cout << "Order " << order_id << " will be delivered after " << cnt << " orders" << std::endl;
}

void GatorDel::create_order(int order_id,int current_system_time,int order_value,int delivery_time){

    std::cout<<std::endl;

    float normalized_order_value = float(order_value)/normalized_const;
    float new_priority = value_wt*normalized_order_value - time_wt*current_system_time;
    //std::cout<<" The priority of this order is "<<new_priority<<std::endl;

    // get orders before current system time and get drivers timestamp when it will next reach base.

    std::vector<std::vector<int>> orders_before_systime = get_orders_before_time(current_system_time);
    //  the driver time stamp base is either updated to current system time or the time it will  get back after delivery.
    
    //std::cout<<"current system time is "<<current_system_time<<" Time to reach base "<<this->driver_time_base<<std::endl;

    //delete the orders from all the hashmap prioirity tree and eta tree.
    for(std::vector<int> vec:orders_before_systime){
        int order_id = vec.at(0);
        auto it = umap.find(order_id);
        if(it!=umap.end()){
            auto ord = it->second;
            eta_tree.deleteNode(ord);
            priority_tree.deleteNode(ord);
            umap.erase(order_id);
        }
       
    }

    auto ord = std::make_shared<Order>(order_id,normalized_order_value,delivery_time,2147483647,current_system_time,-1,new_priority);

    //insert into priority tree and find eta
    priority_tree.insertNode(ord);

    //print inorder of the tree, to see how it goes.
    //priority_tree.inorder(priority_tree.get_root());
    
    //get nodes below this priority
    std::vector<int> nodes_below_priority = priority_tree.update_eta(ord,this->driver_time_base);


    //insert new node into hashmap
    umap[order_id] = ord;

    //insert new node into avl tree
    eta_tree.insertNode(ord);

    //print
    std::cout<<"Order "<<order_id<<" has been created - ETA: "<<umap[order_id]->eta<<std::endl;

    //update the nodes using hashmap and get updated ETA's
    int sz = nodes_below_priority.size();
    std::vector<std::vector<int>> updated_etas{};
    for(int i{sz-2};i>=0;i--){
        int o1 = nodes_below_priority.at(i);
        if(umap[o1]->delivered == -1){
            int o2 = nodes_below_priority.at(i+1);
            umap[o1]->eta = umap[o2]->eta + umap[o2]->delivery_time + umap[o1]->delivery_time;
            updated_etas.push_back(std::vector<int>{umap[o1]->order_id,umap[o1]->eta});
        }   
    }

    //print updated etas

    if(updated_etas.size()>0){
        std::cout<<"Updated ETAs: [";
        for(int i{};i<updated_etas.size();i++){
            std::cout<<updated_etas.at(i).at(0)<<":"<<updated_etas.at(i).at(1);
            if(i!=updated_etas.size()-1){
                std::cout<<",";
            }
        }
        std::cout<<"]"<<std::endl;
    }

    // print orders that have been delivered before system time.

    for(std::vector<int> vec:orders_before_systime){
        std::cout<<"Order "<<vec.at(0)<<" has been delivered at time "<<vec.at(1)<<std::endl;
    }

}

void GatorDel::cancel_order(int order_id,int current_system_time){

    //first find the orders that are delivered before the current system time

    // get orders before current system time and get drivers timestamp when it will next reach base.

    std::vector<std::vector<int>> orders_before_systime = get_orders_before_time(current_system_time);
    //  the driver time stamp base is either updated to current system time or the time it will  get back after delivery.
    
    //std::cout<<"current system time is "<<current_system_time<<" Time to reach base "<<this->driver_time_base<<std::endl;

    //delete the orders from all the hashmap prioirity tree and eta tree.
    for(std::vector<int> vec:orders_before_systime){
        int order_id = vec.at(0);
        auto it = umap.find(order_id);
        if(it!=umap.end()){
            auto ord = it->second;
            eta_tree.deleteNode(ord);
            priority_tree.deleteNode(ord);
            umap.erase(order_id);
        }
       
    }

    // if delivered then can't be cancelled
    auto it = umap.find(order_id);
    if(it!=umap.end()){
        //check if delivered or not
        auto ord = it->second;
        //if in the delivery process left the station
        if(ord->delivered !=-1){
            std::cout<<"Order cant be cancelled\nOn its way"<<std::endl;
        }
        else{

            //why not update it using eta

            //find the etas of tree greater than the given order id
            std::vector<int> greater_eta;
            eta_tree.get_nodes_after(eta_tree.get_root(),ord,greater_eta);

            //need to check if the predecessor exits. if it exists then use it and if not then drivers time.
            Node* node_pred = eta_tree.get_predecessor(ord);
            //if predecessor exists;
            std::shared_ptr<Order> it;
            if(node_pred){
                it = node_pred->data;
            }
            else{ // the case where the delivery guy is getting back from the order last delivered.
                it = std::make_shared<Order>();
                it->eta = this->driver_time_base;
                it->delivery_time = 0;
            }
            //update the etas of the trees;
            for(int i{};i<greater_eta.size();++i){
                if(i==0){
                    int o1 = greater_eta.at(i);
                    umap[o1]->eta = it->eta + it->delivery_time + umap[o1]->delivery_time;
                }
                else{
                    int o1 = greater_eta.at(i);
                    int o2 = greater_eta.at(i-1);
                    
                    umap[o1]->eta = umap[o2]->eta + umap[o2]->delivery_time + umap[o1]->delivery_time;
                }
            }

            //remove the node from all the data structures;
            auto temp = umap[order_id];//creating a temporary node object pointer.
            priority_tree.deleteNode(temp);
            eta_tree.deleteNode(temp);
            umap.erase(order_id);

            std::cout<<"Order "<<order_id<<" has been cancelled"<<std::endl;

            //print the updated etas;
            if(greater_eta.size()){
                std::cout<<"Updated ETAs : [";
                for(int i{};i<greater_eta.size();++i){
                    int o1 = greater_eta.at(i);
                    std::cout<<umap[o1]->order_id<<":"<<umap[o1]->eta;
                    if(i!=greater_eta.size()-1){
                        std::cout<<",";
                    }
                }
                std::cout<<"]"<<std::endl;
            }
        }

    }
    else{
        std::cout<<"Cannot Cancel. The Order ID "<<order_id<<" already delivered or doesnt exist"<<std::endl;
    }

    // print orders that have been delivered before system time.

    for(std::vector<int> vec:orders_before_systime){
        std::cout<<"Order "<<vec.at(0)<<" has been delivered at time "<<vec.at(1)<<std::endl;
    }

}

void GatorDel::update_time(int order_id,int current_system_time,int new_delivery_time){

    //first find the orders that are delivered before the current system time

    // get orders before current system time and get drivers timestamp when it will next reach base.

    std::vector<std::vector<int>> orders_before_systime = get_orders_before_time(current_system_time);
    //  the driver time stamp base is either updated to current system time or the time it will  get back after delivery.
    
    //std::cout<<"current system time is "<<current_system_time<<" Time to reach base "<<this->driver_time_base<<std::endl;

    //delete the orders from all the hashmap prioirity tree and eta tree.
    for(std::vector<int> vec:orders_before_systime){
        int order_id = vec.at(0);
        auto it = umap.find(order_id);
        if(it!=umap.end()){
            auto ord = it->second;
            eta_tree.deleteNode(ord);
            priority_tree.deleteNode(ord);
            umap.erase(order_id);
        }
       
    }

    //find it in eta update it and update the rest;
    auto it = umap.find(order_id);
    if(it!=umap.end()){
        auto ord = it->second;
        //can't update if in the process of delivery
        if(ord->delivered!=-1){
            std::cout<<"Already left the place can't update delivery time"<<std::endl;
        }
        else{
            //find the order_id greater than it in eta tree;
            std::vector<int> greater_eta{};
            greater_eta.push_back(ord->order_id);
            eta_tree.get_nodes_after(eta_tree.get_root(),ord,greater_eta);
            //update the etas of every node in greater_eta;
            for(int i{};i<greater_eta.size();++i){
                if(i==0){
                    int o1 = greater_eta.at(i);
                    umap[o1]->eta = umap[o1]->eta - umap[o1]->delivery_time + new_delivery_time;
                    umap[o1]->delivery_time = new_delivery_time;
                }
                else{
                    int o1 = greater_eta.at(i);
                    int o2 = greater_eta.at(i-1);
                    umap[o1]->eta = umap[o2]->eta + umap[o2]->delivery_time + umap[o1]->delivery_time;
                }
            }
            //print the updated eta's

            //print the updated etas;
            if(greater_eta.size()){
                std::cout<<"Updated ETAs : [";
                for(int i{};i<greater_eta.size();++i){
                    int o1 = greater_eta.at(i);
                    std::cout<<umap[o1]->order_id<<":"<<umap[o1]->eta;
                    if(i!=greater_eta.size()-1){
                        std::cout<<",";
                    }
                }
                std::cout<<"]"<<std::endl;
            }

        }
        
    }
    else{
        std::cout<<"The Order "<<order_id<<" is already delivered or doesnt exist hence can't update"<<std::endl;
    }

    // print orders that have been delivered before system time.

    for(std::vector<int> vec:orders_before_systime){
        std::cout<<"Order "<<vec.at(0)<<" has been delivered at time "<<vec.at(1)<<std::endl;
    }

}

void GatorDel::quit(){
    std::vector<std::vector<int>> orders_before_systime = get_orders_before_time(2147483647);
    for(std::vector<int> vec:orders_before_systime){
        int order_id = vec.at(0);
        auto to_delete = umap[order_id];
        eta_tree.deleteNode(to_delete);
        priority_tree.deleteNode(to_delete);
        umap.erase(order_id);
        std::cout<<"Order "<<vec.at(0)<<" has been delivered at time "<<vec.at(1)<<std::endl;
    }
}