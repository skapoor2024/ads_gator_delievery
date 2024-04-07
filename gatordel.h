//  delivery header
#ifndef GATORDEL_H
#define GATORDEL_H

#include "order.h"
#include "node.h"
#include "avltree.h"
#include<unordered_map>
#include<functional>
#include<utility>

class GatorDel{

    private:

    std::function<bool(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&)> compare_priority;
    std::function<bool(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&)> compare_eta;
    AvlTree priority_tree;
    AvlTree eta_tree;
    std::unordered_map<int, std::shared_ptr<Order>> umap;
    int normalized_const;
    float value_wt;
    float time_wt;
    int driver_time_base;

    std::vector<std::vector<int>>get_orders_before_time(int current_system_time);
    std::shared_ptr<Order> get_order_just_after_priority(float priority);
    //int get_eta(int current_system_time, int delivery_time, float priority);

    public:

    GatorDel();
    void print(int order_id);
    void print(int time1, int time2);
    void get_rank_of_order(int order_id);
    void create_order(int order_id, int current_system_time,int order_value,int delivery_time);
    void cancel_order(int order_id,int current_system_time);
    void update_time(int order_id,int current_system_time,int new_delivery_time);
    void quit();
};


#endif