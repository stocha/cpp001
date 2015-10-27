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
    protected:
        vector<short> it;

    public:

        term() {
        }

        term(short x, short y) {
            it.push_back(x);
            it.push_back(y);

            sort(it.begin(), it.end());
            unique(it.begin(), it.end());
        }

        void add(short v) {
            it.push_back(v);
            sort(it.begin(), it.end());
            unique(it.begin(), it.end());

        }

        int size() {
            return it.size();
        }

        unsigned short min() {
            if (size() <= 0) {
                cerr << "no var in term" << endl;
                exit(1);
            };
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
                sort(it.begin(), it.end());
                unique(it.begin(), it.end());
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
                sort(it.begin(), it.end());
                unique(it.begin(), it.end());
            }

            return res;
        }

        bool isTrue() {
            return size() == 0;
        }

        string str() {
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

    private:

        void unique() {

            if (l.size() < 2) return;
            int a = l.size()-1;
            int b = l.size()-2;

            while(b>=0){
                if(l[a]==l[b]){

                    
                    l.erase(l.begin()+a);
                    l.erase(l.begin()+b);
                    
                    a-=2;
                    b-=2;                    
                }else{
                    --a;
                    --b;
                }
            }
        }
    public:

        bool empty() {
            return l.empty();
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
        
        void sortMelt(){
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

        string str() {
            std::ostringstream sout;


            bool has = false;

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

        vector<line> lines;

        bool unsat = false;



    public:

        bool getUnsat() {
            return unsat;
        }

        vector<bool> getResult() const {
            return bits;
        }

        short findUnbound() {
            for (int i = 0; i < bits.size(); i++) {
                if (!bound[i]) return i;
            }

            return -1;
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
                unsat |= lines[i].checkUnsat();
                if (lines[i].empty()) {
                    lines.erase(lines.begin() + i);
                }
            }
        }

        equation(vector<bool> in) : bits(in.size()), bound(in.size()) {
           // cout << "building equation";

            lines.reserve(bits.size()*2);

            for (int i = 0; i < in.size(); i++) {
                lines.push_back(diag(i, in[i], in.size()));
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
            // cout << "input for depth " << depth << endl;
            // cout << e.str() << endl;

//            if (depth < 20) {
//                cout << "d" << depth << "--" << e.str() << endl;
//                for (int i = 0; i < stsol.size(); i++) {
//                    cout << stsol[i] << "|";
//                }
//                cout << endl;
//            }


            // while (e.buble() || (!e.unsat && e.deduction()));


            if (e.getUnsat()) {
                //     cout << "IT IS UNSAT " << endl;
                return;
            }

            //int f = e.findOneVar();
            int f = e.findUnbound();
            if (f == -1) {
                //cout << " no var found ending " << " depth " << depth << endl;
                //cout << endl << e.str();
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
