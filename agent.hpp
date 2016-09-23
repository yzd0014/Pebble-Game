//
//  agent.hpp
//  Pebble_game
//
//  Created by 戴一通 on 9/23/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#ifndef agent_hpp
#define agent_hpp
#include "struct_library.h"
class agent{
private:
    void arrayCopy(int *array1, int *array2);
    element *order(int *gamePad, int column, int player);
    int compareState(int *input1, int *input2);
    int findState(int *state);
    void insert(linkedList *input);
    void delet();
public:
    double Heuristic(int *state, int column);
    int *result(int *gamePad, int action, int column);
    int ABS(int *gamePad, int column);
    double Max(int *gamePad, int depth, int A, int B, int column);
    double Min(int *gamePad, int depth, int A, int B, int column);
};

#endif /* agent_hpp */
