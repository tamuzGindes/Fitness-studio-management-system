
//
// Created by tamuz on 11/7/2021.
//
#include "../include/Customer.h"
#include "../include/Studio.h"


#include <iostream>

Studio::Studio() : open(true), counterCustomerId(0), counterTrainerId(0), counterWorkoutId(0), trainers(),
                   workout_ids(), workout_options(), actionsLog() {

}

Studio::Studio(const std::string &configFilePath) : open(true), counterCustomerId(0), counterTrainerId(0),
                                                    counterWorkoutId(0), trainers(), workout_ids(), workout_options(),
                                                    actionsLog() {
    std::string line;
    std::ifstream myFile;
    myFile.open(configFilePath, std::ios::in);
    if (myFile.is_open()) {
        //getting first parameter - num of trainers
        getline(myFile, line);
        if (isEmptyOrCommentLine(line))
            getline(myFile, line);
        int numOfTrainers = stoi(line);

        //getting second parameter - for each trainer initialize capacity
        getline(myFile, line);
        if (isEmptyOrCommentLine(line))
            getline(myFile, line);
        if (isEmptyOrCommentLine(line))
            getline(myFile, line);

        std::vector <std::string> input2 = getUserInput(line);
        int numOfCustomers = 0;
        for (int i = 0; i < numOfTrainers; ++i) {
            numOfCustomers = stoi(input2.at(i));
            Trainer *trainer = new Trainer(numOfCustomers);
            trainer->setId(counterTrainerId);
            counterTrainerId += 1;
            trainers.push_back(trainer);
        }

        //getting third parameter - insert workout options
        int price = 0;
        std::string line = "";
        std::string workoutName = "";
        std::string workoutType = "";
        while (getline(myFile, line)) {
            if (isEmptyOrCommentLine(line))
                getline(myFile, line);
            if (isEmptyOrCommentLine(line))
                getline(myFile, line);
            //split the line by ',' and use the parameters to initialize workouts
            std::vector <std::string> input3 = getUserInput(line);
            int inputSize = input3.size();
            if (inputSize > 3){ // in case the workout name contains more than 1 word
                workoutName = input3.at(0);
                for (int i = 0; i < inputSize - 3; ++i) {
                    workoutName += " " + input3.at(i+1);
                }
                workoutType = input3.at(inputSize - 2);
                price = stoi(input3.at(inputSize - 1));
            }
            if (inputSize == 3){
                workoutName = input3.at(0);
                workoutType = input3.at(1);
                price = stoi(input3.at(2));
            }
            Workout workout = checkWorkoutType(counterWorkoutId, workoutName, price, workoutType);
            //save the id for trainer order
            workout_ids.push_back(counterWorkoutId);
            counterWorkoutId++;
            workout_options.push_back(workout);
        }
    }
    myFile.close();
}

bool Studio::isEmptyOrCommentLine(std::string line){
    int lineLength = line.length();
    if (line[0] == '#' || lineLength == 0)
        return true;
    return false;
}

