/* 
 * File:   NinInterf.h
 * Author: denis
 *
 * Created on 18 octobre 2015, 16:25
 */

#ifndef NININTERF_H
#define	NININTERF_H

#include <cstdlib>
#include <iomanip>
#include <bitset>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream> //this is where istringstream is defined
#include <string>
#include <memory>
using namespace std;

class SoluSimp {
    int sz;

public:

    SoluSimp(int sz) {
        this->sz = sz;
    }
public:

    int nbXor(int at) {
        //if(at>=sz/2) return 0;
        int d = (sz - 1) / 2 - at;
        if (d >= 0) return ((sz / 2 - 1) - d + 1);
        cout << "ERROR " << at << " out of formbounds";
        exit(1);
        return 0;
    }

    int coefL(int at, int num) {
        return (at - num);
    }

    int coefLsec(int at, int num) {
        return sz / 2 - 1 + num - at;
    }

    int coefR(int at, int num) {
        return num + sz / 2;
    }

    int coefRsec(int at, int num) {
        return sz / 2 - 1 - num + sz / 2;
    }

    void debug_coef() {

        const int fillnb = 2;
        for (int i = 0; i < sz / 2; i++) {
            cout << setfill('0') << setw(3) << i << " :: ";
            for (int j = 0; j < nbXor(i); j++) {
                cout << setfill('0') << "(" << setw(fillnb) << coefL(i, j) << "&" << setw(fillnb) << coefR(i, j) << ")+";
            }
            cout << " & " << endl;

            if (i + 1 == sz / 2) break;
            cout << setfill('0') << setw(3) << i << " :: ";
            for (int j = 0; j < nbXor(i); j++) {
                cout << setfill('0') << "(" << setw(fillnb) << coefLsec(i, j) << "&" << setw(fillnb) << coefRsec(i, j) << ")+";
            }
            cout << " & " << endl;
        }
        //        for(int i=0;i<sz/2 -1;i++){
        //            cout << i << " :: ";
        //            for(int j=0;j<nbXor(i);j++){
        //                cout << "(" << coefLsec(i,j) << "&" << coefRsec(i,j) << ")+";
        //            }
        //            cout<<" & "<<endl;
        //        }        

    }
};

class bitField {
private:
    bitset<512> dat;
    int sz;
public:

    bitField(int startSz) : sz(startSz) {
    }

    bitField(string strHex) {
        istringstream in(strHex);
        int size = 0;
        in >> size;
        unsigned int a[size / 16];
        bitset<32> ab[size / 16];
        for (int i = 0; i < size / 16; i++) {
            in >> hex >> a[i];
            ab[i] = bitset<32>(a[i]);
        }

        for (int i = 0; i < size * 2; i++) {
            int dec = i / 32;
            int ind = i % 32;
            dat[i] = ab[dec][ind];
        }
        sz = size * 2;
    }

    bitField& operator++() // prefix ++
    {
        for (int i = 0; i < size(); i++) {
            if ((*this)[i] == 0) {
                set(i, 1);
                return *this;
            }
            set(i, 0);
        }
        return *this;
    }

    void incSymetric() // prefix ++
    {
        if (!isSymetric()) {
            ++(*this);
            return;
        }

        int s = size() / 2;
        for (int i = s; i < size(); i++) {
            if ((*this)[i] == 0) {
                set(i, 1);


                for (int i = 0; i < s; i++) { // mise a 0
                    set(i, 0);
                }
                return;
            }
            set(i, 0);
        }

        for (int i = 0; i < s; i++) {
            set(i, 0);
        }
    }

    bool isSymetric() {
        bool res = true;
        int s = size() / 2;
        for (int i = 0; i < s; i++) {
            if ((*this)[i] != (*this)[i + s]) {
                return false;
            }
        }

        return res;
    }

    bitField& swap() {
        int s = size() / 2;
        for (int i = 0; i < s; i++) {
            int l = (*this)[i + s];
            set((i + s), (*this)[i]);
            set((i), l);
        }
        return *this;
    }

    bitField& incAt(int at) // prefix ++
    {
        for (int i = at; i < size(); i++) {
            if ((*this)[i] == 0) {
                set(i, 1);
                return *this;
            }
            set(i, 0);
        }
        return *this;
    }

    bool operator==(const bitField& other) {
        bool res = other.sz == sz;
        if (!res) return res;
        for (int i = 0; i < sz; i++) {
            if ((*this)[i] != other[i]) return false;
        }
        return res;
    }

