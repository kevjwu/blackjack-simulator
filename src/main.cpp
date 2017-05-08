//
//  main_new.cpp
//  blackjack-simulator
//
//  Created by Kevin Wu on 5/5/17.
//  Copyright © 2017 Kevin Wu. All rights reserved.
//

#include <stdio.h>

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
#include <thread>
#include <chrono>
#include <unordered_set>

#include "table.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Welcome to KWu's Blackjack Simulation. Press ENTER to start\n";
    cin.ignore();
    srand(time(NULL));
    Table table = Table();
    table.start();
    
    return 0;
}
