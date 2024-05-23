#include <string>
#include <vector>
#include <algorithm>
#include "WareHouse.h"
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"
#include <sstream>
#include <fstream>
#include <iostream> 
using namespace std;

WareHouse::WareHouse(const string &configFilePath)
    : isOpen(false),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0), volunteerCounter(0),orderCounter(0)  {
     vector<vector<string>> configFileLines = parseConfigFile(configFilePath); 
     
    for (const vector<string>& line : configFileLines) {
        if (!line.empty()) {
            const string& command = line[0];
            
            if (command == "customer") {
                if (line.size() >= 5) {
                    string name = line[1];
                    string type = line[2];
                    int distance = stoi(line[3]);
                    int maxOrders = stoi(line[4]);
                    if(type == "soldier"){
                        customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders)); 
                        raiseCustomerCounter();
                    }
                    else if(type == "civilian"){
                          customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders)); 
                          raiseCustomerCounter();
                    }}}                   
               
             else if (command == "volunteer") {
                
                if (line.size() >= 4) {
                    string name = line[1];
                    string role = line[2];
                    int coolDown;
                    int maxOrders;
                    int maxDistance;
                    int distancePerStep;

                    if(role == "collector"){
                       if (line.size() >= 4) { 
                        coolDown = stoi(line[3]);  
                        volunteers.push_back(new CollectorVolunteer(volunteerCounter, name , coolDown)); 
                        raiseVolunteerCounter();
                       }  
                    }
                    else if (role == "limited_collector") {
                        if (line.size() >= 5) {
                        coolDown = stoi(line[3]);
                        maxOrders = stoi(line[4]);
                        volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, coolDown , maxOrders)); 
                        raiseVolunteerCounter();

                    }}
                    
                    else if (role == "driver") {
                        if (line.size() >= 5) {
                            maxDistance = stoi(line[3]);
                            distancePerStep = stoi(line[4]);
                            volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep)); 
                            raiseVolunteerCounter();
                        }
                    }
                    else if (role == "limited_driver") {
                        if (line.size() >= 6) {
                            maxDistance = stoi(line[3]);
                            distancePerStep = stoi(line[4]);
                            maxOrders = stoi(line[5]);
                            volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep , maxOrders)); 
                            raiseVolunteerCounter();
                       
}}}}}}}

//Destructor
WareHouse::~WareHouse() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }
    volunteers.clear();

    for (Order* order : pendingOrders) {
        delete order;
    }
    pendingOrders.clear();

    for (Order* order : inProcessOrders) {
        delete order;
    }
    inProcessOrders.clear();

    for (Order* order : completedOrders) {
        delete order;
    }
    completedOrders.clear();

    for (Customer* customer : customers) {
        delete customer;
    }
    customers.clear();

    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter = 0;
}

//Copy Constructor
WareHouse::WareHouse(const WareHouse& other)
    : isOpen(other.isOpen),actionsLog(), volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),
      customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter) {
    // Copy actionsLog
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    // Copy volunteers
    for (Volunteer* volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }
    // Copy pendingOrders
    for (Order* order : other.pendingOrders) {
        pendingOrders.push_back(new Order(*order));
    }
    // Copy inProcessOrders
    for (Order* order : other.inProcessOrders) {
        inProcessOrders.push_back(new Order(*order));
    }
    // Copy completedOrders
    for (Order* order : other.completedOrders) {
        completedOrders.push_back(new Order(*order));
    }
    // Copy customers
    for (Customer* customer : other.customers) {
        customers.push_back(customer->clone());
    }
}

// Copy Assignment Operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) { 
        
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
        
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();
       
        for (Order* order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear();
        
        for (Order* order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();
        
        for (Order* order : completedOrders) {
            delete order;
        }
        completedOrders.clear();
       
        for (Customer* customer : customers) {
            delete customer;
        }
        customers.clear();

        // Copy simple data members
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Copy vectors
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        for (Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }
        for (Order* order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }
        for (Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }
        for (Order* order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }
        for (Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }
    }
    return *this;
}

