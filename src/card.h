//
//  card.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef card_h
#define card_h

#include <map>

using namespace std;

enum Rank {
    _A, _2, _3, _4, _5, _6, _7, _8, _9, _10, _J, _Q, _K
};

const char *rank_names[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

enum Suit {
    Heart, Diamond, Club, Spade
};


const char *suit_names[] = { "hart", "diam", "club", "spad" };

const map<Rank, int> RankValue {
    {_A, 11},
    {_2, 2},
    {_3, 3},
    {_4, 4},
    {_5, 5},
    {_6, 6},
    {_7, 7},
    {_8, 8},
    {_9, 9},
    {_10, 10},
    {_J, 10},
    {_Q, 10},
    {_K, 10}
};


class Card {
    
    Suit suit;
    
public:
    Rank rank;
    bool visible;
    
    Card(Rank r, Suit s, bool v) {
        rank = r;
        suit = s;
        visible = v;
    }
    
    void flip() {
        if (visible == true) { return; }
        else { visible = true; }
        return;
    }
    
    void show() {
        cout << rank_names[rank] << " " << suit_names[suit];
    }
    
};

#endif /* card_h */
