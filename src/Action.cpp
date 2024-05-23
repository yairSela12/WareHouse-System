#include <string>
#include <vector>
#include "WareHouse.h"
#include "Action.h"
#include "Customer.h"
#include "Order.h"
#include "Volunteer.h"
using std::string;
using std::vector;
#include <iostream>
using namespace std;

extern WareHouse* backup;


BaseAction::BaseAction() :errorMsg("") ,status(ActionStatus::ERROR)  {}


ActionStatus BaseAction::getStatus() const {
    return status; 
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = std::move(errorMsg);
    cout << getErrorMsg() <<endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

string BaseAction::getStatusString(ActionStatus status) const {
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
////// simulateStep class
SimulateStep :: SimulateStep(int numOfSteps): numOfSteps(numOfSteps) {}
void SimulateStep::act(WareHouse& wareHouse) {
    for (int step = 0; step < numOfSteps; ++step) {
        wareHouse.assignOrdersToVolunteers();
        wareHouse.stepSimulation();
        wareHouse.removeExpiredVolunteers();
    }
    complete();
    wareHouse.addAction(clone());   
}

string SimulateStep::toString() const {
    return "simulateStep " + to_string(numOfSteps);
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}
///// end of simulateStep class

AddOrder:: AddOrder(int id): customerId(id){}

void AddOrder::act(WareHouse &wareHouse) {
    int counter = wareHouse.getCustomerCounter();
    if (customerId >= 0 && customerId < counter){
        Customer& customer = wareHouse.getCustomer(customerId);
        if(customer.canMakeOrder()){
        int distance = customer.getCustomerDistance();
        int orderId = wareHouse.getOrderCounter();
        wareHouse.raiseOrderCounter();
        Order* newOrder = new Order(orderId, customerId , distance);
        customer.addOrder(orderId);
        wareHouse.addOrder(newOrder);
       
        complete();
        }
        else{
            error("cannot place this order") ;
        }}  
    
    else{
         error("cannot place this order") ;
         }
    wareHouse.addAction(clone());
        }  
        
 

string AddOrder:: toString() const {
    return "order " + to_string(customerId);
  }  

AddOrder* AddOrder:: clone() const{
    return new AddOrder(*this);
}

//addCustomer
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
 customerName(customerName), customerType(toCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {}


void AddCustomer::act(WareHouse& wareHouse) {
    Customer* newCustomer = nullptr;
    if (customerType == CustomerType::Soldier) {
        newCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    } 
    else if (customerType == CustomerType::Civilian) {
        newCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    if (newCustomer != nullptr) {
        wareHouse.addCustomer(newCustomer);
        wareHouse.raiseCustomerCounter();
        
        complete();
       
    } else {
        error("Invalid customer type");
    }
    wareHouse.addAction(clone());
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}
string AddCustomer::toString() const {
    return "customer " + customerName + " " + typeToString(customerType) + " " + to_string(distance) + " " + to_string(maxOrders)  ;
}


string AddCustomer:: typeToString(CustomerType customerType){
    if (customerType == CustomerType :: Soldier){
        return "soldier";
    }
    else{
        return "civilian";
    }
}

CustomerType AddCustomer:: toCustomerType(const string &customerType){
    if (customerType == "soldier"){
        return CustomerType :: Soldier ;
    }
    else {
        return CustomerType :: Civilian ;
    }   // we assume that the input type can be only soldier / civilian.
    
}


//printOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    if(orderId < 0 || orderId >= wareHouse.getOrderCounter()){
     error("Order doesn't exist");
    }
    else{
    Order &order = wareHouse.getOrder(orderId);
    std::cout << order.toString() << std::endl;
    
    complete();
    } 
    wareHouse.addAction(clone());
       
}
PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    return "orderStatus " + to_string(orderId);
}


//PrintCustomerStatus
PrintCustomerStatus:: PrintCustomerStatus(int customerId):customerId(customerId){}
void PrintCustomerStatus:: act(WareHouse &wareHouse){
    int counter = wareHouse.getCustomerCounter();
    if(customerId >= 0 && customerId < counter ){
        Customer &customer = wareHouse.getCustomer(customerId);
        cout << "CustomerId: " << to_string(customerId) << endl;   
        for(int orderId : customer.getOrdersIds()){
            Order &order = wareHouse.getOrder(orderId);
            cout << "OrderId:" << to_string(orderId) << endl; 
            cout << "OrderStatus:" << order.orderStatusToString(order.getStatus()) << endl;
        }
        cout << "numOrdersLeft:" << to_string(customer.getNumOrdersLeft()) << endl;
        
        complete();
        
    }    
     else{  
        error("Customer doesn’t exist");
     }
     wareHouse.addAction(clone()); 
     }  
      
       


PrintCustomerStatus* PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus:: toString() const{
    return "customerStatus " + to_string(customerId);
}


//PrintVolunteerStatus 
PrintVolunteerStatus:: PrintVolunteerStatus(int id): volunteerId(id) {}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
   int counter = wareHouse.getVolunteerCounter();
   try{
    if (volunteerId >= 0 && volunteerId < counter ){
        Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
        cout << "VolunteerId: " << to_string(volunteerId) << endl;
        cout << "Busy: " << (volunteer.isBusy() ? "True" : "False") << endl;
        cout << "OrderId: " << ((volunteer.getActiveOrderId() == -1) ? "None" : to_string(volunteer.getActiveOrderId()) ) << endl;
        
        string role = volunteer.getRole();
        if(role == "Collector"){
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(&volunteer);
            cout << "TimeLeft: " << ((collector->getTimeLeft() <= 0) ? "None" : to_string(collector->getTimeLeft())) << endl;
            cout << "OrdersLeft:No Limit" << endl;
            }
        else if(role == "LimitedCollector"){
            LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(&volunteer);
            cout << "TimeLeft: " << ((limitedCollector->getTimeLeft() <= 0) ? "None" : to_string(limitedCollector->getTimeLeft())) << endl;
            cout << "OrdersLeft: "<< to_string(limitedCollector->getNumOrdersLeft()) << endl;
        }
        else if(role == "Driver"){
            DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(&volunteer);
            cout << "TimeLeft: " << ((driver->getDistanceLeft() <= 0) ? "None" : to_string(driver->getDistanceLeft())) << endl;
            cout << "OrdersLeft:No Limit" << endl;
        }
        else if(role == "LimitedDriver"){
            LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(&volunteer);
            cout << "TimeLeft: " << ((limitedDriver->getDistanceLeft() <= 0) ? "None" : to_string(limitedDriver->getDistanceLeft())) << endl;
            cout << "OrdersLeft:No Limit" << to_string(limitedDriver->getNumOrdersLeft()) <<endl;
        }
        complete();
        
    }
        else{
            error("Volunteer doesn't exist");
    }
}    
catch (const std::out_of_range& e) {
        error("Volunteer doesn't exist");
    }    
    wareHouse.addAction(clone());
}
    

