//
//  gamePlay.cpp
//  Pebble_game
//
//  Created by 戴一通 on 9/23/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#include "gamePlay.hpp"
#include <iostream>
#include "agent.hpp"
using namespace std;
GamePlay::GamePlay(int columnIn, int numOfballIn){
    column = columnIn;
    numOfball=numOfballIn;
}

void GamePlay::initialize(){//initialize the gameboard
    for(int i=0;i<20;i++){
        gamePad[i]=0;
    }
    for(int i=0; i<2*column; i++){
        gamePad[i] = numOfball;
    }
}

void GamePlay::display(){//display the current gameboard
    for(int i=0; i<column; i++){
        printf("%d: %d\t", i, gamePad[i]);
    }
    cout<<"\n"<<endl;
    for(int i=column; i<2*column;i++){
        printf("%d: %d\t", i, gamePad[i]);
    }
    cout<<"\n"<<endl;
}
void GamePlay::distribution(int action){//get the new state according to the action and current state
    int box = gamePad[action];//this one will really change the gameboard rather than like reuslt function
    gamePad[action]=0;
    if(action<column){
        for(int i=action+1; i<column;i++){
            if(box>0){
                box=box-1;
                gamePad[i]=gamePad[i]+1;
            }
            else{break;}
        }
        while(box>0){
            for(int i=2*column-1; i>=column; i--){
                if(box>0){
                    box=box-1;
                    gamePad[i]=gamePad[i]+1;
                }
                else{break;}
            }
            
            for(int i=0; i<column; i++){
                if(box>0){
                    box=box-1;
                    gamePad[i]=gamePad[i]+1;
                }
                else {break;}
            }
            
        }
    }
    else{
        for(int i=action-1;i>=column;i--){
            if(box>0){
                box=box-1;
                gamePad[i]=gamePad[i]+1;
            }
            else{
                break;
            }
        }
        while(box>0){
            for(int i=0; i<column;i++){
                if(box>0){
                    box =box-1;
                    gamePad[i]=gamePad[i]+1;
                }
                else{break;}
            }
            for(int i=2*column-1;i>=column;i--){
                if(box>0){
                    box = box-1;
                    gamePad[i]=gamePad[i]+1;
                }
                else{break;}
            }
        }
    }
    
}

void GamePlay::play(){
    agent robot;
    int userMove;
    int robotMove;
    
    system("clear");
    display();
    while(true){
        //user's turn
        do{
            cout<<"Enter which square you want to move: "<<endl;
            cin>>userMove;
        }while(userMove < column || userMove > 2 * column - 1 || gamePad[userMove] == 0);
        getchar();
        distribution(userMove);
        system("clear");
        display();
        if(robot.Heuristic(gamePad, column)==1){
            cout<<"You lose!"<<endl;
            break;
        }
        else if(robot.Heuristic(gamePad, column)==0){
            cout<<"You win!"<<endl;
            break;
        }
        //agent's turn
        robotMove = robot.ABS(gamePad, column);
        distribution(robotMove);
        system("clear");
        display();
        if(robot.Heuristic(gamePad, column)==1){
            cout<<"You lose!"<<endl;
            break;
        }
        else if(robot.Heuristic(gamePad, column)==0){
            cout<<"You win!"<<endl;
            break;
        }
    }
    cout<<"Press any button to return menu"<<endl;
    getchar();
}
