//
// Created by tamuz on 11/7/2021.
//

#include <algorithm>
#include "../include/Trainer.h"
#include "../include/Customer.h"
#include "iostream"

Trainer::Trainer(int t_capacity) : id(0), salary(0), capacity(t_capacity), open(false), customersList(), orderList() {
}

Trainer::Trainer(const Trainer &other) : id(other.id), salary(other.salary), capacity(other.capacity), open(other.open),
                                         customersList(), orderList(other.orderList) {
    int oSizeCustomer = other.customersList.size();
    for (int i = 0; i < oSizeCustomer; ++i) {
        customersList.push_back(other.customersList.at(i)->clone());
    }
}

Trainer::Trainer(Trainer &&other) : id(other.id), salary(other.salary), capacity(other.capacity), open(other.open),
                                    customersList(), orderList(other.orderList) {
    int otherNumOfCustomers = other.customersList.size();
    for (int i = 0; i < otherNumOfCustomers; ++i) {
        customersList.push_back(other.customersList.at(i));
    }
    other.clear();
}



int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::setId(int t_id) {
    id = t_id;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

Customer *Trainer::getCustomer(int id) {
    int index = getCustomerIndex(id);
    if (index != -1) {
        return getCustomers().at(index);
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    bool alreadyOrdered = false;
    int numOfWorkouts = workout_ids.size();
    int numOfOptions = workout_options.size();
    int numsOfOrders = orderList.size();
    for (int i = 0; i < numOfWorkouts; ++i) {
        for (int j = 0; j < numOfOptions; ++j) {
            if (workout_options.at(j).getId() == workout_ids.at(i)) {
                Workout orderedWorkout = workout_options.at(j);
                OrderPair order(customer_id, orderedWorkout);
                //check if trainer already has this order
                for (int f = 0; f < numsOfOrders && !alreadyOrdered; ++f) {
                    if (equalOrderPair(order, orderList.at(f)))
                        alreadyOrdered = true;
                }
                if (!alreadyOrdered) {
                    orderList.push_back(order);
                    salary += order.second.getPrice();
                }
            }
        }
    }
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    salary = 0;
    int numOfCustomers = customersList.size();
    for (int i = 0; i < numOfCustomers; ++i) {
        delete customersList.at(i);
    }
    customersList.clear();
    orderList.clear();
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

void Trainer::setSalary(int newSalary) {
    salary = newSalary;
}

int Trainer::getId() const {
    return id;
}


void Trainer::removeCustomer(int id) {
    int numOfCustomers = customersList.size();
    for (int i = 0; i < numOfCustomers; ++i) {
        if (customersList.at(i)->getId() == id) {
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    deleteOrders(id);
}

void Trainer::deleteOrders(int customer_id) {
    std::vector<OrderPair> workoutsToSave;
    int numsOfOrders = orderList.size();
    for (int i = 0; i < numsOfOrders; ++i) {
        if (orderList.at(i).first == customer_id)
            salary -= orderList.at(i).second.getPrice();
        else
            workoutsToSave.push_back(orderList.at(i));
    }
    orderList.clear();
    int numOfWorkouts = workoutsToSave.size();
    for (int i = 0; i < numOfWorkouts; ++i) {
        orderList.push_back(workoutsToSave.at(i));
    }
}

Trainer::~Trainer() {
    int numOfCustomers = customersList.size();
    for (int i = 0; i < numOfCustomers; ++i) {
        delete customersList.at(i);
    }
}

Trainer& Trainer::operator=(const Trainer &other) {
    if (&other != this) {
        id = other.id;
        salary = other.salary;
        capacity = other.capacity;
        open = other.open;
        int numOfCustomers = customersList.size();
        int otherNumOfCustomers = other.customersList.size();
        int numOfOtherOrders = other.orderList.size();
        //delete customers
        for (int i = 0; i < numOfCustomers; ++i) {
            delete customersList.at(i);
        }
        customersList.clear();
        //copy customers
        for (int i = 0; i < otherNumOfCustomers; ++i) {
            customersList.push_back(other.customersList.at(i)->clone());
        }
        orderList.clear();
        for (int i = 0; i < numOfOtherOrders; ++i) {
            orderList.push_back(OrderPair(other.orderList.at(i).first, other.orderList.at(i).second.clone()));
        }
    }
    return *this;
}

Trainer &Trainer::operator=(Trainer &&other) {
    id = other.id;
    salary = other.salary;
    capacity = other.capacity;
    open = other.open;
    orderList.clear();
    int numOfCustomers = customersList.size();
    int otherNumOfCustomers = other.customersList.size();
    int otherNumOfOrders = other.orderList.size();
    for (int i = 0; i < otherNumOfOrders; ++i) {
        orderList.push_back(other.orderList.at(i));
    }
    for (int i = 0; i < numOfCustomers; ++i) {
        delete customersList.at(i);
    }
    customersList.clear();
    for (int i = 0; i < otherNumOfCustomers; ++i) {
        customersList.push_back(other.customersList.at(i));
    }
    other.clear();
    return *this;
}

void Trainer::clear() {
    id = 0;
    salary = 0;
    capacity = 0;
    open = false;
    orderList.clear();
    customersList.clear();

}

int Trainer::getCustomerIndex(int id) {
    int numOfCustomers = customersList.size();
    for (int i = 0; i < numOfCustomers; ++i) {
        if (customersList.at(i)->getId() == id)
            return i;
    }
    return -1;
}

bool Trainer::equalOrderPair(OrderPair pair1, OrderPair pair2) {
    if (pair1.first == pair2.first && pair1.second.equals(pair2.second))
        return true;
    else
        return false;
}

std::string Trainer::toString() const {
    std::string trainer =
            "Trainer: id-" + std::to_string(id) + " salary-" + " open-" + std::to_string(open) + "  customers list :  ";
    int numOfCustomers = customersList.size();
    int numOfOrders = orderList.size();
    for (int i = 0; i < numOfCustomers; ++i) {
        trainer += customersList.at(i)->toString();
        trainer += "  ";
    }
    trainer += "  orderPair list :  ";
    for (int i = 0; i < numOfOrders; ++i) {
        trainer += " customer_id : " + std::to_string(orderList.at(i).first) + "  workout :  " +
                   orderList.at(i).second.toString();
    }
    return trainer;
}

int Trainer::getNumOfCustomers() const {
    return customersList.size();
}

int Trainer::getNumOfOrders() const {
    return orderList.size();
}
