

#include <iostream>
#include "Volunteer.h"
#include <string>
#include <vector>
using std::string;
using std::vector;


Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) , id(id), name(name)  {}


int Volunteer:: getId() const{
    return id;
}

const string &Volunteer:: getName() const {
    return name;
} 

int Volunteer:: getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
}

CollectorVolunteer:: CollectorVolunteer(int id, const string &name, int coolDown):
 Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}  

 CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (decreaseCoolDown()) {
        completedOrderId = activeOrderId;   
        activeOrderId = NO_ORDER;
}}
 
int CollectorVolunteer:: getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer:: getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft--;
        return (timeLeft == 0);
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::  canTakeOrder(const Order &order) const  {
        return (!(isBusy()));
    }

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown; 
}

string CollectorVolunteer::toString() const {
     return         "Collector Volunteer - ID: " + std::to_string(getId()) + "\n" +
                    ", Name: " + getName() + "\n" +
                    ", Active Order ID: " + std::to_string(getActiveOrderId()) + "\n" +
                    ", Completed Order ID: " + std::to_string(getCompletedOrderId()) + "\n" +
                    ", Cool Down: " + std::to_string(getCoolDown()) + "\n" +
                    ", Time Left: " + std::to_string(getTimeLeft());

}
string CollectorVolunteer :: getRole() const {
    return "Collector";
}

LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id,const string &name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name , coolDown) , maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer:: hasOrdersLeft() const{
    return (ordersLeft > 0); 
}

bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const{
    return !(isBusy());
}

void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
        CollectorVolunteer:: acceptOrder(order);
        ordersLeft--;
}


int LimitedCollectorVolunteer:: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
         return     "LimitedCollectorVolunteer - ID: " + std::to_string(getId()) + "\n" +
                    ", Name: " + getName() + "\n" +
                    ", Active Order ID: " + std::to_string(getActiveOrderId()) + "\n" +
                    ", Completed Order ID: " + std::to_string(getCompletedOrderId()) + "\n" +
                    ", Cool Down: " + std::to_string(getCoolDown()) + "\n" +
                    ", Time Left: " + std::to_string(getTimeLeft()) + "\n" +
                    ", Max Orders: " + std::to_string(maxOrders) + "\n"
                    ", Orders Left: " + std::to_string(ordersLeft);
}
string LimitedCollectorVolunteer :: getRole() const {
    return "LimitedCollector";
}

DriverVolunteer :: DriverVolunteer (int id, const string &name, int maxDistance, int distancePerStep):
Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep) , distanceLeft(maxDistance){}

DriverVolunteer* DriverVolunteer:: clone() const{
    return new DriverVolunteer(*this);
}

int DriverVolunteer :: getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer :: getMaxDistance() const {
    return maxDistance;
} 

int DriverVolunteer :: getDistancePerStep() const{
    return distancePerStep; 
} 


bool DriverVolunteer :: decreaseDistanceLeft(){
    distanceLeft -= distancePerStep;
    return(distanceLeft <= 0); 
}

bool DriverVolunteer :: hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer :: canTakeOrder(const Order &order) const {
    return(!(isBusy()) && maxDistance >= order.getDistance()) ;
}

void DriverVolunteer :: acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
    if (isBusy()) {
       if (decreaseDistanceLeft()){
           completedOrderId = activeOrderId;
           activeOrderId = NO_ORDER;} 
}  }      

string DriverVolunteer:: toString() const {  
    return "Driver Volunteer - ID: " + std::to_string(getId()) + "\n" +
           ", Name: " + getName() + "\n" +
           ", Active Order ID: " + std::to_string(getActiveOrderId()) + "\n" +
           ", Completed Order ID: " + std::to_string(getCompletedOrderId()) + "\n" +
           ", Max Distance: " + std::to_string(getMaxDistance()) + "\n" +
           ", Distance Per Step: " + std::to_string(getDistancePerStep()) + "\n" +
           ", Distance Left: " + std::to_string(getDistanceLeft()) ;

}
string DriverVolunteer :: getRole() const {
    return "Driver";
}

LimitedDriverVolunteer::LimitedDriverVolunteer
(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders):
 DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {} 


LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(getId(), getName(), getMaxDistance(), getDistancePerStep(), maxOrders);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}


int LimitedDriverVolunteer :: getNumOrdersLeft() const{
    return ordersLeft; 
}

bool LimitedDriverVolunteer ::  hasOrdersLeft() const {
    return (ordersLeft > 0);
}

bool LimitedDriverVolunteer :: canTakeOrder(const Order &order) const {
    return hasOrdersLeft() && DriverVolunteer::canTakeOrder(order);
}

void LimitedDriverVolunteer :: acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
    return "Limited Driver Volunteer - ID: " + std::to_string(getId()) + "\n" +
           ", Name: " + getName() + "\n" +
           ", Active Order ID: " + std::to_string(getActiveOrderId()) + "\n" +
           ", Completed Order ID: " + std::to_string(getCompletedOrderId()) + "\n" +
           ", Max Distance: " + std::to_string(getMaxDistance()) + "\n" +
           ", Distance Per Step: " + std::to_string(getDistancePerStep()) + "\n" +
           ", Distance Left: " + std::to_string(getDistanceLeft()) + "\n" +
           ", Max Orders: " + std::to_string(maxOrders) + "\n" +
           ", Orders Left: " + std::to_string(ordersLeft);
}
string LimitedDriverVolunteer :: getRole() const {
    return "LimitedDriver";
}



















        


