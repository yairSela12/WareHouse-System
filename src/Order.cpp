

#include <iostream>
#include "Order.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance),
      status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order:: getId() const{
    return id;
}

int Order:: getCustomerId() const{
    return customerId;
}

void Order:: setStatus(OrderStatus status){
    this -> status = status; 
}

void Order:: setCollectorId (int collectorId){
    this -> collectorId = collectorId;
}

void Order:: setDriverId (int driverId){
    this -> driverId = driverId;
}

 int Order:: getCollectorId() const{
    return collectorId;
}

int Order:: getDistance() const{
    return distance;
}

int Order:: getDriverId() const{
    return driverId;
}

OrderStatus Order:: getStatus() const{
    return status;
}

const string Order::toString() const {
    string statusStr;
    switch (status) {
        case OrderStatus::PENDING:
            statusStr = "Pending";
            break;
        case OrderStatus::COLLECTING:
            statusStr = "Collecting";
            break;
        case OrderStatus::DELIVERING:
            statusStr = "Delivering";
            break;
        case OrderStatus::COMPLETED:
            statusStr = "Completed";
            break;
    }

    return "Order ID: " + std::to_string(id) + "\n"
          +"orderStatus: " + statusStr + "\n"
          +"Customer ID: " + std::to_string(customerId) + "\n"
          +"Collector ID: " + (collectorId != NO_VOLUNTEER ? std::to_string(collectorId) : "None") + "\n"
          +"Driver ID: " + (driverId != NO_VOLUNTEER ? std::to_string(driverId) : "None");
}
string Order::orderStatusToString(OrderStatus status)  {
    switch (status) {
        case OrderStatus::PENDING:
            return "Pending";
        case OrderStatus::COLLECTING:
            return "Collecting";
        case OrderStatus::DELIVERING:
            return "Delivering";
        case OrderStatus::COMPLETED:
            return "Completed";
        default:
            return "Unknown";
    }
}