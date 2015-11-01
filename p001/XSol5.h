/* 
 * File:   XSol5.h
 * Author: denis
 *
 * Created on 28 octobre 2015, 19:11
 */

#ifndef XSOL5_H
#define	XSOL5_H

#include <vector>
#include <algorithm>    // std::min
#include <bitset> 
#include <set>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

namespace xsol5 {

    class term {
    public:
        vector<short> it;

    public:

        term() {
        }

        void countVars(vector<int>& coun) {
            for (auto x : it) {
                coun[x]++;
            }
        }

        term(short x, short y) {
            it.push_back(x);
            it.push_back(y);

            makeUnique();
        }

        void add(short v) {
            it.push_back(v);
        }

        void makeUnique() {
            // cout << " before unique " << str() << endl;
            sort(it.begin(), it.end());


            if (it.size() < 2) return;
            int a = it.size() - 1;
            int b = it.size() - 2;

            while (b >= 0) {
                if (it[a] == it[b]) {


                    //it.erase(it.begin() + a);
                    it.erase(it.begin() + b);

                    a -= 1;
                    b -= 1;
                } else {
                    --a;
                    --b;
                }
            }

            // if(it.size()==2 && it[0]==it[1]){cerr << "impossible " << str() << endl; exit(1);}
            //   cout << " after unique " << str() << endl;
        }

        int size() const {
            return it.size();
        }

        short min() const {
            if (size() <= 0) {
                cerr << "no var in term" << endl;
                exit(1);
            };
            return it[0];
        }

        bool replace(short oldv, short newv) {
            bool res = false;

            for (int i = 0; i < it.size(); i++) {
                if (it[i] == oldv) {
                    it[i] = newv;
                    res = true;
                }
            }

            if (res) {
                makeUnique();
            }

            return res;
        }

        bool contains(short oldv) {
            for (int i = it.size() - 1; i >= 0; i--) {
                if (it[i] == oldv) {
                    return true;
                }
            }
            return false;
        }

        bool toTrue(short oldv) {
            bool res = false;

            for (int i = it.size() - 1; i >= 0; i--) {
                if (it[i] == oldv) {
                    it.erase(it.begin() + i);
                    res = true;
                }
            }

            if (res) {
                makeUnique();
            }

            return res;
        }

        bool isTrue() const {
            return size() == 0;
        }

        string str() const {
            std::ostringstream sout;

            if (isTrue()) {
                sout << "T";
            } else {
                bool has = false;

                sout << "(";
                for (auto x : it) {
                    if (has) sout << ".";
                    sout << hex << setw(1) << x;
                    has = true;
                }
                sout << ")";
            }

            return sout.str();

        }

        bool operator<(const term& ot) const {

            if (it.size() < ot.it.size()) return true;
            if (it.size() > ot.it.size()) return false;

            auto me = it.begin();
            auto he = ot.it.begin();

            while (me != it.end()) {
                bool inf1 = ((*me) < (*he));
                bool inf2 = ((*he) < (*me));
                if (inf1) return true;
                if (inf2) return false;
                ++me;
                ++he;
            }
            return false;
        }

        bool operator==(const term& ot) const {


            if (it.size() != ot.it.size()) return false;

            auto me = it.begin();
            auto he = ot.it.begin();

            while (me != it.end()) {
                bool inf1 = ((*me) < (*he));
                bool inf2 = ((*he) < (*me));
                if (inf1) return false;
                if (inf2) return false;
                ++me;
                ++he;
            }
            return true;
        }
    };

    class line {
        vector<term> l;

    public:
        bool locked = false;


    public:

        line addAll(line x) {
            line res;

            for (auto i : l) {
                res.add(i);
            }
            for (auto i : x.l) {
                res.add(i);
            }

            res.sortMelt();

            return res;
        }

        line fusion(const line& li) {
            line res;


            for (const term& srct : l) {

                //  cout << " term " << srct.str() << endl;
                for (const term& o : li.l) {
                    term x = srct;
                    for (short t : o.it) {
                        x.add(t);
                    }
                    x.makeUnique();
                    res.add(x);

                }
                // cout << " become " << res.str() << endl;
            }
            res.sortMelt();
            return res;
        }

