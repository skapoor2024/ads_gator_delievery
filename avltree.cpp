#include "avltree.h"
#include<algorithm>
#include<iostream>

//private functions

int AvlTree::get_height(Node* node){
    if(node == nullptr) return 0;
        return node->height;
}

int AvlTree::update_height(Node* node){
    return std::max(get_height(node->left),get_height(node->right))+1;
}

int AvlTree::get_balance_factor(Node* node){
    return  get_height(node->left) - get_height(node->right);
}

Node* AvlTree::rotate_left(Node* x){
    Node* y = x->right;
    Node* temp = y->left;

    //perform rotation
    y->left = x;
    x->right = temp;

    //update height
    y->height = update_height(y);
    x->height = update_height(x);

    return y;
}

Node* AvlTree::rotate_right(Node* y){
    Node* x = y->left;
    Node* temp = x->right;

    //perform rotation
    x->right = y;
    y->left = temp;
    
    //update height
    y->height = update_height(y);
    x->height = update_height(x);

    return x;
}

Node* AvlTree::balance(Node* node){

    if(node == nullptr) return node;

    //update height to make sure that after rotations height are balanced if any
    node->height = update_height(node);

    int balance_factor = get_balance_factor(node);

    //left left case

    if(balance_factor>1 && get_balance_factor(node->left)>=0){
        return rotate_right(node);
    }

    //left right case

    else if(balance_factor>1 && get_balance_factor(node->left)<0){
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    //right right case

    else if(balance_factor<-1 && get_balance_factor(node->right)<=0){
        return rotate_left(node);
    }

    else if(balance_factor<-1 && get_balance_factor(node->right)>0){
        node->right = rotate_right(node->right);
        rotate_left(node);
    }

    return node;
}

Node* AvlTree::insert_node(Node* node,const std::shared_ptr<Order>& new_data){
    if(node == nullptr){
        return new Node(new_data);
    }

    if (compare(new_data,node->data)){
        node->left = insert_node(node->left,new_data);
    }
    else if(compare(node->data,new_data)){
        node->right = insert_node(node->right,new_data);
    }

    node->height = update_height(node);
    return balance(node);

}

// Utility function to find the node with minimum key value found in that tree
Node* AvlTree::min_value_node(Node* node) {
    Node* current = node;

    // Loop down to find the leftmost leaf
    while (current && current->left != nullptr){
        current = current->left;
    }   
        

    return current;
}

Node* AvlTree::max_value_node(Node* node){
    Node* current = node;
    while(current && current->right!=nullptr){
        current = current->right;
    }
    return current;
}

Node* AvlTree::find_successor(Node* root,Node* node){
    // if right child exit return the min key value of the right subtree
    if(node->right!=nullptr) return min_value_node(node->right);

    // Otherwise, traverse up using the parent pointer until you see a node which is the left child of its parent. 
    // The parent of such a node is the successor.
    Node* succ = nullptr;
    Node* current = root;

    while(current!=nullptr){
        if(compare(node->data,current->data)){
            succ = current;
            current = current->left;
        }
        else if(compare(current->data,node->data)){
            current = current->right;
        }
        else {
            break;
        }
    }
    return succ;
}

Node* AvlTree::find_predecessor(Node* root, Node* node) {
    // If the left child exists, return the max value node of the left subtree
    if (node->left != nullptr) return max_value_node(node->left);

    // Otherwise, traverse up using the parent pointer until you see a node
    // which is the right child of its parent. The parent of such a node is the predecessor.
    Node* pred = nullptr;
    Node* current = root;

    while (current != nullptr) {
        if (compare(current->data, node->data)) {
            pred = current;
            current = current->right;
        } else if (compare(node->data, current->data)) {
            current = current->left;
        } else {
            break; // Found the node, so break.
        }
    }
    return pred;
}

Node* AvlTree::delete_node(Node* node,const std::shared_ptr<Order>& old_data){
    if(node == nullptr) return node;

    //if the key is smaller than node then left subtree
    if(compare(old_data,node->data)){
        node->left = delete_node(node->left,old_data);
    }

    // if the key is greater than the node then right subtree
    else if (compare(node->data,old_data)){
        node->right = delete_node(node->right,old_data);
    }

    //if node found
    else if(node->data == old_data){

        //if node with one children or zero children
        if(node->left == NULL || node->right == NULL){

            Node* temp = node->left?node->left:node->right;

            //zero children case
            if(temp == nullptr){
                temp = node;
                node = nullptr;
            }
            else{
                // we directly adopt the child.
                *node = *temp;
            }

            delete temp;

        }
        // if both children are present
        else{

            //get next inorder successor node
            Node* temp = min_value_node(node->left);

            //copy the contents of the node
            node->data = temp->data;

            //delete the successor node
            node->right = delete_node(node->right,temp->data);

        }
    }

    //case if tree had only one node
    if(node == nullptr) return node;

    node->height = update_height(node);

    return balance(node);
}

Node* AvlTree::search_node(Node* node,std::shared_ptr<Order>& old_data){
    if(node == nullptr) return node;

    if(compare(old_data,node->data)){
        return search_node(node->left,old_data);
    }
    else if(compare(node->data,old_data)){
        return search_node(node->right,old_data);
    }
    return node; //if equal you return;
}

//public functions

AvlTree::AvlTree(std::function<bool(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&)> comp)
    :root(nullptr),compare(comp){}

// void AvlTree::clear(Node* node){
//     while(node!=nullptr){
//         clear(node->left);
//         clear(node->right);
//         delete node;
//     }
// }

AvlTree::~AvlTree(){
    //clear(root);
    delete root;
}

void AvlTree::insertNode(const std::shared_ptr<Order>& new_data){
    root = insert_node(root,new_data);
}

void AvlTree::deleteNode(const std::shared_ptr<Order>& old_data){
    root = delete_node(root,old_data);
}

Node* AvlTree::get_root(){
    return this->root;
}

void AvlTree::order_bt12(Node* node, std::shared_ptr<Order>& o1, std::shared_ptr<Order>& o2,std::vector<int>& res){
    if(node == nullptr) return;

    if(compare(o1,node->data)){
        order_bt12(node->left,o1,o2,res);
    }

    if(!compare(node->data,o1) && !compare(o2,node->data)){
        res.push_back(node->data->order_id);
    }

    if(compare(node->data,o2)){
        order_bt12(node->right,o1,o2,res);
    }
}

int AvlTree::node_depth(Node* node,const std::shared_ptr<Order>& ord){


    if(node == nullptr){
        return -1;
    }

    //using the equality operator of the order.
    if(*node->data == *ord){
        return 0;
    }

    // if the value of key of ord is less than node
    else if(compare(ord,node->data)){
        
        int left_depth = node_depth(node->left,ord);
        return left_depth==-1?-1:1+left_depth;
    }
    // if the value of key of node is less than ord.
    else if(compare(node->data,ord)){
        int right_depth = node_depth(node->right,ord);
        return right_depth==-1?-1:1+right_depth;
    }

    return -1;
}

// Node* AvlTree::delete_nodes_below(Node* node,const Order& ord,std::vector<std::vector<int>>& res){

//     if(node == nullptr) return node;

//     node->left = delete_nodes_below(node->left,ord,res);
//     //storing values inorder(from low to )
//     if(compare(node->data,ord)){
//         res.push_back(std::vector<int>(node->data.order_id,node->data.eta));
//     }
//     node->right = delete_nodes_below(node->right,ord,res);
//     //deleting post order.
//     if(compare(node->data,ord)){
        
//         Node* temp = delete_node(node,node->data); //here balance will be taken care of.

//         return temp;
//     }

//     return balance(node);
    
// }

//inorder traversal to find the less than key orders.
void AvlTree::get_nodes_below(Node* node,const std::shared_ptr<Order>& ord,std::vector<std::vector<int>>& res,int& time_stamp){

    if(node == nullptr) return;

    get_nodes_below(node->left,ord,res,time_stamp);
    //storing values inorder(from low to high)
    if(node->data->eta <= ord->current_system_time){
        node->data->delivered=1;
        res.push_back(std::vector<int>{node->data->order_id,node->data->eta});
        //time_stamp = std::max(std::max(node->data->eta + node->data->delivered,ord->current_system_time-1),time_stamp);
        time_stamp = std::max(node->data->eta + node->data->delivery_time,time_stamp);
    }
    else if((node->data->eta - node->data->delivery_time)<=ord->current_system_time){
        node->data->delivered = 0; //getting delivered.
        //std::cout<<"The ETA of item being deliveres is "<<node->data->eta<<std::endl;
        time_stamp = std::max(node->data->eta + node->data->delivery_time,time_stamp);
    }
    get_nodes_below(node->right,ord,res,time_stamp);
    
}

std::shared_ptr<Order> AvlTree::get_order_just_after_key(Node* node, const std::shared_ptr<Order>& key, std::shared_ptr<Order>& afterkey) {
    if (node == nullptr) return afterkey;

    if (compare(key, node->data)) {
        afterkey = node->data; // Here, afterkey and node->data are both std::shared_ptr<Order>
        return get_order_just_after_key(node->right, key, afterkey); // Traverse right for just after
    }
    else if (compare(node->data, key)) {
        return get_order_just_after_key(node->left, key, afterkey); // Traverse left if node data is less than key
    }

    return afterkey;
}

void AvlTree::get_nodes_below(Node* node,const std::shared_ptr<Order>& ord,std::vector<int>& res){
    if(node == nullptr) return;
    get_nodes_below(node->left,ord,res);
    if(compare(node->data,ord)){
        res.push_back(node->data->order_id);
    }
    get_nodes_below(node->right,ord,res);
}

//update eta

std::vector<int> AvlTree::update_eta(std::shared_ptr<Order>& old_data,int time_stamp_base){

    Node* node = search_node(this->root,old_data);
    //node found
    if(node!=nullptr){
        int eta1{-1};
        //Node* node_predecessor = find_predecessor(this->root,node);
        Node* node_successor = find_successor(this->root,node);

        // if successor exits
        if(node_successor){
            // if the order of higher priority exits then it should take place after.
            //since its yet to be delivered or being delivered.
            //std::cout<<"The succesor is  "<<node_successor->data->order_id<<std::endl;
            eta1 = node_successor->data->eta + node_successor->data->delivery_time + node->data->delivery_time;
        }
        else{
            //if it's the highest then the eta is time_stamp_base + delivery time
            // This will take care if predecessor exisit of lower prioirty and is being delivered or
            // the case where the higher order has been delivered but hasnt arrived back yet.
            // also the case if the current system time is the largest.
            //std::cout<<" It has no succesor so delivered after the last delivery item"<<std::endl;
            eta1 = time_stamp_base + node->data->delivery_time;
        }

        //update eta of the node;
        node->data->eta = eta1;
    }
    std::vector<int> res{};

    get_nodes_below(this->root,node->data,res);
    res.push_back(node->data->order_id);// adding orderID of the current node.

    return res;
    //update eta of the rest of the nodes that occur before it. start from predecessor. //also check if in the process of delivering if 0
}

void AvlTree::inorder(Node* node){
    if(node == nullptr) return;
    inorder(node->left);
    std::cout<<"Order iD "<<node->data->order_id<<
                " Priority "<<node->data->priority<<
                " ETA "<<node->data->eta<<std::endl;
    inorder(node->right);
}

void AvlTree::get_nodes_below(Node* node,std::shared_ptr<Order>& ord,int& cnt){
    if(node == nullptr) return;
    get_nodes_below(node->left,ord,cnt);
    if(compare(node->data,ord)){
        cnt++;
    }
    get_nodes_below(node->right,ord,cnt);
}

void AvlTree::get_nodes_after(Node* node,const std::shared_ptr<Order>& ord,std::vector<int>& res){
    if(node == nullptr) return;
    get_nodes_after(node->left,ord,res);
    if(compare(ord,node->data)){
        res.push_back(node->data->order_id);
    }
    get_nodes_after(node->right,ord,res);
}

Node* AvlTree::get_predecessor(std::shared_ptr<Order>& ord){
    Node* node = search_node(this->root,ord);
    return find_predecessor(this->root,node);
}