Studio::Studio(const Studio &other) : open(other.open), counterCustomerId(other.counterCustomerId),
                                      counterTrainerId(other.counterTrainerId),
                                      counterWorkoutId(other.counterWorkoutId), trainers(),
                                      workout_ids(other.workout_ids), workout_options(), actionsLog() {
    int oSizeA = other.actionsLog.size();
    int oSizeW = other.workout_options.size();
    int oSizeT = other.trainers.size();
    for (int i = 0; i < oSizeW; ++i) {
        workout_options.push_back(other.workout_options.at(i));
    }
    for (int i = 0; i < oSizeT; ++i) {
        trainers.push_back(other.trainers.at(i)->clone());
    }
    for (int i = 0; i < oSizeA; ++i) {
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
}

Studio::Studio(Studio &&other) : open(other.open), counterCustomerId(other.counterCustomerId),
                                 counterTrainerId(other.counterTrainerId), counterWorkoutId(other.counterWorkoutId),
                                 trainers(), workout_ids(other.workout_ids), workout_options(), actionsLog() {
    int oSizeA = other.actionsLog.size();
    int oSizeW = other.workout_options.size();
    int oSizeT = other.trainers.size();
    if (this != &other) {
        open = other.open;
        workout_options.clear();
        for (int i = 0; i < oSizeW; ++i) {
            workout_options.push_back(other.workout_options.at(i));
        }
        trainers.clear();
        actionsLog.clear();
        for (int i = 0; i < oSizeT; i++) {
            trainers.push_back(other.trainers.at(i));
            other.trainers.at(i) = nullptr;
        }
        for (int i = 0; i < oSizeA; i++) {
            actionsLog.push_back(other.actionsLog.at(i));
            other.actionsLog.at(i) = nullptr;
        }
    }
    other.clear();
}


Workout Studio::checkWorkoutType(int WorkoutId, std::string woName, int price, std::string woType) {
    if (woType == "Anaerobic")
        return Workout(WorkoutId, woName, price, WorkoutType::ANAEROBIC);
    if (woType == "Mixed")
        return Workout(WorkoutId, woName, price, WorkoutType::MIXED);
    //if its Cardio
    return Workout(WorkoutId, woName, price, WorkoutType::CARDIO);
}

void Studio::start() {
    open = true;
    std::string userInput = "";
    std::cout << "Studio is now open!" << std::endl;

    std::getline(std::cin, userInput);
    while (userInput != "closeall") {
        std::vector<std::string> inputs = getUserInput(userInput);
        std::string action = inputs.at(0);
        if (action == "open") {
            int idTrainer = std::stoi(inputs.at(1));
            //get customers information from input
            std::vector<std::string> customerInputs;
            int sizeOfInput = inputs.size();
            for (int i = 2; i < sizeOfInput; ++i) {
                customerInputs.push_back(inputs.at(i));
            }
            //open trainer with customers types according to input
            std::vector<Customer *> customers;
            int numOfCusInput = customerInputs.size();
            for (int i = 0; i < numOfCusInput - 1; i = i + 2) {
                Customer *customer;
                if (customerInputs.at(i + 1) == "swt") {
                    customer = new SweatyCustomer(customerInputs.at(i), counterCustomerId);
                    customers.push_back(customer);
                }
                if (customerInputs.at(i + 1) == "chp") {
                    customer = new CheapCustomer(customerInputs.at(i), counterCustomerId);
                    customers.push_back(customer);
                }
                if (customerInputs.at(i + 1) == "mcl") {
                    customer = new HeavyMuscleCustomer(customerInputs.at(i), counterCustomerId);
                    customers.push_back(customer);
                }
                if (customerInputs.at(i + 1) == "fbd") {
                    customer = new FullBodyCustomer(customerInputs.at(i), counterCustomerId);
                    customers.push_back(customer);
                }
                counterCustomerId = counterCustomerId + 1;
            }
            int numOfCustomers = customers.size();
            if (numOfCustomers > 0) {
                OpenTrainer *open = new OpenTrainer(idTrainer, customers);
                open->act(*this);
                actionsLog.push_back(open);
            }
        }
        if (action == "order") {
            Order *order = new Order(stoi(inputs.at(1)));
            order->act(*this);
            actionsLog.push_back(order);
        }
        if (action == "move") {
            MoveCustomer *moveCustomer = new MoveCustomer(stoi(inputs.at(1)), stoi(inputs.at(2)), stoi(inputs.at(3)));
            moveCustomer->act(*this);
            actionsLog.push_back(moveCustomer);
        }
        if (action == "close") {
            Close *close = new Close(stoi(inputs.at(1)));
            close->act(*this);
            actionsLog.push_back(close);
        }
        if (action == "workout_options") {
            PrintWorkoutOptions *prWorkoutOptions = new PrintWorkoutOptions();
            prWorkoutOptions->act(*this);
            actionsLog.push_back(prWorkoutOptions);
        }
        if (action == "status") {
            PrintTrainerStatus *prTrainerStatus = new PrintTrainerStatus(stoi(inputs.at(1)));
            prTrainerStatus->act(*this);
            actionsLog.push_back(prTrainerStatus);
        }
        if (action == "log") {
            PrintActionsLog *prActionLog = new PrintActionsLog();
            prActionLog->act(*this);
            actionsLog.push_back(prActionLog);
        }
        if (action == "backup") {
            BackupStudio *backupStudio = new BackupStudio();
            actionsLog.push_back(backupStudio);
            backupStudio->act(*this);
        }
        if (action == "restore") {
            RestoreStudio *restoreStudio = new RestoreStudio();
            restoreStudio->act(*this);
            actionsLog.push_back(restoreStudio);
        }
        std::getline(std::cin, userInput);
    }
    CloseAll *closeAll = new CloseAll();
    closeAll->act(*this);
    delete closeAll;
}

// Read user input into vector of words (ignoring ' ' and ',')
std::vector<std::string> Studio::getUserInput(std::string input) {
    std::vector<std::string> userInput;
    std::string word = "";
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++) {
        if (input.at(i) != ' ' && input.at(i) != ',') {
            word += input[i];
            if (i == inputSize - 1) {
                userInput.push_back(word);
            }
        } else {
            if (!word.empty()) {
                userInput.push_back(word);
                word = "";
            }
        }
    }
    return userInput;
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    int numOfTrainers = trainers.size();
    for (int i = 0; i < numOfTrainers; ++i) {
        if (trainers.at(i)->getId() == tid) {
            return trainers.at(i);
        }
    }
    return nullptr;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

std::vector<Trainer *> &Studio::getTrainers() {
    return trainers;
}


Studio::~Studio() {
    int SizeA = actionsLog.size();
    int SizeT = trainers.size();
    for (int i = 0; i < SizeA; ++i) {
        delete actionsLog.at(i);
    }
    for (int i = 0; i < SizeT; ++i) {
        delete trainers.at(i);
    }
}

Studio &Studio::operator=(const Studio &other) {
    if (&other != this) {
        int SizeA = actionsLog.size();
        int SizeT = trainers.size();
        int oSizeA = other.actionsLog.size();
        int oSizeT = other.trainers.size();
        int oSozeW = other.workout_options.size();
        open = other.open;
        //delete trainers
        for (int i = 0; i < SizeT; ++i) {
            delete trainers.at(i);
        }
        trainers.clear();
        for (int i = 0; i < SizeA; ++i) {
            delete actionsLog.at(i);
        }
        actionsLog.clear();
        //copy actions
        for (int i = 0; i < oSizeA; ++i) {
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }
        //copy trainers
        for (int i = 0; i < oSizeT; ++i) {
            trainers.push_back(other.trainers.at(i)->clone());
        }
        workout_options.clear();
        for (int i = 0; i < oSozeW; ++i) {
            workout_options.push_back(other.workout_options.at(i).clone());
        }
    }
    return *this;
}

Studio &Studio::operator=(Studio &&other) {
    if (this != &other) {
        int SizeA = actionsLog.size();
        int SizeT = trainers.size();
        int oSizeA = other.actionsLog.size();
        int oSizeT = other.trainers.size();
        int oSizeW = other.workout_options.size();
        open = other.open;
        workout_options.clear();
        for (int i = 0; i < oSizeW; ++i) {
            workout_options.push_back(other.workout_options.at(i));
        }
        for (int i = 0; i < SizeT; ++i) {
            delete trainers.at(i);
        }
        for (int i = 0; i < SizeA; ++i) {
            delete actionsLog.at(i);
        }
        trainers.clear();
        actionsLog.clear();
        for (int i = 0; i < oSizeT; ++i) {
            trainers.push_back(other.trainers.at(i));
            other.trainers.at(i) = nullptr;
        }
        for (int i = 0; i < oSizeA; ++i) {
            actionsLog.push_back(other.actionsLog.at(i));
            other.actionsLog.at(i) = nullptr;
        }
    }
    other.clear();
    return *this;
}

void Studio::clear() {
    open = false;
    trainers.clear();
    actionsLog.clear();
    workout_options.clear();
}

std::string Studio::toString() const {
    std::string output = "";
    int numOfTrainers = trainers.size();
    for (int i = 0; i < numOfTrainers; i++) {
        output += trainers.at(i)->toString() + "  ";
    }
    return output;
}