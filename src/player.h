//
//  player.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef player_h
#define player_h

#include "card.h"

class Player {
public:
    vector<Hand> hands;
    Shoe *shoe;
    int maxHandSize = 2;
    Card *showCard;
    
    Player () {}
    Player(Shoe *s) {
        shoe = s;
    }
    
    void start_round(Card *c){
        showCard = c;
    }
    
    void display(){
        system("clear");
        cout << "DEALER \t\t" << "You\n";
        for (int i=0; i<2; i++){
            if (i==0){
                showCard->show();
            }
            cout << "\t\t";
            for (int j=0; j<hands.size(); j++){
                hands[j].display(i);
            }
            cout << "\n";
        }
        cout << "\n";
        
    }
    
    void check_split(){
        display();
        string message_indent = "";
        for (int i=0; i<hands.size(); ++i){
            int key;
            message_indent += "\t\t";
            if (hands[i].cards[0]->rank == hands[i].cards[1]->rank){
                display();
                cout << message_indent<<"Press '1' to split, '2' to continue playing\n";
                cin >> key;
                if (key==1){
                    split(i);
                    return;
                }
            }
        }
        return;
    }
    
    void split(int i){
        Hand hand1 = Hand(hands[i].cards[0], shoe->deal());
        Hand hand2 = Hand(hands[i].cards[1], shoe->deal());
        hands.erase(hands.begin() + i);
        hands.push_back(hand1);
        hands.push_back(hand2);
        check_split();
    }
    
};

#endif /* player_h */
