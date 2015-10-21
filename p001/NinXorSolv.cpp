/* 
 * File:   NinXorSolv.cpp
 * Author: denis
 * 
 * Created on 18 octobre 2015, 17:11
 */

#include "NinXorSolv.h"

void NinXorSolv::test() {
    srand(0xCAFEBABE);
    
    int sz=4;
    bitField f(sz);
    f.randomize();f.randomize();f.randomize();f.randomize();f.randomize();
    f.set(3,0);
    cout << f.str() << endl;
    NinXorSolv a(sz,f);
    
    cout << a.str()<< endl;
    
    int un=a.unbound();
    
    auto b=NinXorSolv(a);
    cout << "forcing " << un << endl;
    b.forceAt(un,true);
    
    auto c=NinXorSolv(a);
    c.forceAt(un,false);
    
    cout <<"------- true" << endl;
    cout << b.str()<< endl;
    cout << b.strbound()<< endl;
    
    cout <<"------- false" << endl;
    cout << c.str()<< endl; 
    cout << c.strbound()<< endl; 
    
}




