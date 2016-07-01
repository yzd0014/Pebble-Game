//
//  main.cpp
//  AI game
//
//  Created by 戴一通 on 11/27/14.
//  Copyright (c) 2014 ___FULLUSERNAME__. All rights reserved.
//

#include <iostream>
#include <curses.h>
#include <unistd.h>
using namespace std;
int ply_limit;

struct element{
    int numberOfball;
    int index;
};//used to improve the performance of pruning

struct linkedList{
    int state[20];
    linkedList *p;
} header;

class agent{
private:
    void arrayCopy(int *array1, int *array2){
        for(int i=0; i<20; i++){
            array1[i]=array2[i];
        }
    }
    element *order(int *gamePad, int column, int player){//build action's order to implement A and B pruning
        element *output=(element *)malloc(sizeof(element)*column), t;
        if(player == 0){
           for(int i=0; i<column; i++){
              output[i].numberOfball = gamePad[i];
              output[i].index = i;
           }//initialize array
        }
        else{//initialize array which is copy data from gameboard to this array
            int j=0;
            for(int i=column; i<2*column; i++){
                output[j].numberOfball = gamePad[i];
                output[j].index = i;
                j++;
            }
        }
        for(int i=0; i<column-1; i++){//sorting algorithm according to the number of pebbles in every square
            for(int j=0; j<column-1-i; j++){
                if(output[j].numberOfball>output[j+1].numberOfball){
                    t=output[j];
                    output[j]=output[j+1];
                    output[j+1]=t;
                }
            }
        }
        return output;//return the array
    }
    
    int compareState(int *input1, int *input2){
        int output = 1;
        for(int i=0; i<20;i++){
            if(input1[i]!= input2[i]){
                output = 0;
                break;
            }
        }
        return output;
    }
    int findState(int *state){
        int output = 0;
        linkedList *p = header.p;
        while(p!=NULL){
            if(compareState(p->state, state)==1){
                output = 1;
                break;
            }
            p=p->p;
        }
        return output;
    }
    void insert(linkedList *input){
        linkedList *p = header.p;
        while(p->p!=NULL){
            p=p->p;
        }
        p->p = input;
        input->p = NULL;
    }
    
