//
//  shoe.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef shoe_h
#define shoe_h

#include "card.h"

int myrandom (int i) { return std::rand()%i;}

class Shoe {
public:
    vector<Card*> cards;
    vector<Card*> discards;
    int running_count = 0;
    float true_count = 0;
    float decks_remaining;
    int discard_idx;
    int num_decks;
    int penetration_threshold;
    Shoe() {}
    Shoe(int n_decks, float p){
        num_decks = n_decks;
        penetration_threshold = p * 52;
        prep_cards();
    }
    
    Card* deal() {
        Card* top = cards.back();
        cards.pop_back();
        discards.push_back(top);
        refresh_count();
        return top;
    }
    
    void prep_cards(){
        for (int i = 0; i < num_decks; i++){
            for (Rank j = _A; j <= _K; j = Rank(j+1)){
                for (Suit k = Heart; k <= Spade; k = Suit(k+1)){
                    Card* c = new Card(j, k, false);
                    cards.push_back(c);
                }
            }
        }
        std::random_shuffle( cards.begin(), cards.end(), myrandom);
    };
    
    void refill_shoe(){
        cout << "New shoe!\n";
        for (vector<Card*>::iterator it = cards.begin() ; it != cards.end(); ++it)
        {
            delete (*it);
        }
        cards.clear();
        
        for (vector<Card*>::iterator it = discards.begin() ; it != discards.end(); ++it)
        {
            delete (*it);
        }
        discards.clear();
        discard_idx = 0;
        running_count = 0;
        true_count = 0;
        prep_cards();
    };
    
    void refresh_count(){
        for (int i=discard_idx; i < discards.size(); i++){
            
            if (RankValue.find(discards[i]->rank)->second >= 10){
                running_count--;
            }
            else if (RankValue.find(discards[i]->rank)->second <= 6){
                running_count++;
            }
            else {
                continue;
            }
            discard_idx = i;
        }
        decks_remaining = floor(((float) cards.size() / (float) 52) * 4 + 0.5)/4;
        true_count = floor(running_count / decks_remaining * 4 + 0.5)/4;
        discard_idx++;
    }
    
    void undo_count(Card* card){
        if (RankValue.find(card->rank)->second >= 10){
            running_count++;
        }
        else if (RankValue.find(card->rank)->second <= 6){
            running_count--;
        }
        else { }
        decks_remaining = floor(((float) cards.size() / (float) 52) * 4 + 0.5)/4;
        true_count = floor(running_count / decks_remaining * 4 + 0.5)/4;
    };
    
    
};

#endif /* shoe_h */
