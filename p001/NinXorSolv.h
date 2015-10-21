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

    int size() {
        return e.size();
    }

    bool substitute(int v, bool b) {

        int found = -1;
        for (int i = 0; i < e.size(); i++) {
            if (e[i] == v) {
                found = i;
                break;
            }
        }

        if (found != -1) {
            if (b) {
                e.erase(e.begin() + found);
            } else {
                e.clear();
            }

        }
        return found != -1;
    }

    void addAll(prodExpr o) {
        for (auto i : o.e) {
            e.push_back(i);
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

    bool substitute(int v, xorExpr xe) {
        bool r = false;

        vector<prodExpr> specProd;
        for (int i = e.size() - 1; i >= 0; i--) {

            if (xe.e.empty()) {

                if (xe.t) {
                    bool x = e[i].substitute(v, xe.t);

                    if (x && e[i].size() == 1) {

                    } else if (x && e[i].size() == 0) {
                        t=(t!=true);
                        e.erase(i + e.begin());
                    }

                    r = r | x;
                } else {
                    bool x = e[i].substitute(v, xe.t);
                    if (x) {
                        e.erase(i + e.begin());
                    }

                    r = r | x;
                }


            } else {
                auto pe = e[i];
                bool x = pe.substitute(v, true);
                r = r | x;
                if (x) {
                    specProd.push_back(pe);
                    e.erase(i + e.begin());
                }
            }

            
        }

        bool evol = !xe.t;
        if (specProd.size()&1 == 1) {
            t = (t != evol);
        }
        for (auto p : xe.e) {
            for (auto po : specProd) {
                auto dup = po;
                dup.addAll(p);
                e.push_back(dup);
            }

        }
        


        return r|specProd.size()>0;
    }

    string str() {
        std::ostringstream sout;


        //cerr << " e.sz "<<e.size()<<endl;
        sout << (t ? "T /" : "F /");
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
            diags.push_back(diag(i, ovecp[i]));
        }

    }

    NinXorSolv(const NinXorSolv& that) : bound(that.Sz) {
        *this = that;
    }

    void sat() {
        bool cont = false;
        do {
            cout << "-----++++ bound var " << endl << strbound() << endl;
            cout << "satisfiable = " << unsat << endl;
            
            if(unsat) return;

            int n = unbound();

            if (n != -1) {
                auto c = NinXorSolv(*this);
                c.forceAt(n, true);
                c.sat();

                c = NinXorSolv(*this);
                c.forceAt(n, false);
                c.sat();

            }else{
                cout << "sat for" << endl << str() << endl;
                cout << "bound var " << endl << strbound() << endl;

                //cout << "satisfiable = " << unsat << endl;            
            
            }

        } while (cont);

    }

    string str() {
        string res = "";
        for (int i = 0; i < diags.size(); i++) {
            res += diags[i].str();
            res += +"\n";
        }
        return res;
    }

    string strbound() {
        std::ostringstream sout;
        for (int i = 0; i < diags.size(); i++) {

            if (bound[i]) {
                sout << i << " -> " << solvedVar[i].str();
            }

        }
        return sout.str();
    }

    int unbound() {
        for (int i = 0; i < Sz; i++) {
            if (!bound[i]) return i;
        }

        return -1;
    }

    void forceAt(int i, bool force) {
        if (bound[i]) {
            cerr << "already bound" << endl;
            exit(1);
        }
        auto a = xorExpr();
        a.t = force;

        substitute(i, a);
    }

    bool substitute(int k, xorExpr xe) {

        bound.set(k, 1);
        solvedVar[k] = xe;

        bool res=false;
        
        for (int i = 0; i < Sz; i++) {
            bool ch=diags[i].substitute(k, xe);
            res=res||ch;
            
            if(diags[i].e.empty() && diags[i].t!=false){
                unsat=true;
            }            
        }
        return res;
    }



    static void test();

private:

    xorExpr diag(int i, bool b) {
        xorExpr res;

        int half = Sz / 2;

        int y = max(0, i - half + 1);
        for (int x = min(i, half - 1); x >= 0 && y < half; x--) {

            res.e.push_back(prodExpr(x, y + half));

            y++;
        }

        res.t = b;

        return res;

    }




};

#endif	/* NINXORSOLV_H */

