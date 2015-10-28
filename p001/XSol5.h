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

using namespace std;

namespace xsol5 {

    const int upborne = 513;
    typedef bitset<upborne> prod;
    typedef set<bitset < upborne >> line;

    class XSol5 {
    public:

        XSol5(vector<bool> in) {
            

            for (int i = 0; i < in.size(); i++) {
                line it=diag(i, in[i], in.size());
                cout << strline(it) << endl;
            }

        }

    private:

        string strprod(prod p) {
            std::ostringstream sout;
            if (p.none()) {
                sout << "T";
            } else {
                bool has = false;
                sout << "(";
                for (int i = 0; i < upborne; i++) {
                    if (has) sout << ".";
                    if (p[i])
                        sout << hex << setw(1) << (i - 1);
                    has = true;
                }
                sout << ")";
            }
            return sout.str();
        }

        string strline(line l) {
            std::ostringstream sout;

            bool has = false;

            // if (locked) sout << "<LOCK>";
            sout << "[";
            for (auto x : l) {
                if (has) sout << "+";
                sout << strprod(x);
                has = true;
            }
            sout << "]";

            return sout.str();

            return sout.str();
        }

        line diag(int i, bool b, int sz) {
            line res;
            int half = sz / 2;
            int y = max(0, i - half + 1);
            prod p;
            if (!b) res.insert(p);
            for (int x = min(i, half - 1); x >= 0 && y < half; x--) {
                prod np;
                np[x + 1] = true;
                np[y + half + 1] = true;
                res.insert(np);
                y++;
            }
            return res;
        }
    };

}

#endif	/* XSOL5_H */

