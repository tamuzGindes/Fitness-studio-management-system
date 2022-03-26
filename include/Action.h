#pragma once
#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/Studio.h"

enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

extern Studio *backup;

class BaseAction {
public:
    BaseAction();

    ActionStatus getStatus() const;

    virtual void act(Studio &studio) = 0;

    virtual std::string toString() const = 0;

    virtual BaseAction *clone() = 0;

    std::string getName() const;

    std::string getArgs() const;

    virtual ~BaseAction() = 0;

protected:
    void complete();

    void setArgs(std::string argsIn); //setting and saving arguments given by input

    void error(std::string errorMsg);

    std::string getErrorMsg() const;

    std::string name = "";
private:
    std::string errorMsg;
    ActionStatus status;
    std::string args;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);

    OpenTrainer(OpenTrainer &other);

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new OpenTrainer(*this); }

    ~OpenTrainer();

private:
    const int trainerId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new Order(*this); }


private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new MoveCustomer(*this); }


private:
    bool legalMove(Trainer *oldTrainer, Trainer *newTrainer) const;

    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new Close(*this); }

private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();

    void act(Studio &studio);

    BaseAction *clone() { return new CloseAll(*this); }

    std::string toString() const;

private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new PrintWorkoutOptions(*this); }

private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new PrintTrainerStatus(*this); }

private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new PrintActionsLog(*this); }

private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new BackupStudio(*this); }

private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();

    void act(Studio &studio);

    std::string toString() const;

    BaseAction *clone() { return new RestoreStudio(*this); }

};


#endif
