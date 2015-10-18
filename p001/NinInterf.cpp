/* 
 * File:   NinInterf.cpp
 * Author: denis
 * 
 * Created on 18 octobre 2015, 16:25
 */

#include "NinInterf.h"

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

vector<bitField> seqInvertSimetric::invert(bitField in) {

    //cout << "seqInvert " << endl;

    SoluSimp ss(in.size());
    vector<bitField> resvv;

    if (in[in.size() - 1] == 1) return resvv;

    bitField res(in.size());
    res.clear();

    const int halfSize = in.size() / 2;
    int depth = 0;
    while (true) {

        //cout << "CHECK " << res.str() << " against " << in.str() << " depth " << depth << " half " << halfSize << endl;

        long v = in[depth];

        if (depth < halfSize) {
            //cout << "depth " << depth <<" nbXor "<< ss.nbXor(depth)<< endl;
            for (int i = 0; i < ss.nbXor(depth); i++) {
                //  cout << v << " ^= " << res[ss.coefL(depth,i)] << " & " << res[ss.coefR(depth,i)]<< endl;
                //cout << "addr" << depth << " ^= " << ss.coefL(depth,i) << " & " << ss.coefR(depth,i)<< endl;
                v ^= (res[ss.coefL(depth, i)] & res[ss.coefR(depth, i)]);
            }
        } else {
            int ndDepth = in.size() - depth - 2;
            //cout << "depth " << depth << " ndDepth " << ndDepth << " nbXor ";
            //cout << ss.nbXor(ndDepth)<< endl;
            for (int i = 0; i < ss.nbXor(ndDepth); i++) {
                //  cout << v << " ^= " << res[ss.coefLsec(ndDepth,i)] << " & " << res[ss.coefRsec(ndDepth,i)]<< endl;
                //cout << "addr" << depth << " ^= " << ss.coefLsec(ndDepth,i) << " & " << ss.coefRsec(ndDepth,i)<< endl;
                v ^= (res[ss.coefLsec(ndDepth, i)] & res[ss.coefRsec(ndDepth, i)]);
            }
        }
        if (v != 0) {
            res.incSymetric();
            depth = 0;
            //cout << "next res" << res.str() << endl;
            if (res.isZero()) {

                break;
            }
            continue;
        }
        //cout << "PASSED " << depth << endl;

        depth++;
        if (!(depth < in.size() - 1)) {
            //cout << "Depth end : " << depth << endl;

            if (!res.isSymetric()) {
                //  cout << "swap res" << res.str() << endl;
                res.swap();
                resvv.push_back(res);
                res.swap();
            } else {
                //cout << "is symetric" << res.str() << endl;
            }
            depth = 0;
            resvv.push_back(res);
            res.incSymetric();
            if (res.isZero()) break;
        }

    }
    sort(resvv.begin(), resvv.end(), compareBits);
    return resvv;
}

bitField applyEncode(bitField in) {
    int sz = in.size();
    bitField rr(sz);
    for (int i = 0; i < (sz / 2); i++) {
        for (int j = 0; j < (sz / 2); j++) {
            rr.set(i + j, rr[i + j]^(in[i] & in[j + (sz / 2)]));
        }
    }

    return rr;
}

void FormulazTests::testBitField() {

    cout << "Test BitField" << endl;

    string form = "16\nb0c1 52f9";
    bitField bf(form);

    cout << bf.strHex() << endl;
    cout << bf.str() << endl;


    bitField rf(4);

    int nbVal = 20;
    for (int i = 0; i < nbVal; i++) {
        rf.randomize();
        cout << rf.str() << "->" << applyEncode(rf).str() << endl;
    }
    nbVal = 16;
    rf.clear();
    cout << "----------- INCR ------------" << endl;
    for (int i = 0; i < nbVal; i++) {
        ++rf;
        cout << rf.str() << "->" << applyEncode(rf).str() << endl;
    }

}

void FormulazTests::testInvertRef() {
    cout << "----------- ##### ------------" << endl;
    cout << "Test InvertRef" << endl;
    cout << "----------- --- ------------" << endl;
    bitField bf(4);
    refInvert ri;

    int nbVal = 0;

    nbVal = 16;
    bf.clear();
    cout << "----------- INCR ------------" << endl;
    for (int i = 0; i < nbVal; i++) {
        ++bf;
        cout << bf.str() << "->" << "" << endl;
        for (bitField e : ri.invert(bf)) {
            cout << "----->" << e.str() << endl;
        }

    }

}

void testCompImpl() {
    int nbBit = 18;

    SoluSimp ss(nbBit);
    ss.debug_coef();

    //refInvert refInv;
    seqInvert seqInv;
    seqInvertSimetric seqSym;
    //compareImpl imp(&refInv,&seqInv);   
    //compareImpl imp(&refInv,&refInv);
    // compareImpl imp(&seqInv,&seqInv); 
    compareImpl imp(&seqInv, &seqSym);
    //compareImpl imp(&seqSym, &seqSym);
    imp.compareThem(nbBit, 30);
}

void FormulazTests::testAll() {
    //testInvertRef();
    testCompImpl();

    //testBitField();

}