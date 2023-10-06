
/*
 * Name: Safin Rashid
 * EID: srr3288
 * PA7
 * Santacruz, Fall 2022
 */

#include <stdio.h>
#include "UTString.h"
#include "Customer.h"
#include "CustomerDB.h"
#include <iostream>

using namespace std;

void readString(UTString&);
void readNum(int&);

CustomerDB database;

int num_books = 0;
int num_dice = 0;
int num_figures = 0;
int num_towers = 0;

/* clear the inventory and reset the customer database to empty */
void reset(void) {
    database.clear();
    num_books = 0;
    num_dice = 0;
    num_figures = 0;
    num_towers = 0;
}

/*
 * selectInventItem is a convenience function that allows you
 * to obtain a pointer to the inventory record using the item type name
 * word must be "Books", "Dice", "Figures" or "Towers"
 * for example the expression *selectInventItem("Books") returns the
 * current global variable for the number of books in the inventory
 */
int* selectInventItem(UTString word) {
    if (word == "Books") {
        return &num_books;
    } else if (word == "Dice") {
        return &num_dice;
    } else if (word == "Figures") {
        return &num_figures;
    } else if (word == "Towers"){
        return &num_towers;
    }

    /* NOT REACHED */
    return 0;
}

/*
 * this overload of selectInventItem is similar to selectInventItem above, however
 * this overload takes a Customer as a second argument and selects the data member
 * of that Customer that matches "word". The function returns a pointer to one of
 * the three data members from the specified customer.
 */
int* selectInventItem(UTString word, Customer& cust) {
    if (word == "Books") {
        return &cust.books;
    } else if (word == "Dice") {
        return &cust.dice;
    } else if (word == "Figures") {
        return &cust.figures;
    } else if (word == "Towers"){
        return &cust.towers;
    }

    /* NOT REACHED */
    return 0;
}

/*
 * findMax searches through the CustomerDB "database" and returns the Customer
 * who has purchased the most items of the specified type.
 * type must be one of "Books", "Dice", "Figures" or "Towers".
 *
 * Note: if two or more Customers are tied for having purchased the most of that item type
 * then findMax returns the first Customer in the CustomerDB who has purchased that maximal
 * quantity.
 *
 * Note: in the special case (invalid case) where there are zero Customers in the
 * CustomerDB, fundMax returns a null pointer (0)
 */
Customer* findMax(UTString type) {
    Customer* result = 0;
    int max = 0;
    for (int k = 0; k < database.size(); k += 1) {
        Customer& cust = database[k];
        int *p = selectInventItem(type, cust);
        if (*p > max) {
            max = *p;
            result = &cust;
        }
    }

    return result;
}

void processPurchase() {
    //TODO
    UTString name;
    readString(name);

    UTString item2buy;
    readString(item2buy);

    int quant;
    readNum(quant);

    if(quant > 0) {
        int *sell = selectInventItem(item2buy);
        if (*sell < quant) {
            cout << "Sorry " << name.c_str() << ", we only have " << *sell << " " << item2buy.c_str() << endl;
            return;
        }
        *sell -= quant;
        *selectInventItem(item2buy, database[name]) += quant;
    }

}

void printSummary(UTString item) {

    if (database.size() == 0 || findMax(item) == 0) {
        cout << "no one has purchased any " << item.c_str() << endl;
        return;
    }

    cout << findMax(item)->name.c_str() << " has purchased the most " << item.c_str() <<
            " (" << *selectInventItem(item, *findMax(item)) << ")" << endl;

}

void processSummarize() {
    //TODO
    cout << "There are "<< num_books << " Books "
                        << num_dice << " Dice "
                        << num_figures << " Figures and "
                        << num_towers << " Towers in inventory" << endl;

    cout << "we have had a total of " << database.size() << " different customers" << endl;

    printSummary("Books");
    printSummary("Dice");
    printSummary("Figures");
    printSummary("Towers");

}

void processInventory() {
    // TODO
    UTString item_stock;
    readString(item_stock);

    int quant;
    readNum(quant);

    int* item = selectInventItem(item_stock);
    *item += quant;

}