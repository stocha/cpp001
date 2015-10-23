/* 
 * File:   NinXorSolv.cpp
 * Author: denis
 * 
 * Created on 18 octobre 2015, 17:11
 */

#include "NinXorSolv.h"

#include "XSol2.h"

vector<bool> bftov(bitField bf) {
    vector<bool> res;
    for (int i = 0; i < bf.size(); i++) {
        res.push_back(bf[i]);
    }
    return res;
}

bitField vtobf(vector<bool> v) {
    bitField res(v.size());
    for (int i = 0; i < v.size(); i++) {
        res.set(i,v[i]);
    }
    return res;
}



class  xsol2invert: public inverterInterface {

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
        XSol2 sol=XSol2(bftov(in));


        auto pres = sol.dosolve();
        
        vector<bitField> res;
        
        for(int i=0;i<pres.size();i++){
            res.push_back(vtobf(pres[i]));
        }

        sort(res.begin(), res.end(), compareBits);
        return res;
    }
};

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



void xsolt00() {
    auto x = vx();

    x.add(vp(4, 5));
    x.add(vp(4, 5) | vp(6, 7));
    x.add(vp(3, 3));
    x.add(vp(4, 5));
    x.add(vp(7, 8));


    cout << x.str() << endl;

    x.add(vp(4, 5) | vp(6, 7));
    x.add(vp(3, 3));
    x.add(vp());

    cout << x.str() << endl;

    x.add(vp(7, 8));
    x.add(vp());

    cout << x.str() << endl;
}

void xsolt02() {
    srand(0xCAFEBABE);

    int sz = 8;
    bitField f(sz);
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    
    
    f.set(0, 1);
    f.set(sz-1, 0);    


    f = bitField(8);
    f.set(0, 0);
    f.set(1, 0);
    f.set(2, 1);
    f.set(3, 0);
    f.set(4, 0);
    f.set(5, 0);
    f.set(6, 0);
    f.set(7, 0);    
    
   
    
    cout << f.str() << endl;
    
    auto x= XSol2(bftov(f)) ;
    
    x.dosolve();
}

void xsolt01() {
    srand(0xCAFEBABE);

    int sz = 8;
    bitField f(sz);
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();
    f.randomize();

   // f = bitField(8);
    f.set(0, 1);
    f.set(sz-1, 0);
//    f.set(1, 1);
//    f.set(2, 0);
//    f.set(3, 1);
//    f.set(4, 1);
//    f.set(5, 1);
//    f.set(6, 1);
//    f.set(7, 0);


    //0010.1100

    cout << f.str() << endl;
    equation a(bftov(f));

    cout << a.str() << endl;
    
    cout << a.debugUnique() << endl;


    cout << "-----------------" << endl;
    //vector<bitField> invr = a.sat();

    //cout << invr.size() << " results found" << endl;


    //for (auto b : invr) {
    //    cout << b.str() << endl;
    //}

}

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
    f.set(1, 0);
    f.set(2, 1);
    f.set(3, 0);
    f.set(4, 0);
    f.set(5, 0);
    f.set(6, 0);
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
    
    cout << " end result " << "------------";

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
   // int nbBit = 512;
     int nbBit = 16;

    SoluSimp ss(nbBit);
    //ss.debug_coef();

    //refInvert refInv;
    seqInvert seqInv;
    seqInvertSimetric seqSym;
    xorInvert xoInv;
    
     xsol2invert solinv;
    //compareImpl imp(&refInv,&seqInv);   
    //compareImpl imp(&refInv,&refInv);
    // compareImpl imp(&seqInv,&seqInv); 
    //compareImpl imp(&seqInv, &seqSym);
    //compareImpl imp(&xoInv, &seqSym);
    //compareImpl imp(&seqSym, &seqSym);
   // compareImpl imp(&xoInv, &xoInv);
    //compareImpl imp(&solinv, &seqSym);
     compareImpl imp(&solinv, &solinv);
    imp.compareThem(nbBit, 30);
}

void NinXorSolv::test() {
    //oldTest();
    testCompImplxo();
    //test2();

   // xsolt00();
   //  xsolt01();
   // xsolt02();
}




