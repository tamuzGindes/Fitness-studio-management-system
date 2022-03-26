#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Customer.h"
#include "../include/Action.h"

class BaseAction;


class Studio {
public:
    Studio();

    Studio(const std::string &configFilePath);

    Studio(const Studio &other);

    Studio(Studio &&other);

    void start();

    int getNumOfTrainers() const;

    Trainer *getTrainer(int tid);

    const std::vector<BaseAction *> &getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout> &getWorkoutOptions();

    std::vector<Trainer *> &getTrainers();

    Studio *clone() { return new Studio(*this); }

    std::string toString() const;

    ~Studio();

    Studio &operator=(const Studio &other);

    Studio &operator=(Studio &&other);

    void clear();

private:
    bool open;
    int counterCustomerId;
    int counterTrainerId;
    int counterWorkoutId;
    std::vector<Trainer *> trainers;
    std::vector<int> workout_ids;
    std::vector<Workout> workout_options;
    std::vector<BaseAction *> actionsLog;
    bool isEmptyOrCommentLine(std::string line);
    std::vector<std::string> getUserInput(std::string input);

    Workout checkWorkoutType(int WorkoutId, std::string woName, int price, std::string woType);
};

#endif