        void countVars(vector<int>& coun) {
            for (auto x : l) {
                x.countVars(coun);
            }
        }

    private:

        void unique() {

            if (l.size() < 2) return;
            int a = l.size() - 1;
            int b = l.size() - 2;

            while (b >= 0) {
                if (l[a] == l[b]) {


                    l.erase(l.begin() + a);
                    l.erase(l.begin() + b);

                    a -= 2;
                    b -= 2;
                } else {
                    --a;
                    --b;
                }
            }
        }
    public:

        bool empty() {
            return l.empty();
        }

        int size() {
            return l.size();
        }

        bool hasTrue() {
            return l.size() > 0 && l[0].isTrue();
        }

        term lastPoly() {

            return l[l.size() - 1];
        }

        short firstSingleVar() {
            for (const term& x : l) {
                if (x.size() == 1) {
                    return x.min();
                }
            }

            return -1;
        }

        line substitute(const short v, const line& expr) {
            line res;
            bool hascont = false;
            for (auto x : l) {
                if (!x.contains(v)) {
                    res.add(x);
                    continue;
                };
                x.toTrue(v);
                hascont = true;
                for (const term & e : expr.l) {
                    term tt = x;
                    for (auto t : e.it) {
                        tt.add(t);
                    }
                    tt.makeUnique();
                    res.add(tt);
                }

            }

            if (hascont) {
                // cout<< "res befro metl "<< endl  << res.str() << endl;
                res.sortMelt();
                // cout << "substitute " << v << " <> " << expr.str() << " in " << str() << endl;
                // cout << res.str() << endl;
                return res;
            } else {
                line ot;
                return ot;
            }

        }

        line substitute2(const short v, const line& expr) {
            line res;


            bool hascont = false;
            for (auto x : l) {
                if (!x.contains(v)) {
                    res.add(x);
                    continue;
                };
                x.toTrue(v);
                hascont = true;
                for (const term & e : expr.l) {
                    term tt = x;
                    for (auto t : e.it) {
                        tt.add(t);
                    }
                    tt.makeUnique();
                    res.add(tt);
                }

            }

            if (hascont) {
                // cout<< "res befro metl "<< endl  << res.str() << endl;
                res.sortMelt();
                // cout << "substitute " << v << " <> " << expr.str() << " in " << str() << endl;
                // cout << res.str() << endl;
                return res;
            } else {
                line ot;
                return ot;
            }
        }

        short uniqueSingleVar(vector<int>& coun) {
            bool counted = false;
            for (auto x : l) {
                if (x.size() == 1) {
                    if (!counted) {
                        countVars(coun);
                    }
                    if (coun[x.min()] == 1) return x.min();
                }
            }

            return -1;
        }

        void forceBit(short it, bool v) {
            bool done = false;
            if (v) {
                for (int i = l.size() - 1; i >= 0; i--) {
                    done |= l[i].toTrue(it);
                }
            } else {
                for (int i = l.size() - 1; i >= 0; i--) {
                    if (l[i].contains(it)) {
                        l.erase(l.begin() + i);
                    }
                }
            }
            if (done) {
                sortMelt();
            }


        }

        void sortMelt() {
            sort(l.begin(), l.end());
            unique();
        }

        bool checkUnsat() {
            if (l.size() != 1) return false;
            if (l[0].isTrue()) return true;
        }

        void add(term t) {
            l.push_back(t);
            sortMelt();
        }

        void remove(short t) {
            for (int i = 0; i < l.size(); i++) {
                if (l[i].size() == 1 && l[i].min() == t) {
                    l.erase(l.begin() + i);
                }
            }

        }

        string str() const {
            std::ostringstream sout;


            bool has = false;

            if (locked) sout << "<LOCK>";
            sout << "[";
            for (auto x : l) {
                if (has) sout << "+";
                sout << x.str(); //sout << endl;
                has = true;
            }
            sout << "]";

            return sout.str();

        }

    };

