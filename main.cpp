//
//  main.cpp
//  Pebble_game
//
//  Created by 戴一通 on 9/23/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "global_data.hpp"
#include "gamePlay.hpp"

using namespace std;
int main(int argc, const char * argv[]) {
    // insert code here...
    char userChoice;
    int ply_input;
    int column_input;
    int numOfball_input;
    header.p = NULL;
    
    do{
        system("clear");
        cout<<"A. start game\nB. exit\n"<<endl;
        cout<<"Please enter A or B: "<<endl;
        cin>>userChoice;
        if(userChoice == 'A' || userChoice == 'a'){
            system("clear");
            cout<<"Please enter the number of initial pebble in every single square: "<<endl;
            cin>>numOfball_input;
            cout<<"Please enter the number of column: "<<endl;
            cin>>column_input;
            cout<<"Please enter the number of ply: "<<endl;
            cin>>ply_input;
            ply_limit = ply_input;
            GamePlay game(column_input, numOfball_input);
            game.initialize();
            game.play();
        }
    }while(userChoice != 'B' && userChoice != 'b');
    return 0;

}
