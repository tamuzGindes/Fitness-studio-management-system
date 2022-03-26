//
// Created by tamuz on 11/7/2021.
//
#include <iostream>
#include <string>
#include <vector>
#include "../include/Action.h"

BaseAction::BaseAction() : errorMsg(""), status(ERROR), args("") {

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    BaseAction::errorMsg = errorMsg;
    std::cout << "Error: " + errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return BaseAction::errorMsg;
}

std::string BaseAction::getName() const {
    return name;
}

std::string BaseAction::getArgs() const {
    return args;
}

void BaseAction::setArgs(std::string argsIn) {
    args = argsIn;
}

BaseAction::~BaseAction() {

}


OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) {
    name = "open";
}

//copy constructor
OpenTrainer::OpenTrainer(OpenTrainer &other) : trainerId(other.trainerId), customers(other.customers) {
    setArgs(other.getArgs());
    name = other.getName();
    if (other.getStatus() == ActionStatus::COMPLETED)
        complete();
    else
        error(other.getErrorMsg());
}

void OpenTrainer::act(Studio &studio) {
    std::string args = std::to_string(trainerId) + " ";
    Trainer *trainer = studio.getTrainer(trainerId);
    int customersSize = customers.size();
    if (trainer == nullptr || trainer->isOpen()) {
        error("Trainer does not exist or is not open.");
        for (int i = 0; i < customersSize; ++i) {
            args = args + customers.at(i)->toString() + ",";
            delete customers.at(i);
        }
        setArgs(args.substr(0, args.size() - 1));
    } else {
        int numOfCustomersAdded = 0;
        if (customersSize != 0) {
            trainer->openTrainer();
            // Add customers to trainer list
            for (int i = 0; i < customersSize; ++i) {
                args = args + customers.at(i)->toString() + ",";
                if (numOfCustomersAdded < trainer->getCapacity()) {
                    trainer->addCustomer(customers.at(i));
                    numOfCustomersAdded = numOfCustomersAdded + 1;
                }
                else{
                    delete customers.at(i);
                }
            }
            complete();
            setArgs(args.substr(0, args.size() - 1));
        }
    }
}


std::string OpenTrainer::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

OpenTrainer::~OpenTrainer() {

}

Order::Order(int id) : trainerId(id) {
    name = "order";
}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    std::string args = "";
    args += std::to_string(trainerId);
    args += " ";
    if (trainer == nullptr || !trainer->isOpen()) {
        error("Trainer does not exist or is not open.");
    } else {
        std::vector<Customer *> customersOfTrainer = trainer->getCustomers();
        Customer *customer;
        for (int i = 0; i < trainer->getNumOfCustomers(); ++i) {
            customer = customersOfTrainer.at(i);
            trainer->order(customer->getId(), customer->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        }
        std::vector<OrderPair> *pairs = &trainer->getOrders();
        for (int i = 0; i < trainer->getNumOfOrders(); i++) {
            int index = trainer->getCustomerIndex(pairs->at(i).first);
            if (index != -1) {
                std::cout << ((trainer->getCustomers()).at(index))->getName() + " Is Doing " +
                             pairs->at(i).second.getName() << std::endl;
            }
        }
        complete();
    }
    setArgs(args);

}

std::string Order::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {
    name = "move";
}

void MoveCustomer::act(Studio &studio) {
    std::string arg = "" + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    setArgs(arg);
    Trainer *newTrainer = studio.getTrainer(dstTrainer);
    Trainer *oldTrainer = studio.getTrainer(srcTrainer);

    // check if the transfer is possible
    if (!legalMove(newTrainer, oldTrainer)) {
        error("Cannot move customer");
    } else {
        //getting customer and his workouts from old trainer
        std::vector<OrderPair> &oldTrainerOrders = oldTrainer->getOrders();
        Customer *customer = oldTrainer->getCustomer(id);
        std::vector<Workout> workouts;
        for (int i = 0; i < oldTrainer->getNumOfOrders(); i++) {
            if (oldTrainerOrders.at(i).first == id) {
                workouts.push_back(oldTrainerOrders.at(i).second);
            }
        }
        //remove the customer from old trainer list
        oldTrainer->removeCustomer(id);
        //end trainer session if there are no customers left
        if (oldTrainer->getCustomers().size() == 0) {
            oldTrainer->closeTrainer();
        }

        //moving orders to the new trainer, and update his salary
        int prevSalary;
        newTrainer->addCustomer(customer);
        int numOfWorkouts = workouts.size();
        for (int i = 0; i < numOfWorkouts; i++) {
            //setting orders
            OrderPair pair(id, workouts.at(i));
            newTrainer->getOrders().push_back(pair);
            //setting salary
            prevSalary = newTrainer->getSalary();
            newTrainer->setSalary(prevSalary + workouts.at(i).getPrice());
        }
        complete();
    }
}

