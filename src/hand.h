//
//  hand.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef hand_h
#define hand_h

#include "card.h"
#include "shoe.h"

class Hand {
public:
    vector<Card*> cards;
    int counter = 0;
    int aces = 0;
    int hardscore = 0;
    int bestscore = 0;
    Hand() {}
    
    Hand(Card* c1, Card* c2){
        c1->flip();
        c2->flip();
        cards.push_back(c1);
        cards.push_back(c2);
        calc_score();
    }
    
    void calc_score(){
        for (int i=counter; i<cards.size(); i++){
            if (cards[i]->rank == Rank(_A)){
                aces++;
            }
            hardscore += RankValue.find(cards[i]->rank)->second;
            bestscore += RankValue.find(cards[i]->rank)->second;
            counter = i;
        }
        counter++;
        while (bestscore > 21 and aces > 0){
            bestscore -= 10;
            aces--;
        }
    }
    
    void display(int i){
        if (cards.size() < i + 1){
            cout << "\t\t";
        }
        else {
            if (cards[i]->visible==true){
                cards[i]->show();
            }
            cout << "\t\t";
        }
        return;
    }
    
    void hit_me(Shoe *shoe){
        Card* hit = shoe->deal();
        hit->flip();
        cards.push_back(hit);
        calc_score();
    }
    
};



class DealerHand : public Hand {
public:
    DealerHand(){}
    DealerHand(Card* c1, Card* c2) {
        c1->flip();
        cards.push_back(c1);
        cards.push_back(c2);
    }
    
    bool peek(){
        calc_score();
        if (hardscore == 21){
            cards[1]->flip();
            return true;
        }
        return false;
    }
};



#endif /* hand_h */