    class equation {
        vector<bool> bits;
        vector<bool> bound;

    public:
        vector<line> lines;

    private:
        bool unsat = false;



    public:

        line anf() {

            equation& eq = *this;

            // cout << eq.str() << endl;

            line ll;
            term tr;
            ll.add(tr);

            // cout << " --------- " << endl;
            for (auto x : eq.lines) {
                x.add(tr);
                ll = ll.fusion(x);

                // cout << " multiply " << x.str() << endl;

                // cout << ll.str() << endl;
            }

            ll.add(tr);

            return ll;
        }

        bool getUnsat() {
            return unsat;
        }

        vector<bool> getResult() const {
            return bits;
        }

        short findUnbound() {
            vector<int> coun(bits.size());

            for (auto x : lines) {
                x.countVars(coun);
            }

            short it = -1;
            int max = 0;
            for (int i = 0; i < coun.size(); i++) {
                if (coun[i] > max && !bound[i]) {
                    max = coun[i];
                    it = i;
                }

            }

            if (it == -1)
                for (int i = 0; i < bits.size(); i++) {
                    if (!bound[i]) return i;
                }

            return it;
        }

        void forceBit(short it, bool v) {
            if (bound[it]) {
                cerr << "forceBit error alredy bound" << endl;
                exit(1);
            }

            bound[it] = true;
            bits[it] = v;

            for (int i = lines.size() - 1; i >= 0; i--) {
                lines[i].forceBit(it, v);
                //cout << "forcing "<< it << " at " << v << " on " << x.str() << endl;
            }
        }

        void trimEmpty() {
            for (int i = lines.size() - 1; i >= 0; i--) {
                //cout << "forcing "<< it << " at " << v << " on " << x.str() << endl;                
                if (lines[i].empty()) {
                    lines.erase(lines.begin() + i);
                }
                unsat |= lines[i].checkUnsat();
                if (unsat) {
                    //  cout << "unsat line found !! " << lines[i].str() << endl;
                    return;
                }
            }
        }

        void applySubstitution(const short v, const line& l) {
            vector<int> torem;
            vector<line> toadd;

            //     cout << "applying subst " << endl << str() << endl;

            int currsz = lines.size();
            for (int i = 0; i < currsz; i++) {
                line& x = lines[i];
                if (x.locked) continue;

                auto nline = x.substitute(v, l);
                if (nline.size() != 0) {
                    // cout << " future rem " << i << endl;
                    torem.push_back(i);
                    toadd.push_back(nline);
                }
            }
            //   cout << " before erased old lines "<< endl << str() << endl;


            for (int i = torem.size() - 1; i >= 0; i--) {
                int itrm = torem[i];

                //     cout << "erasing " << itrm << " " << lines[itrm].str() << endl;
                lines.erase(lines.begin() + itrm);
            }
            // cout << " erased old lines "<< endl << str() << endl;
            for (const line& l : toadd) {
                lines.push_back(l);
            }

            trimEmpty();
        }

        bool substitutionDeduction() {
            bool found = false;
            vector<int> coun(bits.size());
            for (int i = lines.size() - 1; i >= 0; i--) {
                if (lines[i].locked) continue;
                short f = lines[i].uniqueSingleVar(coun);

                if (f != -1) {

                    line model = lines[i];
                    model.remove(f);


                    found = true;
                    lines[i].locked = true;

                    // cout << "subst found " << f << " <> " << model.str() << endl;



                    // cout << "keeping :  " << f << " <> " << model.str() << endl;
                    applySubstitution(f, model);

                    //   cout << "after sub "<< endl << str() << endl;
                    return true;

                }



            }
            return found;
        }

