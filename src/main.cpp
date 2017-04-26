//
//  main.cpp
//  blackjack-simulator
//
//  Created by Kevin Wu on 4/23/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <chrono>
#include <unordered_set>
using namespace std;

enum Rank {
    _A, _2, _3, _4, _5, _6, _7, _8, _9, _10, _J, _Q, _K
};

const char *rank_names[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

enum Suit {
    Heart, Diamond, Club, Spade
};


const char *suit_names[] = { "heart", "diamond", "club", "spade" };

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
        if (visible == true) { cout << "Error. Card already face-up."; return; }
        else { visible = true; }
        return;
    }
    
    void show() {
        cout << rank_names[rank] << " " << suit_names[suit] << "\n";
    }
    
};

int myrandom (int i) { return std::rand()%i;}

class Shoe {
public:
    vector<Card*> cards;
    vector<Card*> discards;
    int running_count;
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
    };
    
    
};

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
    
    void display(){
        for (int i=0; i<cards.size(); i++){
            if (cards[i]->visible==true){
                cards[i]->show();
            }
        }
        cout << "\n";
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
            display();
            return true;
        }
        display();
        return false;
    }
    
    // Dealer stands on all 17s
    void play(Shoe *shoe){
        cards[1]->flip();
        display();
        this_thread::sleep_for(std::chrono::seconds(1));
        if (bestscore>=17){
            return;
        }
        else {
            bool stay = false;
            while (stay == false){
                this_thread::sleep_for(std::chrono::seconds(1));
                hit_me(shoe);
                if (bestscore>=17){
                    stay = true;
                }
            }
        }
        display();
    }
};

class MyHand : public Hand {
public:
    MyHand() {}
    MyHand(Card* c1, Card* c2) : Hand(c1, c2){
        display();
    }
    bool play(Shoe *shoe){
        int key = 0;
        while (bestscore < 21 && key!=2) {
            cout << "Press '1' to hit, '2' to stay\n";
            cin >> key;
            if (key==1){
                hit_me(shoe);
            }
            display();
        }
        if (bestscore > 21) {
            return true;
        }
        return false;
    }
};

class Round {
public:
    Shoe *shoe;
    DealerHand dealerhand;
    MyHand myhand;
    bool me_bust;
    bool dealer_bj;
    Round(Shoe *s){
        shoe = s;
    }
    
    void go(){
        cout << "Dealer: \n";
        dealerhand = DealerHand(shoe->deal(), shoe->deal());
        dealer_bj = dealerhand.peek();
        if (dealer_bj==true){
            return;
        }
        cout << "Your:\n";
        
        myhand = MyHand(shoe->deal(), shoe->deal());
        me_bust = myhand.play(shoe);
        if (me_bust==true){
            shoe->undo_count(dealerhand.cards[1]);
            return;
        }
        cout << "Dealer's turn:\n";
        dealerhand.play(shoe);
    }
    
    void get_result(){
        if (dealer_bj==true){
            cout << "Dealer got blackjack.\n";
            return;
        }
        if (me_bust==true){
            cout << "You busted.\n";
            return;
        }
        else {
            if (dealerhand.bestscore > 21){
                cout << "Dealer busted. You win!\n";
            }
            else if (myhand.bestscore>dealerhand.bestscore){
                cout << "You win!\n";
            }
            else if (myhand.bestscore<dealerhand.bestscore){
                cout << "Dealer wins.\n";
            }
            else {
                cout << "It's a push.\n";
            }
        }
        return;
    };
};

class Table {
public:
    Shoe shoe;
    Table(){
        shoe = Shoe(1, 0.75);
    }
    void start(){
        bool exit = false;
        while (exit==false){
            Round round = Round(&shoe);
            round.go();
            this_thread::sleep_for(std::chrono::seconds(1));
            round.get_result();
            this_thread::sleep_for(std::chrono::seconds(2));
            cout << "====================\n";
            cout << "=== COUNT:" << shoe.running_count << " ======\n";
            cout << "====================\n";
            if (shoe.cards.size() <= shoe.penetration_threshold){
                shoe.refill_shoe();
            }
            this_thread::sleep_for(std::chrono::seconds(2));
            
        }
    }
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Welcome to KWu's Blackjack Simulation. Press ENTER to start\n";
    cin.ignore();
    srand(time(NULL));
    Table table = Table();
    table.start();
    
    return 0;
}
