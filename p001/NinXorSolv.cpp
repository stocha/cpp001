/* 
 * File:   NinXorSolv.cpp
 * Author: denis
 * 
 * Created on 18 octobre 2015, 17:11
 */

#include "NinXorSolv.h"

class xorInvert : public inverterInterface {

    struct myclass001 {

        bool operator()(bitField a, bitField b) {
            for (int i = 0; i < a.size(); i++) {
                int ind = a.size() - i - 1;
                if (a[ind] < b[ind]) return true;
                else if (a[ind] > b[ind]) return false;
            }
            return true;
        }
    } compareBits;
public:

    vector<bitField> invert(bitField in) {
        NinXorSolv a(in.size(), in);


        auto res = a.sat();

        sort(res.begin(), res.end(), compareBits);
        return res;
    }
};

void test2() {
    srand(0xCAFEBABE);

    int sz = 8;
    bitField f(sz);
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.set(3, 0);

    f = bitField(8);
    f.set(0, 0);
    f.set(1, 1);
    f.set(2, 0);
    f.set(3, 1);
    f.set(4, 1);
    f.set(5, 1);
    f.set(6, 1);
    f.set(7, 0);


    //0010.1100

    cout << f.str() << endl;
    NinXorSolv a(sz, f);

    cout << a.str() << endl;


    cout << "-----------------" << endl;
    vector<bitField> invr = a.sat();

    cout << invr.size() << " results found" << endl;


    for (auto b : invr) {
        cout << b.str() << endl;
    }

}

void oldTest() {
    srand(0xCAFEBABE);

    int sz = 4;
    bitField f(sz);
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.set(3, 0);
    cout << f.str() << endl;
    NinXorSolv a(sz, f);

    cout << a.str() << endl;

    int un = a.unbound();

    auto b = NinXorSolv(a);
    //cout << "forcing " << un << endl;
    b.forceAt(un, true);

    auto c = NinXorSolv(a);
    c.forceAt(un, false);

    cout << "------- true" << endl;
    cout << b.str() << endl;
    cout << b.strbound() << endl;

    cout << "------- false" << endl;
    cout << c.str() << endl;
    cout << c.strbound() << endl;

    auto d = NinXorSolv(a);
    auto myx = xorExpr();
    myx.e.push_back(prodExpr(9, 8));
    myx.e.push_back(prodExpr(7, 6));
    d.substitute(1, myx);
    cout << d.str() << endl;
    cout << d.strbound() << endl;

    cout << "-----------------" << endl;
    vector<bitField> invr = a.sat();

    cout << invr.size() << " results found" << endl;


    for (auto b : invr) {
        cout << b.str() << endl;
    }

}

void testCompImplxo() {
    int nbBit = 8;

    SoluSimp ss(nbBit);
    ss.debug_coef();

    //refInvert refInv;
    seqInvert seqInv;
    seqInvertSimetric seqSym;
    xorInvert xoInv;
    //compareImpl imp(&refInv,&seqInv);   
    //compareImpl imp(&refInv,&refInv);
    // compareImpl imp(&seqInv,&seqInv); 
    //compareImpl imp(&seqInv, &seqSym);
    compareImpl imp(&xoInv, &seqSym);
    //compareImpl imp(&seqSym, &seqSym);
    //compareImpl imp(&xoInv, &xoInv);
    imp.compareThem(nbBit, 40);
}

void NinXorSolv::test() {
    //oldTest();
    //testCompImplxo();
    test2();
}




