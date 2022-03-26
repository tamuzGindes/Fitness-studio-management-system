//
// Created by tamuz on 11/7/2021.
//

#include "../include/Customer.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {
}

Customer::Customer(const Customer &other) : name(other.name), id(other.id) {
}


std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() {

}


SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workouts;
    int numOfOptions = workout_options.size();
    for (int i = 0; i < numOfOptions; ++i) {
        if (workout_options.at(i).getType() == WorkoutType::CARDIO) {
            workouts.push_back(workout_options.at(i).getId());
        }
    }
    return workouts;
}

std::string SweatyCustomer::toString() const {
    return getName() + ",swt ";
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workouts;
    int cheapestIndex = 0;
    int numOfOptions = workout_options.size();
    for (int i = 0; i < numOfOptions; ++i) {
        if (workout_options.at(i).getPrice() < workout_options.at(cheapestIndex).getPrice()) {
            cheapestIndex = i;
        }
        if (workout_options.at(i).getPrice() == workout_options.at(cheapestIndex).getPrice()) {
            if (workout_options.at(i).getId() < workout_options.at(cheapestIndex).getId()) {
                cheapestIndex = i;
            }
        }
    }
    if (!workout_options.empty()) {
        workouts.push_back(workout_options.at(cheapestIndex).getId());
    }
    return workouts;
}

std::string CheapCustomer::toString() const {
    return getName() + ",chp ";
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workouts;
    int numOfOptions = workout_options.size();
    std::vector<pair<int, int>> anaerobicWorkouts;//<price,index>
    for (int i = 0; i < numOfOptions; ++i) {
        if (workout_options.at(i).getType() == WorkoutType::ANAEROBIC) {
            anaerobicWorkouts.push_back(pair<int, int>(workout_options.at(i).getPrice(), i));
        }
    }
    sort(anaerobicWorkouts.begin(), anaerobicWorkouts.end());
    std::reverse(anaerobicWorkouts.begin(), anaerobicWorkouts.end());
    int numOfAnaeronic = anaerobicWorkouts.size();
    for (int i = 0; i < numOfAnaeronic; ++i) {
        workouts.push_back(workout_options.at(anaerobicWorkouts.at(i).second).getId());
    }
    return workouts;
}

std::string HeavyMuscleCustomer::toString() const {
    return getName() + ",mcl ";
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workouts;
    int cardioIndex = -1;
    int mixedIndex = -1;
    int anaerobicIndex = -1;
    bool foundCardio = false;
    bool foundMixed = false;
    bool foundAnaerobic = false;
    int numOfOptions = workout_options.size();
    if (!workout_options.empty()) {
        for (int i = 0; i < numOfOptions; ++i) {
            if (workout_options.at(i).getType() == WorkoutType::CARDIO && !foundCardio) {
                cardioIndex = i;
                foundCardio = true;
            }
            if (workout_options.at(i).getType() == WorkoutType::MIXED && !foundMixed) {
                mixedIndex = i;
                foundMixed = true;
            }
            if (workout_options.at(i).getType() == WorkoutType::ANAEROBIC && !foundAnaerobic) {
                anaerobicIndex = i;
                foundAnaerobic = true;
            }
            if (foundMixed && foundAnaerobic && foundCardio) {
                break;
            }
        }
        for (int i = 0; i < numOfOptions; ++i) {
            if (foundCardio) {
                if (workout_options.at(i).getType() == WorkoutType::CARDIO &&
                    (workout_options.at(i).getPrice() < workout_options.at(cardioIndex).getPrice())) {
                    cardioIndex = i;
                } else if (workout_options.at(i).getType() == WorkoutType::CARDIO &&
                           workout_options.at(i).getPrice() == workout_options.at(cardioIndex).getPrice()) {
                    if (workout_options.at(i).getId() < workout_options.at(cardioIndex).getId()) {
                        cardioIndex = i;
                    }
                }
            }
            if (foundMixed) {
                for (int i = 0; i < numOfOptions; ++i) {
                    if (workout_options.at(i).getType() == WorkoutType::MIXED &&
                        workout_options.at(i).getPrice() > workout_options.at(mixedIndex).getPrice()) {
                        mixedIndex = i;
                    } else if (workout_options.at(i).getType() == WorkoutType::MIXED &&
                               workout_options.at(i).getPrice() == workout_options.at(mixedIndex).getPrice()) {
                        if (workout_options.at(i).getId() < workout_options.at(mixedIndex).getId()) {
                            mixedIndex = i;
                        }
                    }
                }
            }
            if (foundAnaerobic) {
                if (workout_options.at(i).getType() == WorkoutType::ANAEROBIC &&
                    (workout_options.at(i).getPrice() < workout_options.at(anaerobicIndex).getPrice())) {
                    anaerobicIndex = i;
                } else if (workout_options.at(i).getType() == WorkoutType::ANAEROBIC &&
                           workout_options.at(i).getPrice() == workout_options.at(anaerobicIndex).getPrice()) {
                    if (workout_options.at(i).getId() < workout_options.at(anaerobicIndex).getId()) {
                        anaerobicIndex = i;
                    }
                }
            }
        }
        if (foundCardio)
            workouts.push_back(workout_options.at(cardioIndex).getId());
        if (foundMixed)
            workouts.push_back(workout_options.at(mixedIndex).getId());
        if (foundAnaerobic)
            workouts.push_back(workout_options.at(anaerobicIndex).getId());
    }
    return workouts;
}

std::string FullBodyCustomer::toString() const {
    return getName() + ",fbd ";
}