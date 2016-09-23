//
//  struct_library.h
//  Pebble_game
//
//  Created by 戴一通 on 9/23/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#ifndef struct_library_h
#define struct_library_h
struct element{
    int numberOfball;
    int index;
};//used to improve the performance of pruning
struct linkedList{
    int state[20];
    linkedList *p;
};
#endif /* struct_library_h */
