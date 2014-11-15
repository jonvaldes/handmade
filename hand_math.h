#pragma once

inline int imax(int x, int y){
    if(x>y){
        return x;
    }
    return y;
}

int iclamp(int x, int a, int b);

