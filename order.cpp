// Order.cpp
#include "order.h"

//defining constructor
Order::Order(int order_id, int order_value, int delivery_time, int eta, int current_system_time,int delivered,float priority)
    : order_id(order_id), order_value(order_value), delivery_time(delivery_time), eta(eta), current_system_time(current_system_time),delivered(delivered),priority(priority) {}

//defining copy constructor
Order::Order(const Order& other)
    :Order(other.order_id,other.order_value,other.delivery_time,other.eta, other.current_system_time,other.delivered,other.priority){}

//defining finding same order
bool Order::operator==(const Order& other){
    return (this->order_id == other.order_id) && (this->order_value == other.order_value) && (this->delivery_time == other.delivery_time)
            &&(this->eta == other.eta) &&(this->current_system_time == other.current_system_time); 
}