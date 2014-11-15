#include "hand_math.h"

int iclamp(int x, int a, int b){
    if(x > b){
        return b;
    }
    if(x < a){
        return a;
    }
    return x;
}