        bool basicDeduction() {

            //   cout << "basic deduction apply " << endl;

            bool found = false;
            for (int i = lines.size() - 1; i >= 0; i--) {
                if (lines[i].size() == 2 && lines[i].hasTrue()) {

                    term t = lines[i].lastPoly();

                    //     cout << "basic simple true " << lines[i].str() << endl;

                    for (short s : t.it) {
                        forceBit(s, true);
                        found = true;
                    }

                    lines.erase(lines.begin() + i);
                } else if (lines[i].size() == 1 && !lines[i].hasTrue()) {

                    short s = lines[i].firstSingleVar();
                    if (s != -1) {
                        //       cout << "basic simple false " << lines[i].str() << endl;
                        forceBit(s, false);
                        found = true;

                        lines.erase(lines.begin() + i);
                    }
                }
            }

            return found;
        }

        equation(vector<bool> in) : bits(in.size()), bound(in.size()) {
            // cout << "building equation";

            lines.reserve(bits.size()*2);

            for (int i = 0; i < in.size(); i++) {
                lines.push_back(diag(i, in[i], in.size()));
            }


        }

        equation(int sz, const line& xli) : bits(sz), bound(sz) {
            lines.push_back(xli);

        }
        
        
        
        void factorize(){
            vector<line> added;
            
            
            for(int i=lines.size()-1;i>=0;--i){
                vector<int> coun(bits.size());
                lines[i].countVars(coun);
                vector<int> factor;
                
                if(!lines[i].hasTrue()) continue;
                
                for(int k=0;k<coun.size();k++){
                  //  cout << k << " has " << coun[k] << endl;
                    if(coun[k]==lines[i].size()-1){
                        factor.push_back(k);
                    }
                }
                
                if(factor.size()>0){
                    line r=lines[i];
                    for(int f : factor){
                        r.forceBit(f,true);
                    }
                    r.sortMelt();
                    added.push_back(r);
                    
                    line n;
                    term t;
                    
                    for(int f: factor){
                        t.add(f);
                    }
                    n.add(t);
                    
                    added.push_back(n);
                    lines.erase(lines.begin()+i);
                    
                }
                
            }
            
            for(auto t : added){
                lines.push_back(t);
            }
        
        
        }
        
    private:

        line diag(int i, bool b, int sz) {
            line res;

            int half = sz / 2;

            int y = max(0, i - half + 1);

            if (b) res.add(term());
            for (int x = min(i, half - 1); x >= 0 && y < half; x--) {

                res.add(term(x, y + half));

                y++;
            }



            return res;

        }
    public:

        string strCurrSolve() {
            std::ostringstream sout;

            // cout << "dat cap " << dat.capacity() << endl;
            // cout << "dat siz " << dat.size() << endl;
            int sz = bits.size();
            for (int i = sz - 1; i >= 0; i--) {

                if (bound[i])
                    sout << bits[i] << "|";
                else
                    sout << "(" << bits[i] << ")" << "|";
            }
            sout << endl;

            return sout.str();
        }

        string str() {
            std::ostringstream sout;


            for (int i = 0; i < bits.size(); i++) {
                if (i % 8 == 0) sout << " ";
                if (bound[i]) sout << "|";
                else sout << ".";
                sout << (bits[i] ? "#" : "-");
            }



            for (auto x : lines) {
                sout << endl;
                sout << x.str();
            }

            return sout.str();

        }
    };

    class XSol5 {
    public:

        void test01() {
            // minimizing normal form
            int sz = 6;

            int minsz = 0xFFFFFF;

            
            for (int i = 0; i < (1 << (sz - 1)); i++) {
                vector<bool> it = intToBool(i, sz);
                
                if(it[0]) continue;
                if(it[sz-2]) continue;

                equation eqsin(it);

                // cout << eqsin.str() << endl;


                line anf = eqsin.anf();
                int siz = anf.size();

                if (siz > 1) {
                   // cout << boolToStr(it) << endl;
                    //cout << anf.str() << endl << endl;
                }

                if (siz < minsz && siz > 1) minsz = siz;
            }

            for (int i = 0; i < (1 << sz); i++) {
                vector<bool> it = intToBool(i, sz);

                equation eqsin(it);

                line anf = eqsin.anf();
                int sz = anf.size();

                if (sz == minsz) {
                    
                    cout << eqsin.str() << endl;
                    cout << boolToStr(it) << endl;
                    equation anfe(sz,anf);
                    anfe.factorize();
                    cout<< "raw line " << anf.str() << endl << endl;
                    
                    cout << anfe.str() << endl << endl;
                }
            }

        }

