
//
// Created by tamuz on 11/7/2021.
//

#include <iostream>
#include "../include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name),
                                                                                  price(w_price), type(w_type) {

}

Workout::Workout(const Workout &other) : id(other.id), name(other.name), price(other.price), type(other.type) {
}

int Workout::getId() const {
    return id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::toString() const {
    std::string output = "workout id : " + std::to_string(id) + " workout name :" + name + " workout price :" +
                         std::to_string(price) + " workout type :" + std::to_string(type);
    return output;
}

bool Workout::equals(Workout other) {
    if (type == other.getType() && id == other.getId() && name == other.getName() && price == other.getPrice())
        return true;
    else
        return false;
}