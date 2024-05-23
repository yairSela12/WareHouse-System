#pragma once
#include <string>
#include <vector>
using namespace std;
#include "Order.h"
#include "Customer.h"
class BaseAction;
class Volunteer;


class WareHouse {

    public:
        //constructor
        WareHouse(const string &configFilePath);
        //Destructor
        virtual ~WareHouse();
        //Copy Constructor
        WareHouse(const WareHouse& other);
        //Copy Assignment Operator
        WareHouse& operator=(const WareHouse& other);
        //Move Constructor
        WareHouse(WareHouse&& other) noexcept;
        //Move Assignment Operator
        WareHouse& operator=(WareHouse&& other) noexcept;
        
        
        // Another methods
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        
        void close();
        void open();

       
        // extra methods we created
        void addCustomer(Customer* customer);
        void stepSimulation();
        Volunteer* findAvailableCollector();
        Volunteer* findAvailableDriver();
        void removeExpiredVolunteers();
        void assignOrdersToVolunteers();
        void raiseCustomerCounter();
        void raiseVolunteerCounter();
        void raiseOrderCounter();
        int getCustomerCounter();
        int getVolunteerCounter();
        int getOrderCounter();
        const vector<Order*> &getPendingOrders() const;
        const vector<Order*> &getInProcessOrders() const;
        const vector<Order*> &getCompletedOrders() const;

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; ////For assigning unique Order IDs
        
        vector<vector<string>> parseConfigFile(const string &configFilePath);
};