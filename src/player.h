//
//  player.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef player_h
#define player_h

class Player {
public:
    vector<Hand> hands;
    int bankroll;
    Shoe *shoe;
    int maxHandSize = 2;
    
    Player () {}
    Player(Shoe *s) {
        shoe = s;
    }
    
    
    bool new_hand(Hand &hand){
        hands.push_back(hand);
        if (hand.cards[0]->rank == hand.cards[1]->rank){
            return true;
        }
        return false;
    }
    
    void split(){
        Hand hand1 = Hand(hands.back().cards[0], shoe->deal());
        Hand hand2 = Hand(hands.back().cards[1], shoe->deal());
        hands.pop_back();
        new_hand(hand1);
        new_hand(hand2);
    }
    
};

#endif /* player_h */
