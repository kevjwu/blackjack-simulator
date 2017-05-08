//
//  round.h
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#ifndef round_h
#define round_h

#include "shoe.h"
#include "hand.h"
#include "player.h"

class Round {
public:
    Shoe *shoe;
    DealerHand dealerhand;
    bool dealer_bj;
    bool continue_round;
    int maxHandSize = 2;
    Player *me;
    Round(Shoe *s, Player *p){
        shoe = s;
        me = p;
        me->hands.clear();
    }
    
    void display(){
        system("clear");
        cout << "DEALER \t\t" << "You\n";
        int cardsToShow = max(maxHandSize, (int) dealerhand.cards.size());
        for (int i=0; i<cardsToShow; i++){
            dealerhand.display(i);
            for (int j=0; j<me->hands.size(); j++){
                me->hands[j].display(i);
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
    void go(){
        continue_round=false;
        dealerhand = DealerHand(shoe->deal(), shoe->deal());
        dealer_bj = dealerhand.peek();
        Hand init_hand = Hand(shoe->deal(), shoe->deal());
        bool split = me->new_hand(init_hand);
        while (split==true){
            int key = 0;
            display();
            cout << "Press '1' to split, '2' to continue playing\n";
            cin >> key;
            if (key==1){
                split = user_split();
            }
        }
        
        display();
        if (dealer_bj==true && init_hand.hardscore==21){
            cout << "You and dealer both get blackjack. Push.\n";
            return;
        }
        else if (dealer_bj==true){
            cout << "Dealer got blackjack.\n";
            return;
        }
        
        else if (init_hand.hardscore==21){
            cout << "Blackjack!\n";
            shoe->undo_count(dealerhand.cards[1]);
            return;
        }
        else {
            continue_round = user_play();
            if (continue_round==false){
                shoe->undo_count(dealerhand.cards[1]);
                return;
            }
            
            cout << "....Dealer's turn....\n";
            dealer_play();
        }
    }
    
    bool user_split(){
        Hand hand1 = Hand(me->hands.back().cards[0], shoe->deal());
        Hand hand2 = Hand(me->hands.back().cards[1], shoe->deal());
        me->hands.pop_back();
        bool split1 = me->new_hand(hand1);
        bool split2 = me->new_hand(hand2);
        return split1 || split2;
    }
    
    bool user_play(){
        bool continue_play = false;
        for (vector<Hand>::iterator it = me->hands.begin() ; it != me->hands.end(); ++it)
        {
            int key = 0;
            bool stop = false;
            while (it->bestscore < 21 && stop==false) {
                cout << "Press '1' to hit, '2' to stay, '3' to double\n";
                cin >> key;
                if (key==1){
                    it->hit_me(shoe);
                }
                else if (key==2){
                    stop=true;
                }
                else if (key==3){
                    it->hit_me(shoe);
                    stop=true;
                }
                else {
                    cout << "Invalid key.\n";
                    continue;
                }
                if (it->cards.size() > maxHandSize){
                    maxHandSize = it->cards.size();
                }
                display();
            }
            if (it->bestscore > 21) {
                cout << "You busted\n";
            }
            else {
                continue_play = true;
            }

        }
        return continue_play;
    }
    
    void dealer_play(){
        this_thread::sleep_for(std::chrono::seconds(1));
        dealerhand.cards[1]->flip();
        display();
        this_thread::sleep_for(std::chrono::seconds(1));
        if (dealerhand.bestscore>=17){
            return;
        }
        else {
            bool stay = false;
            while (stay == false){
                this_thread::sleep_for(std::chrono::seconds(1));
                dealerhand.hit_me(shoe);
                display();
                if (dealerhand.bestscore>=17){
                    display();
                    stay = true;
                }
            }
        }
        display();
    }

    void get_result(){
        if (continue_round==false){
            return;
        }
        if (dealerhand.bestscore > 21){
            cout << "Dealer busted. You win!\n";
            return;
        }
        for (vector<Hand>::iterator it = me->hands.begin() ; it != me->hands.end(); ++it)
        {
            if (it->bestscore > dealerhand.bestscore){
                cout << "You win!\n";
            }
            else if (it->bestscore < dealerhand.bestscore){
                cout << "Dealer wins.\n";
            }
            else {
                cout << "It's a push.\n";
            }
        }
        return;
    }
};
    
    
#endif /* round_h */
