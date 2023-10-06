/*
 * Name: Safin Rashid
 * EID: srr3288
 * PA7
 * Santacruz, Fall 2022
 */
#include <iostream>
#include <cassert>
#include "UTString.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 1;

Customer::Customer(UTString name) { // done, please do not edit
    this->books = 0;
    this->dice = 0;
    this->figures = 0;
    this->towers = 0;
    this->name = name;
}

CustomerDB::CustomerDB(void) { // done, please do not edit
    this->capacity = default_capacity;
    this->data = new Customer[this->capacity];
    this->length = 0;
}

int CustomerDB::size(void) { // done, please do not edit
    return this->length;
}

CustomerDB::~CustomerDB(void) { // done, please do not edit
    delete[] this->data;
}

void CustomerDB::clear(void) { // done, please do not edit
    delete[] this->data;
    this->capacity = default_capacity;
    this->data = new Customer[this->capacity];
    this->length = 0;
}

Customer& CustomerDB::operator[](int k) { // done, please do not edit
    assert(k >= 0 && k < this->length);
    return this->data[k];
}

Customer& CustomerDB::operator[](UTString name) {
    //TODO
    if(isMember(name)){//return customer reference
        for(int i = 0; i < this->length; i++) {
            if(this->data[i].name.operator==(name)) {
                return this->data[i];
            }
        }
    }else if(this->length == this->capacity) {
        this->capacity = this->capacity * 2;
        Customer* new_data = new Customer[this->capacity];
        for(int k = 0; k < this->length; k++) {
            new_data[k] = this->data[k];
        }
        delete[] this->data;
        this->data = new_data;
    }

    Customer new_cust(name);
    this->data[this->length] = new_cust;
    this->length++;
    return this->data[this->length - 1];

}

bool CustomerDB::isMember(UTString name) {
    //TODO
	for(int j = 0; j < this->length; j++){
        if(this->data[j].name == name) return true;
    }
    return 0;
}