// Move Constructor
WareHouse::WareHouse(WareHouse&& other) noexcept
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter) {
}

// Move Assignment Operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();

        for (Order* order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear();

        for (Order* order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();

        for (Order* order : completedOrders) {
            delete order;
        }
        completedOrders.clear();

        for (Customer* customer : customers) {
            delete customer;
        }
        customers.clear();

        // Move resources from the other object
        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Reset the other object
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
    }

    return *this;
}

void WareHouse:: addOrder(Order* order){
    if(order != nullptr){
    pendingOrders.push_back(order);}
}

void WareHouse::addAction(BaseAction* action){
    if(action != nullptr){
    actionsLog.push_back(action);}
}
Customer& WareHouse::getCustomer(int customerId) const {
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
    static SoldierCustomer customerNotFound = SoldierCustomer(-1 , "error" , -1 , -1);
    return customerNotFound; // we are not going to get this return ever!

}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    } 

    throw std::out_of_range("Volunteer with specified ID not found");
}

Order& WareHouse::getOrder(int orderId) const {
    for (Order* order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order* order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order* order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }}
    static Order orderNotFound = Order(-1 , -1 ,-1);
    return orderNotFound; // we are not going to get this return ever!
    
}


const vector<BaseAction*>& WareHouse :: getActions() const{
    return actionsLog;
}

const vector<Order*>& WareHouse :: getPendingOrders() const{
    return pendingOrders;
}
 
const vector<Order*>& WareHouse :: getInProcessOrders() const{ 
    return inProcessOrders;
}

const vector<Order*>& WareHouse :: getCompletedOrders() const{
    return completedOrders;
}

void WareHouse::start() {
    isOpen = true;
    cout << "Warehouse is open!" << endl;

    string userInput;
    while (isOpen) {
        cout << "> ";
        getline(cin, userInput);

        stringstream ss(userInput);
        string actionName;
        ss >> actionName;

        if (actionName == "step") {
            int numSteps;
            ss >> numSteps;
            SimulateStep simulateStep(numSteps);
            simulateStep.act(*this);
        } else if (actionName == "order") {
            int customerId;
            ss >> customerId;
            AddOrder addOrder(customerId);
            addOrder.act(*this);
        } else if (actionName == "customer") {
            string customerName, customerType;
            int distance, maxOrders;
            ss >> customerName >> customerType >> distance >> maxOrders;
            AddCustomer addCustomer(customerName, customerType, distance, maxOrders);
            addCustomer.act(*this);
        }
        else if (actionName == "orderStatus") {
            int orderId;
            ss >> orderId;
            PrintOrderStatus printOrderStatus(orderId);
            printOrderStatus.act(*this);
        } else if (actionName == "customerStatus") {
            int customerId;
            ss >> customerId;
            PrintCustomerStatus printCustomerStatus(customerId);
            printCustomerStatus.act(*this);
        } else if (actionName == "volunteerStatus") {
            int volunteerId;
            ss >> volunteerId;
            PrintVolunteerStatus printVolunteerStatus(volunteerId);
            printVolunteerStatus.act(*this);
        } else if (actionName == "log") {
            PrintActionsLog printActionsLog;
            printActionsLog.act(*this);
        } else if (actionName == "close") {
            Close closeAction;
            closeAction.act(*this);
        } else if (actionName == "backup") {
            BackupWareHouse BackupAction;
            BackupAction.act(*this);
        } else if (actionName == "restore") {
            RestoreWareHouse restoreAction;
            restoreAction.act(*this);
        }
        else {
            cout << "Unknown action. Please enter a valid action." << endl;
        }
    }
}
void WareHouse:: close(){
    isOpen = false;
    
}

