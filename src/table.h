//
//  table.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef table_h
#define table_h

#include "shoe.h"
#include "round.h"

class Table {
public:
    Shoe shoe;
    Table(){
        shoe = Shoe(8, 2);
    }
    void start(){
        bool exit = false;
        int i = 0;
        while (exit==false){
            Round round = Round(&shoe);
            round.go();
            this_thread::sleep_for(std::chrono::seconds(1));
            round.get_result();
            this_thread::sleep_for(std::chrono::seconds(2));
            if (i % 5 == 0){
                cout << "====================\n";
                cout << "=== RUNNING COUNT:" << shoe.running_count << "\n";
                cout << "=== DECKS REMAINING:" << shoe.decks_remaining << "\n";
                cout << "=== TRUE COUNT:" << shoe.true_count << "\n";
                cout << "====================\n";
                this_thread::sleep_for(std::chrono::seconds(1));
            }
            if (shoe.cards.size() <= shoe.penetration_threshold){
                cout << "====================\n";
                cout << "RUNNING COUNT:" << shoe.running_count << "\n";
                cout << "DECKS REMAINING:" << shoe.decks_remaining << "\n";
                cout << "TRUE COUNT:" << shoe.true_count << "\n";
                cout << "====================\n";
                this_thread::sleep_for(std::chrono::seconds(1));
                shoe.refill_shoe();
            }
            this_thread::sleep_for(std::chrono::seconds(1));
            i++;
        }
    }
    
};

#endif /* table_h */