bool MoveCustomer::legalMove(Trainer *newTrainer, Trainer *oldTrainer) const {
    return newTrainer != nullptr && oldTrainer != nullptr && oldTrainer->isOpen() && newTrainer->isOpen() &&
           oldTrainer->getCustomer(id) != nullptr && newTrainer->getNumOfCustomers() < newTrainer->getCapacity();
}

std::string MoveCustomer::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}


Close::Close(int id) : trainerId(id) {
    name = "close";
}

void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        error("Trainer does not exist or is not open");
    } else {
        std::cout << "Trainer " << trainerId << " closed. Salary " << trainer->getSalary() << "NIS" << std::endl;
        trainer->closeTrainer();
        complete();
    }
    std::string args = "";
    args += std::to_string(trainerId);
    args += " ";
    setArgs(args);
}

std::string Close::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

CloseAll::CloseAll() {
    name = "closeall";
}

void CloseAll::act(Studio &studio) {
    std::vector<Trainer *> &trainers = studio.getTrainers();
    //close all trainers in studio
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
        if (trainers.at(i)->isOpen()) {
            Close close(studio.getTrainers().at(i)->getId());
            close.act(studio);
        }
    }

    //delete all actions and trainers in studio
    const std::vector<BaseAction *> &actionsLog = studio.getActionsLog();
    int numOfActions = actionsLog.size();
    for (int i = 0; i < numOfActions; ++i) {
        delete actionsLog.at(i);
    }
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
        delete trainers.at(i);
    }
    studio.clear();
    complete();
}

std::string CloseAll::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

PrintWorkoutOptions::PrintWorkoutOptions() {
    name = "workout_options";
}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts = studio.getWorkoutOptions();
    int numOfWorkouts = workouts.size();
    for (int i = 0; i < numOfWorkouts; ++i) {
        std::cout << workouts.at(i).getName() << ", " << workouts.at(i).getType() << ", " << workouts.at(i).getPrice()
                  << "NIS "
                  << std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {
    name = "status";
}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    std::string status;
    if (trainer->isOpen()) {
        //print trainer status and his customers
        status = "open";
        std::cout << "Trainer " << trainer->getId() << " status: " << status << "\nCustomers: " << std::endl;
        std::vector<Customer *> customers = trainer->getCustomers();
        for (int i = 0; i < trainer->getNumOfCustomers(); ++i) {
            std::cout << customers.at(i)->getId() << " " << customers.at(i)->getName() << std::endl;
        }

        //print orders
        std::cout << "Orders: \n";
        std::vector<OrderPair> &orders = trainer->getOrders();
        for (int i = 0; i < trainer->getNumOfOrders(); ++i) {
            std::cout << orders.at(i).second.getName() << " " << orders.at(i).second.getPrice() << "NIS "
                      << orders.at(i).first
                      << std::endl;
        }
        std::cout << "Current Trainer's Salary: " << trainer->getSalary() << "NIS " << std::endl;
    } else {
        status = "close";
        std::cout << "Trainer " << trainer->getId() << " status: " << status << std::endl;
    }
    std::string args = "";
    args += std::to_string(trainerId);
    args += " ";
    setArgs(args);
    complete();
}

std::string PrintTrainerStatus::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}


PrintActionsLog::PrintActionsLog() {
    name = "log";
}

void PrintActionsLog::act(Studio &studio) {
    std::string status = "";
    std::vector<BaseAction *> log = studio.getActionsLog();
    int numOfActions = log.size();

    for (int i = 0; i < numOfActions; ++i) {
        std::cout << log.at(i)->toString() << std::endl;
    }
    complete();
    setArgs("");
}

std::string PrintActionsLog::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

BackupStudio::BackupStudio() {
    name = "backup";
}


void BackupStudio::act(Studio &studio) {
    if (backup != nullptr) {
        delete backup;
    }
    backup = studio.clone();
    complete();
    setArgs("");
}

std::string BackupStudio::toString() const {
    std::string stat = "Completed";
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}

RestoreStudio::RestoreStudio() {
    name = "restore";
}

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        error("No backup available");
    } else {
        studio = *backup;
        complete();
    }
    setArgs("");
}

std::string RestoreStudio::toString() const {
    std::string stat = "";
    if (getStatus() == COMPLETED) {
        stat = "Completed";
    } else {
        stat = "Error: " + getErrorMsg();
    }
    std::string output = getName() + " " + getArgs() + stat;
    return output;
}