void WareHouse::assignOrdersToVolunteers() {
    auto it = pendingOrders.begin();
    while (it != pendingOrders.end()) {
        Order* order = *it;
        if ((order->getStatus() == OrderStatus::PENDING) ) {
            Volunteer* collector = findAvailableCollector();
            if (collector) {
                collector->acceptOrder(*order);
                order->setCollectorId(collector->getId());
                it = pendingOrders.erase(it);
                inProcessOrders.push_back(order);
                order->setStatus(OrderStatus::COLLECTING);
            } else {
                ++it; 
            }
        } else if (order->getStatus() == OrderStatus::COLLECTING) {
            Volunteer* driver = findAvailableDriver();
            if (driver) {
                driver->acceptOrder(*order);
                order->setDriverId(driver->getId());
                it = pendingOrders.erase(it);
                inProcessOrders.push_back(order);
                order->setStatus(OrderStatus::DELIVERING);
            } else {
                ++it; 
            }
        } else {
            ++it; 
        }
    }
}

    


Volunteer* WareHouse::findAvailableCollector() {
    for (Volunteer* volunteer : volunteers) {
        string role = volunteer->getRole();
        if ( role == "Collector" || role == "LimitedCollector") {
            if (!(volunteer->isBusy())) {
                return volunteer;
            }
        }
    }
    return nullptr;
}

Volunteer* WareHouse::findAvailableDriver() {
    for (Volunteer* volunteer : volunteers) {
        string role = volunteer->getRole();
        if (role == "Driver" || role == "LimitedDriver") {
            if (!(volunteer->isBusy())) {
                return volunteer;
            }
        }
    }
    return nullptr; // No available driver
}

void WareHouse::stepSimulation() {
    for (Volunteer* volunteer : volunteers) {
        if(volunteer->isBusy()){
           volunteer->step(); 
           if(!(volunteer->isBusy())){
                Order* completedOrder = &(getOrder(volunteer->getCompletedOrderId()));
                string role = volunteer->getRole();
                if( role == "Collector" || role == "LimitedCollector" ){
                   // Find the iterator for the specific pointer in the source vector
                    auto it = std:: find(inProcessOrders.begin() , inProcessOrders.end(), completedOrder);
                    if(it != inProcessOrders.end()){
                    inProcessOrders.erase(it);
                    pendingOrders.push_back(completedOrder);
                    
                }

                else {
                    std::cout << "Pointer not found in the pendingOrders" << std::endl;
                }}

                else if( role == "Driver" || role == "LimitedDriver") {
                    auto it = std:: find(inProcessOrders.begin() , inProcessOrders.end(), completedOrder);
                    if(it != inProcessOrders.end()){
                    inProcessOrders.erase(it);
                    completedOrders.push_back(completedOrder);
                    completedOrder->setStatus(OrderStatus:: COMPLETED);
                    }
                    else {
                        std::cout << "Pointer not found in the pendingOrders" << std::endl;
                }}}}}}    
    





void WareHouse::removeExpiredVolunteers() {
    auto it = volunteers.begin();
    while (it != volunteers.end()) {
        Volunteer* volunteer = *it;
        if (!(volunteer->hasOrdersLeft()) && !(volunteer->isBusy())) {
            delete volunteer;  
            it = volunteers.erase(it);  
        } 
        else {
            ++it;  
        }
    }
}





void WareHouse:: raiseCustomerCounter(){
    customerCounter++;
}

void WareHouse:: raiseVolunteerCounter(){
    volunteerCounter++;
}

void WareHouse:: raiseOrderCounter(){
    orderCounter++;
}

int WareHouse:: getCustomerCounter(){
    return customerCounter;
}

int WareHouse:: getVolunteerCounter(){
    return volunteerCounter;
}

int WareHouse:: getOrderCounter(){
    return orderCounter;
}

void WareHouse:: addCustomer(Customer* customer) {
    customers.push_back(customer);
    }
vector<vector<string>> WareHouse::parseConfigFile(const string &configFilePath) {
    ifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        cerr << "Error: Unable to open configuration file." << endl;
        exit(1); // or handle the error in a way that makes sense for your program
    }

    vector<vector<string>> configFileLines;
    string line;
    while (getline(configFile, line)) {
        // Ignore comments starting with '#'
        size_t commentPos = line.find('#');
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }

        istringstream iss(line);
        vector<string> words;
        string word;
        while (iss >> word) {
            words.push_back(word);
        }

        if (!words.empty()) {
            configFileLines.push_back(words);
        }
    }

    configFile.close();

    return configFileLines;
}