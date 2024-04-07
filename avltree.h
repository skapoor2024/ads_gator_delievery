#ifndef AVLTREE_H
#define AVLTREE_H

#include "order.h"
#include "node.h"
#include <functional>
#include <vector>
#include <memory> 

class AvlTree {
private:
    Node* root;
    std::function<bool(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&)> compare;

    // Private member functions
    int get_height(Node* node);
    int update_height(Node* node);
    int get_balance_factor(Node* node);

    Node* rotate_left(Node* x);
    Node* rotate_right(Node* y);
    Node* balance(Node* node);
    Node* insert_node(Node* node, const std::shared_ptr<Order>& new_data);
    Node* min_value_node(Node* node);
    Node* max_value_node(Node* node);
    Node* find_successor(Node* root,Node *n);
    Node* find_predecessor(Node* root,Node* n);
    Node* delete_node(Node* node, const std::shared_ptr<Order>& old_data);
    Node* search_node(Node* node,std::shared_ptr<Order>& old_data);

    void reverse_inorder_traversal(Node* node,std::shared_ptr<Order>& old_data,bool& start);
    
public:
    // Constructor
    AvlTree(std::function<bool(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&)> comp);

    // Destructor
    ~AvlTree();

    void insertNode(const std::shared_ptr<Order>& new_data);
    void deleteNode(const std::shared_ptr<Order>& old_data);

    Node* get_root();

    void order_bt12(Node* node,std::shared_ptr<Order>& o1, std::shared_ptr<Order>& o2, std::vector<int>& res);
    int node_depth(Node* node, const std::shared_ptr<Order>& ord);
    void get_nodes_below(Node* node, const std::shared_ptr<Order>& ord, std::vector<std::vector<int>>& res,int& time_stamp);
    std::shared_ptr<Order> get_order_just_after_key(Node* node, const std::shared_ptr<Order>& key, std::shared_ptr<Order>& afterkey);
    std::vector<int> update_eta(std::shared_ptr<Order>& old_data,int time_stamp_base);
    void get_nodes_below(Node* node,const std::shared_ptr<Order>& ord,std::vector<int>& res);
    void inorder(Node* node);
    void get_nodes_below(Node* node,std::shared_ptr<Order>& ord,int& cnt);
    void get_nodes_after(Node* node,const std::shared_ptr<Order>& ord,std::vector<int>& res);
    Node* get_predecessor(std::shared_ptr<Order>& ord);
};
#endif // AVLTREE_H
