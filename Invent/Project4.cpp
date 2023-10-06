/* 
 * EE312 Simple CRM Project
 *
 * Fall 2022 Santacruz
 * srr3288
 * Safin Rashid
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS];
int num_customers = 0;

int num_books = 0, num_dice = 0, num_figures = 0, num_towers = 0;
String books, dice, figures, towers;

char itemSearch(String* item){

    books = StringCreate("Books");
    dice = StringCreate("Dice");
    figures = StringCreate("Figures");
    towers = StringCreate("Towers");

    char ret = 0;
    if(StringIsEqualTo(item, &books)){ret = 'b';}
    else if(StringIsEqualTo(item, &dice)){ret = 'd';}
    else if(StringIsEqualTo(item, &figures)){ret = 'f';}
    else if(StringIsEqualTo(item, &towers)){ret = 't';}

    StringDestroy(&books);StringDestroy(&dice);StringDestroy(&figures);StringDestroy(&towers);

    return ret; //returns 0 if not an item, or letter of item
}

/* clear the inventory and reset the customer database to empty */
void reset(void) {

    num_books = 0;num_dice = 0;num_figures = 0;num_towers = 0;
    for(int i = 0; i < num_customers; i++) {
        StringDestroy(&customers[i].name);
        customers[i].books = 0;customers[i].dice = 0;customers[i].figures = 0;customers[i].towers = 0;
    }
    num_customers = 0;
}

void processInventory() {

    String readItem;
    int inStock;
    readString(&readItem);
    readNum(&inStock);

    if(inStock<0){
        StringDestroy(&readItem); return; //invalid,negative stock number
    }
    char check = itemSearch(&readItem);

    if(check == 'b'){num_books = inStock;}
    else if(check == 'd'){num_dice = inStock;}
    else if(check == 'f'){num_figures = inStock;}
    else if(check == 't'){num_towers = inStock;}
    else{printf("Invalid Item\n");}
    StringDestroy(&readItem); //free
}

//possible flaw
int customerSearch(String* name){

    if(num_customers == 0){return -1;}
    if(num_customers) {
        int index = -1;
        for (int i = 0; i < num_customers; i++) {
            if (StringIsEqualTo(&customers[i].name, name)) {
                index = i;
            }
        }
        return index;
    }else{return -1;}
}

void processPurchase() {
    int quant;
    String name, item;

    readString(&name);//name = customer name
    readString(&item);//find item
    readNum(&quant);  //find item quantity

    char check = itemSearch(&item);
    StringDestroy(&item); //free checked string

    if(quant < 0){ //invalid/negative purchase quantity lmao
        StringDestroy(&name);
        return;
    }

    //see if customer exists
    int index = customerSearch(&name), retcode = 0;

    //error: requested ammount higher than inventory
    if(check == 'b' and num_books < quant){ //item is books and quant is high
        printf("Sorry "); StringPrint(&name); printf(", we only have "); //print out errors
        printf("%d Books\n", num_books); StringDestroy(&name); retcode = 1;
    }else if(check == 'd' and num_dice < quant){
        printf("Sorry "); StringPrint(&name); printf(", we only have ");
        printf("%d Dice\n", num_dice); StringDestroy(&name); retcode = 1;
    }else if(check == 'f' and num_figures < quant){
        printf("Sorry "); StringPrint(&name); printf(", we only have ");
        printf("%d Figures\n", num_figures); StringDestroy(&name); retcode = 1;
    }else if(check == 't' and num_towers < quant){
        printf("Sorry "); StringPrint(&name); printf(", we only have ");
        printf("%d Towers\n", num_towers); StringDestroy(&name); retcode = 1;
    }if(retcode){return;}

    //if new customer, add new customer
    if(index < 0){
        num_customers++; //yay new cu$tomer
        index = num_customers - 1;//manually set index for new customer(at end)
        customers[index].name = name;
    }else{StringDestroy(&name);}//free name

    //make purchase, update inventory and customer profile
    if(check == 'b'){
        customers[index].books = customers[index].books + quant;    //add to customer
        num_books -= quant;                                         //remove from inventory
    }else if(check == 'd'){
        customers[index].dice = customers[index].dice + quant;
        num_dice -= quant;
    }else if(check == 'f'){
        customers[index].figures = customers[index].figures + quant;
        num_figures -= quant;
    }else if(check == 't'){
        customers[index].towers = customers[index].towers + quant;
        num_towers -= quant;
    }
}

void processSummarize() {

    printf("There are %d Books %d Dice %d Figures and %d Towers in inventory\n", num_books, num_dice, num_figures, num_towers);
    printf("we have had a total of %d different customers\n", num_customers); //DISPLAYS INCORRECT NUM_CUSTOMERS (1)

    int maxBooks = 0, maxDice = 0, indexBooks = -1, indexDice = -1,
    maxFigures = 0, maxTowers = 0, indexFigures = -1, indexTowers = -1;

    //print books record
    for(int b = 0; b < num_customers; b++){
        if(customers[b].books > maxBooks){
            indexBooks = b;
            maxBooks = customers[b].books;
        }
    }
    if(indexBooks >= 0) {
        StringPrint(&customers[indexBooks].name);
        printf(" has purchased the most Books (%d)\n", maxBooks);
    }else{printf("no one has purchased any Books\n");}

    //print dice record
    for(int d = 0; d < num_customers; d++){
        if(customers[d].dice > maxDice){
            indexDice = d;
            maxDice = customers[d].dice;
        }
    }
    if(indexDice >= 0) {
        StringPrint(&customers[indexDice].name);
        printf(" has purchased the most Dice (%d)\n", maxDice);
    }else{printf("no one has purchased any Dice\n");}

    //print figures record
    for(int f = 0; f < num_customers; f++){
        if(customers[f].figures > maxFigures){
            maxFigures = customers[f].figures;
            indexFigures = f;
        }
    }
    if(indexFigures >= 0) {
        StringPrint(&customers[indexFigures].name);
        printf(" has purchased the most Figures (%d)\n", maxFigures);
    }else{printf("no one has purchased any Figures\n");}

    //print towers record
    for(int t = 0; t < num_customers; t++){
        if(customers[t].towers > maxTowers){
            indexTowers = t;
            maxTowers = customers[t].towers;
        }
    }
    if(indexTowers >= 0) {
        StringPrint(&customers[indexTowers].name);
        printf(" has purchased the most Towers (%d)\n", maxTowers);
    }else{printf("no one has purchased any Towers\n");}
}

/*
==222397==
==222397== HEAP SUMMARY:
==222397==     in use at exit: 0 bytes in 0 blocks
==222397==   total heap usage: 296 allocs, 296 frees, 3,958 bytes allocated
==222397==
==222397== All heap blocks were freed -- no leaks are possible
==222397==
==222397== For lists of detected and suppressed errors, rerun with: -s
==222397== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
srashid@mario (~) %
*/