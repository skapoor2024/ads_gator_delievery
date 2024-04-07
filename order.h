// Order header
#ifndef ORDER_H
#define ORDER_H

class Order {
public:

    int order_id;
    int order_value;
    int delivery_time;
    int eta;
    int current_system_time;
    int delivered;

    float priority;

    Order(int order_id=-1, int order_value=-1, int delivery_time=-1, int eta=2147483647, int current_system_time = -1,int delivered = -1,float priority = -1.0);
    Order(const Order& other);
    ~Order() = default;
    bool operator==(const Order&);
};

#endif // ORDER_H
