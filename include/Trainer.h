#pragma once
#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "../include/Customer.h"
#include "../include/Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer {
public:
    Trainer(int t_capacity);

    Trainer(const Trainer &other);

    Trainer(Trainer &&other);

    Trainer& operator=(const Trainer &other);

    Trainer &operator=(Trainer &&other);

    int getCapacity() const;

    void setId(int t_id);

    void addCustomer(Customer *customer);

    void removeCustomer(int id);

    int getCustomerIndex(int id);

    std::vector<Customer *> &getCustomers();

    std::vector<OrderPair> &getOrders();

    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options);

    void openTrainer();

    void closeTrainer();

    int getSalary();

    int getId() const;

    void setSalary(int newSalary);

    bool isOpen();

    void clear();

    Customer *getCustomer(int id);

    bool equalOrderPair(OrderPair pair1, OrderPair pair2);

    ~Trainer();

    std::string toString() const;

    Trainer *clone() { return new Trainer(*this); }

    int getNumOfCustomers() const;

    int getNumOfOrders() const;

private:
    int id;
    int salary;
    int capacity;
    bool open;

    void deleteOrders(int customer_id);

    std::vector<Customer *> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)

};


#endif