        void test00() {

            vector<bool> x00x = strToBool("0001 0010 0100 1000");


            vector<bool> x000 = strToBool(" 0000 0000");
            cout << boolToStr(x000) << endl;

            equation eq000(x000);
            line l000 = eq000.anf();

            cout << l000.str() << endl;

            for (int i = 0; i < x000.size(); i++) {
                vector<bool> sing = x000;
                sing[i] = true;
                cout << boolToStr(sing) << endl;
                equation eqsin(sing);
                cout << eqsin.anf().str() << endl;
                //cout << eqsin.anf().addAll(l000).str()<<endl;
            }

            solveeq(x000.size(), eq000);

            cout << "soluces for" << endl;
            for (auto x : solution) {
                cout << boolToStr(x) << endl;
            }
        }

        vector<vector<bool>> solution;

        void recsolve(int depth, equation& e, vector<int> stsol) {
            //  cout << "input for depth " << depth << endl;
            // cout << e.str() << endl;

            //                                    if (depth < 18) {
            //                                        cout << "d" << depth << "--" << e.strCurrSolve() << endl;
            //                                        for (int i = 0; i < stsol.size(); i++) {
            //                                            cout << stsol[i] << "|";
            //                                        }
            //                                        cout << endl;
            //                                    }


            // while (e.buble() || (!e.unsat && e.deduction()));

            bool loop = false;
            do {
                loop = false;

                while (e.basicDeduction()) {
                    //cout << " deducted base " << depth << endl;
                    //cout << e.str() << endl;

                };
                e.trimEmpty();

                loop |= e.substitutionDeduction();

                if (loop) {
                    //cout << " deductedsub " << depth << endl;
                    //cout << e.str() << endl;

                    e.trimEmpty();
                }


            } while (loop);





            if (e.getUnsat()) {
                //  cout << "IT IS UNSAT " << endl;
                return;
            }

            //int f = e.findOneVar();
            int f = e.findUnbound();
            if (f == -1) {
                // cout << " no var found ending " << " depth " << depth << endl;
                //cout << endl << e.str() << endl;
                solution.push_back(e.getResult());

                return;
            }

            equation next = e;
            next.forceBit(f, false);
            stsol.push_back(-f);
            // cout << "depth " << depth << " exploring " << f << " to false " << endl;
            recsolve(depth + 1, next, stsol);
            e.forceBit(f, true);
            stsol.pop_back();
            stsol.push_back(f);
            //  cout << "depth " << depth << " setting " << f << " to true " << endl;
            recsolve(depth + 1, e, stsol);





        }

        void solveeq(int sz, const equation& eq) {
            vector<int> st;

            // cout << "solving " << endl << e.str() << endl;

            equation e = eq;
            recsolve(0, e, st);
        }

        vector<bool> strToBool(string s) {
            vector<bool> res;
            for (auto x : s) {
                if (x == '1') res.push_back(true);
                if (x == '0') res.push_back(false);
            }
            return res;
        }

        vector<bool> intToBool(unsigned int s, int sz) {
            vector<bool> res;
            unsigned int curs = 1;
            int dec = 0;
            while (curs != 0 && (curs < (1 << sz))) {
                unsigned int x = (s & curs) >> dec;
                if (x == 1) res.push_back(true);
                if (x == 0) res.push_back(false);

                dec++;
                curs = curs << 1;
            }
            return res;
        }

        string boolToStr(vector<bool> it) {
            ostringstream sout;

            int c = 0;
            for (auto b : it) {
                sout << b ? 1 : 0;
                if (c % 8 == 3) sout << ".";
                if (c % 8 == 7) sout << " ";

                ++c;
            }
            return sout.str();
        }

        XSol5(vector<bool> in) {
            //test00();
            test01();

        }

    private:


    };

}

#endif	/* XSOL5_H */

