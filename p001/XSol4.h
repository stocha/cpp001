/* 
 * File:   XSol4.h
 * Author: denis
 *
 * Created on 27 octobre 2015, 13:37
 */

#ifndef XSOL4_H
#define	XSOL4_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

namespace xsol4 {

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
                sout << x.str();
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
        
        equation(int sz, const line& xli) : bits(sz), bound(sz){
            lines.push_back(xli);
            
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

    class XSol4 {
        vector<bool> in;


    public:
        vector<vector<bool>> solution;

    public:

        vector<string> hexSolution() {
            vector<string> res;
            for (auto inp : solution) {
                std::ostringstream sout;

                unsigned int acc = 0;
                int nbBit = 0;
                bool esp = false;
                for (int i = 0; i < inp.size(); i++) {
                    int val = inp[i];
                    acc |= (val << nbBit);
                    nbBit++;

                    if (nbBit == 32) {
                        if (esp) sout << " ";
                        sout << hex << setw(8) << setfill('0') << acc;
                        acc = 0;
                        nbBit = 0;

                        esp = true;
                    }
                }

                res.push_back(sout.str());
            }
            sort(res.begin(), res.end());
            return res;
        }

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

    public:

        vector<vector<bool>> result() {
            return solution;
        }

        void solve() {

            vector<int> st;

            if (in[in.size() - 1]) return;

            equation e(in);

            // cout << "solving " << endl << e.str() << endl;

            recsolve(0, e, st);

        }

        void solveeq(int sz,const line& eq) {
            vector<int> st;
            
            
            
            equation e(sz,eq) ;

            // cout << "solving " << endl << e.str() << endl;

            recsolve(0, e, st);
        }

        XSol4(string is) {

            int size;
            std::istringstream sin(is);
            sin >> size;

            vector<bool> input(size * 2);
            unsigned int read;
            int curr = 0;

            for (int i = 0; i < size / 16; i++) { // Read size / 16 integers to a
                sin >> hex >> read;
                for (int i = 0; i < 32; i++) {
                    input[curr + i] = ((read >> i)&1);
                }
                curr += 32;
            }

            in = input;

            for (int i = 0; i < in.size(); i++) {

                if (in[i])
                    cerr << "X";
                else
                    cerr << "-";
            }
            cerr << endl;

        }

        XSol4(const vector<bool>& inp) {
            in = inp;
        }

    private:

    };

}

#endif	/* XSOL4_H */