    bool isZero() {
        for (int i = 0; i < size(); i++) {
            if ((*this)[i] != 0) {
                return false;
            }
        }
        return true;
    }

    long operator[](std::size_t pos) const {
        return dat[pos] ? 1 : 0;
    }

    void set(std::size_t at, long value) {
        dat[at] = value;
    }

    int size() {
        return sz;
    }

    void randomize() {
        for (int i = 0; i < size(); i++) {
            set(i, rand() % 2);
        }
    }

    void clear() {
        for (int i = 0; i < size(); i++) {
            set(i, 0);
        }
    }

    string str() {
        std::ostringstream sout;
        unsigned int a [sz / 32];
        //bitset<32> ab[sz / 32];
        for (int i = sz - 1; i >= 0; --i) {
            sout << (dat[i] ? '1' : '0'); // print result    
            if (i % 4 == 0) sout << ".";
            if (i % 8 == 0) sout << " ";
        }

        string res = sout.str();
        return res;


    }

    string strHex() {

        std::ostringstream sout;
        int nb = sz / 32;
        if (sz % 32 != 0) nb += 1;
        unsigned int a [nb];
        bitset<32> ab[nb];
        for (int i = 0; i < sz; i++) {
            int dec = i / 32;
            int ind = i % 32;
            ab[dec][ind] = dat[i];
        }
        for (int i = 0; i < (nb); i++)
            sout << setfill('0') << setw(8) << hex << ab[i].to_ulong() << " "; // print result       

        return sout.str();
    }


};

bitField applyEncode(bitField in);

class inverterInterface {
public:
    virtual vector<bitField> invert(bitField in) = 0;
};

class refInvert : public inverterInterface {
public:

    vector<bitField> invert(bitField in) {
        vector<bitField> resvv;

        bitField res(in.size());


        while (true) {

            if (applyEncode(res) == in) {
                //cout << res.str() << " is solution " << endl;
                resvv.push_back(res);
            }

            ++res;
            if (res.isZero()) break;
        }


        return resvv;
    }
};

class seqInvert : public inverterInterface {
public:

    vector<bitField> invert(bitField in) {

        //cout << "seqInvert " << endl;

        SoluSimp ss(in.size());
        vector<bitField> resvv;

        if (in[in.size() - 1] == 1) return resvv;

        bitField res(in.size());
        res.clear();

        const int halfSize = in.size() / 2;
        int depth = 0;
        while (true) {

            // cout << "CHECK " << res.str() << " against " << in.str() << " depth " << depth << " half " << halfSize << endl;

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
                ++res;
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
                depth = 0;
                resvv.push_back(res);
                ++res;
                if (res.isZero()) break;
            }

        }


        return resvv;
    }
};

class seqInvertSimetric : public inverterInterface {
public:

    vector<bitField> invert(bitField in);
};

class compareImpl {
    inverterInterface *a;
    inverterInterface *b;
public:

    compareImpl(inverterInterface *a, inverterInterface *b) : a(a), b(b) {

    }

    void compareThem(int bitSz, int nbNum) {

        clock_t start = clock();


        bitField bf(bitSz);
        for (int essai = 0; essai < nbNum; essai++) {
            bf.randomize();
            cout << "test " << bf.strHex() << " / " << bf.str() << endl;

            auto ra = a->invert(bf);
            auto rb = b->invert(bf);


            bool eq = true;

            if (ra.size() == rb.size()) {
                cout << " nb result " << ra.size() << endl;

                for (int i = 0; i < ra.size(); i++) {
                    if (!(ra[i] == rb[i])) {
                        eq = false;
                        break;
                    }
                }
            } else {
                eq = false;
            }




            if (!eq) {
                cout << "ERROR for " << bf.str() << endl;
                cout << "---------- RA SOL" << endl;
                for (auto e : ra) {
                    cout << e.str() << "|" << endl;
                }
                cout << "---------- RB SOL" << endl;
                for (auto e : rb) {
                    cout << e.str() << "|" << endl;
                }
                cout << "----------" << endl;
                exit(1);
            }

        }

        clock_t end = clock();
        double time = (double) (end - start) / CLOCKS_PER_SEC;
        cout << " time spent for " << bitSz << " bit and " << nbNum << " values is " << time << endl;
        if (time > 0) {
            cout << "" << nbNum / time << " inst per second " << endl;
        }
    }

};

class FormulazTests {
public:
    static void testBitField();
    static void testInvertRef();
    static void testAll();

};


#endif	/* NININTERF_H */

