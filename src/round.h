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
    Player me;
    Round() {}
    Round(Shoe *s){
        shoe = s;
        me = Player(s);
    }
    
    void display(){
        system("clear");
        cout << "DEALER \t\t" << "You\n";
        int cardsToShow = max(maxHandSize, (int) dealerhand.cards.size());
        for (int i=0; i<cardsToShow; i++){
            dealerhand.display(i);
            for (int j=0; j<me.hands.size(); j++){
                me.hands[j].display(i);
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
    void go(){
        continue_round=false;
        dealerhand = DealerHand(shoe->deal(), shoe->deal());
        me.start_round(dealerhand.cards[0]);
        dealer_bj = dealerhand.peek();
        Hand init_hand = Hand(shoe->deal(), shoe->deal());
        me.hands.push_back(init_hand);
        me.check_split();
        if (dealer_bj==true && init_hand.hardscore==21){
            display();
            cout << "You and dealer both get blackjack. Push.\n";
            return;
        }
        else if (dealer_bj==true){
            display();
            cout << "Dealer got blackjack.\n";
            return;
        }
        
        else if (init_hand.hardscore==21){
            display();
            cout << "Blackjack!\n";
            shoe->undo_count(dealerhand.cards[1]);
            return;
        }
        else {
            display();
            continue_round = user_play();
            if (continue_round==false){
                shoe->undo_count(dealerhand.cards[1]);
                return;
            }
            
            cout << "....Dealer's turn....\n";
            dealer_play();
        }
        
    }
    
    bool user_play(){
        bool continue_play = false;
        string message_indent = "";
        for (vector<Hand>::iterator it = me.hands.begin() ; it != me.hands.end(); ++it)
        {
            message_indent += "\t\t";
            int key = 0;
            bool stop = false;
            bool can_double = true;
            while (it->bestscore < 21 && stop==false) {
                if (can_double==true){
                    cout <<  message_indent << "Press '1' to hit, '2' to stay, '3' to double\n";
                    cin >> key;
                    if (key==1){
                        it->hit_me(shoe);
                        can_double=false;
                    }
                    else if (key==2){
                        stop=true;
                    }
                    else if (key==3){
                        it->hit_me(shoe);
                        stop=true;
                    }
                    else {
                        cout <<  message_indent << "Invalid key.\n";
                        continue;
                    }
                }
                else {
                    display();
                    cout <<  message_indent << "Press '1' to hit, '2' to stay\n";
                    cin >> key;
                    if (key==1){
                        it->hit_me(shoe);
                        display();
                        can_double=false;
                    }
                    else if (key==2){
                        stop=true;
                    }
                    else {
                        cout <<  message_indent << "Invalid key.\n";
                        continue;
                    }
                }
                if (it->cards.size() > maxHandSize){
                    maxHandSize = it->cards.size();
                }
            }
            display();
            if (it->bestscore > 21) {
                cout <<  message_indent <<"BUST\n";
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
        string message_indent = "";
        for (vector<Hand>::iterator it = me.hands.begin() ; it != me.hands.end(); ++it)
        {
            message_indent += "\t\t";
            if (it->bestscore>21){
                cout <<  message_indent<<"BUST\n";
            }
            if (it->bestscore > dealerhand.bestscore){
                cout <<  message_indent<<"You win!\n";
            }
            else if (it->bestscore < dealerhand.bestscore){
                cout <<  message_indent<<"Dealer wins.\n";
            }
            else {
                cout <<  message_indent<<"It's a push.\n";
            }
        }
        return;
    }
};


#endif /* round_h */