    void delet(){
        linkedList *p = header.p;
        while(p->p->p!=NULL){
            p=p->p;
        }
        free(p->p);
        p->p = NULL;
    }
    
public:
    double Heuristic(int *state, int column){//heuristic function
        double utility=0;
        double sum = 0;
        double agent_sum = 0;
        for(int i=0; i<2*column; i++){//calculate the total pebbles of the gameboard
            sum = sum + state[i];
        }
        for(int i=0; i<column; i++){//calculate the number of pebbles that my agent has
            agent_sum = agent_sum + state[i];
        }
        utility = agent_sum/sum;
        return utility;
    }
    int *result(int *gamePad, int action, int column){//calculate new state according to the action and current state
        int *result = (int *)malloc(sizeof(int)*20);
        for(int i=0;i<20;i++){
            result[i]=0;
        }
        arrayCopy(result, gamePad);
        
        int box = result[action];
        result[action]=0;
        if(action<column){
            for(int i=action+1; i<column;i++){
                if(box>0){
                    box=box-1;
                    result[i]=result[i]+1;
                }
                else{break;}
            }
            while(box>0){
                for(int i=2*column-1; i>=column; i--){
                    if(box>0){
                        box=box-1;
                        result[i]=result[i]+1;
                    }
                    else{break;}
                }
                
                for(int i=0; i<column; i++){
                    if(box>0){
                        box=box-1;
                        result[i]=result[i]+1;
                    }
                    else {break;}
                }
                
            }
        }
        else{
            for(int i=action-1;i>=column;i--){
                if(box>0){
                    box=box-1;
                    result[i]=result[i]+1;
                }
                else{
                    break;
                }
            }
            while(box>0){
                for(int i=0; i<column;i++){
                    if(box>0){
                        box =box-1;
                        result[i]=result[i]+1;
                    }
                    else{break;}
                }
                for(int i=2*column-1;i>=column;i--){
                    if(box>0){
                        box = box-1;
                        result[i]=result[i]+1;
                    }
                    else{break;}
                }
            }
        }
        //this method won't change the orignal gameboard and will return a new gameboard
        return result;
    }
    int ABS(int *gamePad, int column){//alpha-beta minmax algorithm
        int action = 0;
        int currentSmall=0;//used to hold current square that contain smallest number of pebbles
        int *state = (int *)malloc(sizeof(int)*20);
        for(int i=0; i<20; i++){
            state[i]=0;
        }
        arrayCopy(state, gamePad);
        double A = -10;
        double B = 10;
        double output=0;
        
        double utility = -10;//-10 stands for negative infinity
        int current_Depth = 0;
        
        linkedList *p=(linkedList *)malloc(sizeof(linkedList));
        arrayCopy(p->state, gamePad);
        p->p = header.p;
        header.p = p;
        
        for(int i=0; i<column; i++){
            if(state[i]!=0){//make sure this action is legal
                if((output=Min(result(state, i, column), current_Depth+1, A, B, column))>utility||
                   (output==utility&&state[i]<currentSmall)){
                    utility = output;
                    action = i;
                    currentSmall = state[i];
                }
            }
        }
        free(state);
        return action;
    }
    double Max(int *gamePad, int depth, int A, int B, int column){//max node picks up the maxmum one
        double utility = -10;
        double output=0;
        int action=0;
        
        if(Heuristic(gamePad, column)==0||Heuristic(gamePad, column)==1||depth==ply_limit||findState(gamePad)==1){
            return Heuristic(gamePad, column);
        }
        element *action_line;
        action_line = order(gamePad, column, 0);
        
        linkedList *p=(linkedList *)malloc(sizeof(linkedList));
        arrayCopy(p->state, gamePad);
        insert(p);
        
        for(int i=0; i<column; i++){
            action = action_line[i].index;
            if(gamePad[action]>0){
                if((output=Min(result(gamePad, action, column), depth+1, A, B, column))>utility){
                    utility = output;

                 }
                if(utility >= B){return utility;}
                if(utility > A){
                    A = utility;
                }
            }
        }
        delet();
        free(gamePad);
        free(action_line);
        return utility;
    }
    double Min(int *gamePad, int depth, int A, int B, int column){//min node picks up the minmum one
        double utility = 10;
        double output=0;
        int action=0;
        
        if(Heuristic(gamePad, column)==0||Heuristic(gamePad, column)==1||depth==ply_limit||findState(gamePad)==1){
            return Heuristic(gamePad, column);
        }
        element *action_line;
        action_line = order(gamePad, column, 1);
        
        linkedList *p=(linkedList *)malloc(sizeof(linkedList));
        arrayCopy(p->state, gamePad);
        insert(p);
        
        for(int i=0; i<column; i++){
            action = action_line[i].index;
            if(gamePad[action]>0){
                if((output=Max(result(gamePad, action, column), depth+1, A, B, column))<utility){
                    utility = output;
                }
            }
            if(utility<=A){return  utility;}
            if(utility < B){
                B = utility;
            }
        }
        
        delet();
        free(gamePad);
        free(action_line);
        return utility;
    }
};

class GamePlay{
private:
    int gamePad[20];
    int column;
    int numOfball;
public:
    GamePlay(int columnIn, int numOfballIn){
        column = columnIn;
        numOfball=numOfballIn;
    }
    void initialize(){//initialize the gameboard
        for(int i=0;i<20;i++){
            gamePad[i]=0;
        }
        for(int i=0; i<2*column; i++){
            gamePad[i] = numOfball;
        }
    }
    void display(){//display the current gameboard
        for(int i=0; i<column; i++){
            printf("%d: %d\t", i, gamePad[i]);
        }
        cout<<"\n"<<endl;
        for(int i=column; i<2*column;i++){
            printf("%d: %d\t", i, gamePad[i]);
        }
        cout<<"\n"<<endl;
    }
    void distribution(int action){//get the new state according to the action and current state
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
    void play(){
        agent robot;
        int userMove;
        int robotMove;
        
        system("clear");
        display();
        while(true){
            //user's turn
            cout<<"Enter which square you want to move: "<<endl;
            cin>>userMove;
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
    
};

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
        if(userChoice == 'A'){
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
    }while(userChoice != 'B');
    return 0;
}
