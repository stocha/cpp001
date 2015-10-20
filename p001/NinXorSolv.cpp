/* 
 * File:   NinXorSolv.cpp
 * Author: denis
 * 
 * Created on 18 octobre 2015, 17:11
 */

#include "NinXorSolv.h"

void NinXorSolv::test() {
    srand(0xCAFEBABE);
    
    int sz=8;
    bitField f(sz);
    f.randomize();f.randomize();f.randomize();f.randomize();f.randomize();
    cout << f.str() << endl;
    NinXorSolv a(sz,f);
    
    cout << a.debug();
    
}