PrintVolunteerStatus*  PrintVolunteerStatus:: clone() const{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus:: toString() const{
    return "volunteerStatus " + to_string(volunteerId);
}


// printActionLog
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog:: act(WareHouse &wareHouse){
    const vector<BaseAction*>& actions = wareHouse.getActions();
    for (const BaseAction* action : actions) {
        if (action != this) {
            cout << action->toString() << " " << getStatusString(action->getStatus()) << endl;
        }
    }
    complete();
    wareHouse.addAction(clone());
}
PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const {
    return "log ";
}
//Close
Close:: Close(){}
void Close:: act(WareHouse &wareHouse){
    for ( Order* order : wareHouse.getPendingOrders()) {
        cout << "OrderID: " << order->getId()
             << " , CustomerID: " << order->getCustomerId()
             << " , OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
    }

    for ( Order* order : wareHouse.getInProcessOrders()) {
        cout << "OrderID: " << order->getId()
             << " , CustomerID: " << order->getCustomerId()
             << " , OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
    }

    for ( Order* order : wareHouse.getCompletedOrders()) {
        cout << "OrderID: " << order->getId()
             << " , CustomerID: " << order->getCustomerId()
             << " , OrderStatus: Completed" << endl;
    }
    wareHouse.close();
    
    complete();
    wareHouse.addAction(clone());
}
Close* Close:: clone() const{
    return new Close(*this);
}
string Close:: toString() const{
    return "Closing the WareHouse";
}


//BackupWareHouse
BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        delete backup;
    }
    backup = new WareHouse(wareHouse);
    
    complete();
    wareHouse.addAction(clone());  
}
BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup ";
}

//RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup) {
        wareHouse = *backup;
        complete(); 
        
    } 
    else {
        error("No backup available");
    }
    wareHouse.addAction(clone());
     

}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "restore ";
}




