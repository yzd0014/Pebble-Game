//
//  gamePlay.hpp
//  Pebble_game
//
//  Created by 戴一通 on 9/23/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#ifndef gamePlay_hpp
#define gamePlay_hpp

class GamePlay{
private:
    int gamePad[20];
    int column;
    int numOfball;
public:
    GamePlay(int columnIn, int numOfballIn);
    void initialize();
    void display();
    void distribution(int action);
    void play();
    
};
#endif /* gamePlay_hpp */
