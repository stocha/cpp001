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

        short min() {
            if (size() <= 0) {
                cerr << "no var in term" << endl;
                exit(1);
            };
        }

        bool replace(short oldv, short newv) {
            for (int i = 0; i < it.size(); i++) {
                if (it[i] == oldv) {
                    it[i] = newv;
                }
            }
            sort(it.begin(), it.end());
            unique(it.begin(), it.end());
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
                    sout << hex << setw(3) << x;
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

        void add(term t) {
            l.push_back(t);
            sort(l.begin(), l.end());
            unique(l.begin(), l.end());
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

    public:

        equation(vector<bool> in) : bits(in.size()), bound(in.size()) {
            lines.reserve(bits.size()*2);

            for(int i=0;i<in.size();i++){
                lines.push_back(diag(i,in[i],in.size()));
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


            bool has = false;

            for (int i = 0; i < bits.size(); i++) {
                if (i % 8 == 0) sout << " ";
                if (bound[i]) sout << "|";
                else sout << ".";
                sout << (bits[i] ? "#" : "-");
            }

            sout << "";
            for (auto x : lines) {
                if (has) sout << endl;
                sout << x.str();
                has = true;
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

    public:
        
        vector<vector<bool>> result(){
            return solution;
        }

        void solve() {

            vector<int> st;

            if (in[in.size() - 1]) return;

            equation e(in);

             cout << "solving " << endl << e.str() << endl;

           // recsolve(0, e, st);

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

        XSol4(vector<bool> inp)  {
            in = inp;
        }        

    private:

    };

}

#endif	/* XSOL4_H */

