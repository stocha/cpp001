/* 
 * File:   NinXorSolv.h
 * Author: denis
 *
 * Created on 18 octobre 2015, 17:11
 */

#ifndef NINXORSOLV_H
#define	NINXORSOLV_H

#include <vector>
#include <algorithm>    // std::min

using namespace std;

#include "NinInterf.h"
#include "Formulae.h"

class prodExpr {
    vector<int> e;

public:

    prodExpr(int a, int b) {
        e.push_back(a);
        e.push_back(b);
    }

    bool substitute(int v, vector<prodExpr> xe, bool b) {

        int found = -1;
        for (int i = 0; i < e.size(); i++) {
            if (e[i] == v) {
                found = i;
                break;
            }
        }

        if (found != -1) {
            e.erase(e.begin() + found);
            
            return true;
        }

    }

    string str() {

        std::ostringstream sout;

        bool m = false;
        for (int i = 0; i < e.size(); i++) {

            if (m)
                sout << "*";

            sout << e[i];

            m = true;

        }

        return sout.str();
    }
};

class xorExpr {
public:



    vector<prodExpr> e;
    bool t = false;

    void substitute(int v, xorExpr xe) {

        for (int i = 0; i < e.size(); i++) {
            bool x = e[i].substitute(v, xe.e,xe.t);
            
        }

    }

    string str() {
        std::ostringstream sout;


        //cerr << " e.sz "<<e.size()<<endl;
        //sout << t?"":"!";

        for (int i = 0; i < e.size(); i++) {
            sout << "+";
            sout << e[i].str();
        }

        return sout.str();

    }
};

class NinXorSolv {
public:

    int Sz;
private:
    vector<xorExpr> diags;
    vector<xorExpr> solvedVar;
    bitField bound;

    bool unsat = false;
public:

    NinXorSolv(int szp, bitField ovecp) : Sz(szp), solvedVar(szp), bound(szp) {
        for (int i = 0; i < Sz; i++) {
            diags.push_back(diag(i));
        }

    }

    NinXorSolv(const NinXorSolv& that) : bound(that.Sz) {
        *this = that;
    }

    string str() {
        string res = "";
        for (int i = 0; i < diags.size(); i++) {
            res += diags[i].str();
            res += +"\n";
        }
        return res;
    }

    int unbound() {
        for (int i = 0; i < Sz; i++) {
            if (!bound[i]) return i;
        }

        return -1;
    }

    void forceAt(int i, bool force) {
        if (bound[i]){ cerr << "already bound" << endl;
            exit(1);
        }
        solvedVar[i] = xorExpr();
        solvedVar[i].t = force;
        bound.set(i,1);
        
        substitute(i,solvedVar[i]);

    }

    void substitute(int k, xorExpr xe) {

        for (int i = 0; i < Sz; i++) {
            diags[i].substitute(k, xe);
        }

    }



    static void test();

private:

    xorExpr diag(int i) {
        xorExpr res;

        int half = Sz / 2;

        int y = max(0, i - half + 1);
        for (int x = min(i, half - 1); x >= 0 && y < half; x--) {

            res.e.push_back(prodExpr(x, y + half));

            y++;
        }


        return res;

    }




};

#endif	/* NINXORSOLV_H */

