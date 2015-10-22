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

class prodExpr {
public:
    vector<int> e;

public:
    
    prodExpr(){
    }

    prodExpr(int a, int b) {
        e.push_back(a);
        e.push_back(b);
    }

    int size() {
        return e.size();
    }

    void uniqueonly() {
        sort(e.begin(), e.end());
        e.erase(unique(e.begin(), e.end()), e.end());

        // cout << "after unique " << str() << endl;
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
        uniqueonly();
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

    xorExpr() {
    }

    xorExpr(bool it) {
        t = it;

    }



    vector<prodExpr> e;
    bool t = false;

    int countIt(int v) {
        int res = 0;
        for (auto x : e) {
            for (auto y : x.e) {
                if (y == v)res++;
            }
        }
        return res;
    }

    vector<std::tuple<int, xorExpr>> deduce() {
        vector<std::tuple<int, xorExpr>> res;

        if (e.size() == 1 && t == true) {
            for (auto x : e[0].e) {

                res.push_back(tuple<int, xorExpr>(x, xorExpr(true)));
            }

        } else
            if (e.size() == 1 && t == false && e[0].e.size() == 1) {
            res.push_back(tuple<int, xorExpr>(e[0].e[0], xorExpr(false)));
        } else if (e.size() > 1
                //&& false 
                ) { // deduc sub
            for (int i = 0; i < e.size(); i++) {
                if (e[i].e.size() == 1) {


                    int val = (e[i].e[0]);
                    int countit = countIt(val);
                    if (countit > 1) continue;

                    e.erase(e.begin() + i);
                    t = (t != true);

                    auto r = tuple<int, xorExpr>(val, *this);
                    //  cout << "found  " << val << " eq " << get<1>(r).str() << endl;

                    e.clear();
                    t = false;
                    res.push_back(r);
                    return res;
                }
            }

        } // deduce par substitution d'expressions

        return res;

    }

    bool substitute(int v, xorExpr xe) {
        bool r = false;

        vector<prodExpr> specProd;
        for (int i = e.size() - 1; i >= 0; i--) {

            if (xe.e.empty()) {

                if (xe.t) {
                    bool x = e[i].substitute(v, xe.t);

                    if (x && e[i].size() == 1) {

                    } else if (x && e[i].size() == 0) {
                        t = (t != true);
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


            } else { // non empty expression
                auto pe = e[i];
                bool x = pe.substitute(v, true);
                r = r | x;
                if (x) {
                    specProd.push_back(pe);
                    e.erase(i + e.begin());
                }
            }


        }

        //bool evol = !xe.t;
        //if (specProd.size()&1 == 1) {
        //   t = (t != evol);
        //}

        xorExpr mod = xe;
        if (!xe.t) {
            mod.e.push_back(prodExpr());
        }
        for (auto p : mod.e) {
            for (auto po : specProd) {
                auto dup = po;
                dup.addAll(p);

                e.push_back(dup);
            }

        }



        return r | specProd.size() > 0;
    }

    string str() {
        std::ostringstream sout;


        //cerr << " e.sz "<<e.size()<<endl;
        bool m = false;
        sout << (t ? "T /" : "F /");
        for (int i = 0; i < e.size(); i++) {
            if (m) sout << "+";
            sout << e[i].str();

            m = true;
        }

        //sout << "/";

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

    void satrec(vector<bitField>& satres) {
        bool cont = false;
        do {
            // cout << "sat for" << endl << str() << endl;
            // cout << "-----++++ bound var " << endl << strbound() << endl;
            deduce();
            //  cout << "sat deduced" << endl << str() << endl;

            // cout << "-----++++ bound var deduced " << endl << strbound() << endl;
            //cout << "satisfiable = " << unsat << endl;

            if (unsat) return;

            int n = unbound();

            if (n != -1) {
                auto c = NinXorSolv(*this);
                c.forceAt(n, true);
                c.satrec(satres);

                c = NinXorSolv(*this);
                c.forceAt(n, false);
                c.satrec(satres);

            } else {
                //cout << "sat for" << endl << str() << endl;
                //cout << "bound var " << endl << strbound() << endl;

                //cout << "satisfiable = " << unsat << endl;            
                bitField curr(Sz);
                for (int i = 0; i < Sz; i++) {
                    int e = eval(i);

                    if (e == -1) {
                        cerr << "unbound " << i << endl;
                        exit(1);
                    }
                    curr.set(i, e);

                }
                satres.push_back(curr);
                //cout << " push res " << curr.str() << " nb "<< satres.size() << endl;
            }

        } while (cont);

    }

    vector<bitField> sat() {
        vector<bitField> satres;
        //cout << "clearing the result" << endl;
        satres.clear();

        satrec(satres);


        return satres;
    }

    long eval(int pos) {
        if (solvedVar[pos].e.empty()) return (solvedVar[pos].t ? 1 : 0);

        bool res = false;
        for (auto x : solvedVar[pos].e) {
            bool ad = true;

            for (auto aa : x.e) {

                if (!bound[aa]) {
                    cout << "unbound eval " << aa << endl;
                    return -1;
                };

                int ev = eval(aa);
                if (ev == -1) {
                    cout << "unbound eval " << aa << endl;
                    return -1;
                };

                ad = ad & ev;
            }
            res = (res != ad);
        }

        if (!solvedVar[pos].t) res = (res != true);

        solvedVar[pos].e.clear();
        solvedVar[pos].t = res;


        //  cout << "eval " << pos << " " << res << endl;
        return res ? 1 : 0;
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
                sout << i << " -> " << solvedVar[i].str() << "[" << eval(i) << ". " << " ||";
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

        bool res = false;

        for (int i = 0; i < Sz; i++) {
            bool ch = diags[i].substitute(k, xe);
            res = res || ch;

            if (diags[i].e.empty() && diags[i].t != false) {
                unsat = true;
            }
        }
        return res;
    }

    void deduce() {
        bool something = false;

        //cout << " from " << endl << str();
        //cout << " from var " << strbound() << endl;

        do {
            something = false;
            for (int i = 0; i < Sz; i++) {
                auto x = diags[i].deduce();

                if (!x.empty()) {
                    something = true;

                    for (auto it : x) {
                        substitute(get<0>(it), get<1>(it));
                    }

                    break;
                }
            }

          //  cout << " deducing " << endl << str();
          //  cout << " deduc var " << strbound() << endl;
        } while (something);
        //cout << "------ end deduce -----------" << endl;
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

