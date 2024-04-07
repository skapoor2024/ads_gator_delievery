#include "gatordel.h"
#include<iostream>

int main(){

    GatorDel gator_del;

    gator_del.create_order(4001,1,200,3);
    gator_del.create_order(4002,3,250,6);
    gator_del.create_order(4003,8,100,3);
    gator_del.create_order(4004,13,100,5);
    gator_del.print(2,15);
    gator_del.get_rank_of_order(4003);
    gator_del.update_time(4003,15,2);
    gator_del.create_order(4005,17,150,4);
    gator_del.cancel_order(4002,20);
    gator_del.create_order(4006,22,300,3);
    gator_del.print(10,25);
    gator_del.create_order(4007,25,200,2);
    gator_del.create_order(4008,28,350,5);
    gator_del.print(20,30);
    gator_del.get_rank_of_order(4006);
    gator_del.create_order(4009, 32, 250, 3);
    gator_del.cancel_order(4004, 34);
    gator_del.update_time(4005, 37, 5);
    gator_del.create_order(4010, 40, 400, 6);
    gator_del.print(35, 45);
    gator_del.get_rank_of_order(4007);
    gator_del.create_order(4011, 40, 200, 4);
    gator_del.create_order(4012, 42, 300, 3);
    gator_del.print(50, 55);
    gator_del.update_time(4010, 55, 7);
    gator_del.cancel_order(4009, 56);
    gator_del.print(60, 90);
    gator_del.quit();

}