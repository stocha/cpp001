/* 
 * File:   XSol3.h
 * Author: denis
 *
 * Created on 24 octobre 2015, 13:06
 */

#ifndef XSOL3_H
#define	XSOL3_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;
namespace xsol3 {

    const int nul = 0xFFFFFFFF;

    inline const int vp(const int x, const int y) {
        return ((x + 1) << 16) | (y + 1);
    }

    inline const int vpx(const int v) {
        if (v == -1) return -1;

        return ((v >> 16)&0xFFFF) - 1;
    }

    inline const int vpy(const int v) {
        if (v == -1) return -1;

        return ((v)&0xFFFF) - 1;
    }

    class cursor {
    private:
        int vcurr = 1;

        int vcurrloc = 0;

        int cdiagsz = 1;
        int left = 1;

        int phase = 0;

        int diagind = 0;

        int sz;

    public:

        int halfdiag() const {
            return phase;
        }

        int currloc() const {
            return vcurrloc;
        }

        int remaining() const {
            return left;
        }

        int diagnum() const {
            return diagind;
        }

        int intbitnum() {
            if (phase == 0) return diagnum();
            return sz - diagnum() - 2;
        }

        int ptr() const {
            return vcurr;
        }

        cursor(int size) : sz(size) {
        }

        long inc() {
            vcurr++;
            vcurrloc++;


            left--;

            if (left < 0) {
                phase ^= 1;
                if (phase == 0) {
                    cdiagsz++;
                    diagind++;
                }
                vcurrloc = 0;
                left = cdiagsz;
            }

            return vcurr^((sz * sz) / 4 + sz);
        }

        bool operator++() {
            return inc();
        }

        string str() const {
            std::ostringstream sout;


            // sout << " [ Dsz " << cdiagsz << " cloc " << vcurrloc << " PH " << phase << " left " << left << " ] ";
            sout << "(" << diagnum() << "|" << vcurrloc;


            return sout.str();
        }


    };

    class coefdiag {
        int sz;
    public:

        coefdiag(int size) : sz(size) {
        }

        int coefR(int at, int num) {
            return (at - num)+sz / 2;
        }

        int coefLsec(int at, int num) {
            return sz / 2 - 1 + num - at;
        }

        int coefL(int at, int num) {
            return num;
        }

        int coefRsec(int at, int num) {
            return sz / 2 - 1 - num + sz / 2;
        }

    };

    class equation {
        vector<bool> one;
        vector<bool> bound;
        vector<int> dat;

        int sz;


    public:
        bool unsat = false;

    private:

        int at(cursor const c) {
            int v = dat[c.ptr()];
            // cout << c.ptr() << "-->" << v;
            return v;
        }

        void setAt(cursor const c, int const x, int const y) {
            dat[c.ptr()] = vp(x, y);



            // cout << c.ptr() << "<--" << vp(x, y);
        }

    public:

        vector<bool> getResult() const {
            return one;
        }

        equation(vector<bool> in) : sz(in.size()), one(in.size()), bound(in.size()), dat((in.size() * in.size()) / 4 + in.size() + 2) {
            cursor c(sz);
            coefdiag coef(sz);
            dat[0] = -1;

            do {
                // cout << c.str() << " | " ;

                if (c.currloc()) {
                    int x, y;
                    if (c.halfdiag()) {
                        x = coef.coefLsec(c.diagnum(), c.currloc() - 1);
                        y = coef.coefRsec(c.diagnum(), c.currloc() - 1);
                    } else {
                        x = coef.coefL(c.diagnum(), c.currloc() - 1);
                        y = coef.coefR(c.diagnum(), c.currloc() - 1);
                    }

                    // coef.coefR(c.currloc(),c.diagnum()/2);
                    //  coef.coefR(c.diagnum()/2, c.currloc());

                    // cout << c.str() << " | " << "{" << x << "," << y << "}";
                    setAt(c, x, y);

                    int v = at(c);
                    //cout <<"'"<< vpx(v) << "." << vpy(v)<<"'"; 

                    if (c.remaining() == 0) {

                        //  cout << endl;
                    }

                } else {
                    int bit = c.intbitnum();

                    //  cout << endl << bit << "<-" << in[bit] << endl;

                    if (in[bit])
                        dat[c.ptr()] = 0;
                    else {
                        // setAt(c,0x7FFF,0x7FFF);
                        dat[c.ptr()] = -1;
                    }

                }




            } while (++c);
        }

        int findOneVar() {
            cursor c(sz);

            do {
                if (c.currloc() != 0) {
                    int v = at(c);
                    int cy = vpy(v);

                    //   cout<< "|" << v << "e" << cy << endl;

                    if (cy >= 0) {
                        //      cout << "find var " << cy << endl;
                        return cy;
                    }
                }

            } while (++c);
            return -1;
        }

        bool deduction() {
            cursor c(sz);
            int tr = 0;
            int other = 0;
            int lastVar = -1;
            int nextVar = -1;
            do {


                if (c.currloc() == 0) {
                    tr = 0;
                    other = 0;
                    lastVar = -1;
                    nextVar = -1;
                }

                int v = at(c);
                if (v != -1) {
                    if (v == 0) {
                        tr++;
                    } else {
                        other++;
                        nextVar = lastVar;
                        lastVar = v;
                    }
                    //  cout << " " << v << " " << tr << " " << other << " " << lastVar << endl;
                }



                if (c.remaining() == 0) {
                    if (other == 1 && vpx(lastVar) == -1) {
                        break;
                    }
                }

            } while (++c);

            if (other == 1 && vpx(lastVar) == -1) {
                bool ff = (tr & 1);

                //   cout << " found " << vpy(lastVar) << " as " << ff << " at " << c.diagnum() << " " <<c.halfdiag()  << endl ;
                //   cout << str() << endl;

                solveOneVar(ff, vpy(lastVar));

                //  cout << " after " << endl;
                //  cout << str() << endl;

                return true;


            }
            if ((tr & 1) == 0 && other == 2 && vpx(lastVar) == -1 && vpx(nextVar) == -1) {
                bool ff = (tr & 1);

                //   cout << " found " << vpy(lastVar) << " as " << ff << " at " << c.diagnum() << " " <<c.halfdiag()  << endl ;
                //   cout << str() << endl;

                substitute(vpy(nextVar), vpy(lastVar));

                //  cout << " after " << endl;
                //  cout << str() << endl;

                return true;


            }
            return false;
        }

        int findUnboundVar() {
            cursor c(sz);

            int f = findOneVar();

            if (f != -1) return f;

            for (int i = 0; i < sz; i++) {
                if (!bound[i]) return i;
            }

            return -1;
        }

        bool solveOneVar(bool dir, int f) {
            bound[f] = true;
            if (dir) {
                one[f] = true;
                substitute(f, -1);
            } else {
                falsevar(f);
            }


            return true;
        }

        void falsevar(int oraw) {
            cursor c(sz);

            do {
                int curr = c.ptr();
                int v = dat[curr];
                int nv = v;

                int cx = vpx(v);
                int cy = vpy(v);

                if (cx == oraw) {
                    nv = -1;

                } else
                    if (cy == oraw) {
                    nv = -1;
                }
                //          if(c.remaining()==0) cout << endl;


                dat[curr] = nv;

            } while (++c);

        }

        void substitute(int oraw, int nraw) {

            cursor c(sz);

            do {
                int curr = c.ptr();
                int v = dat[curr];
                int nv = v;

                int cx = vpx(v);
                int cy = vpy(v);

                if (cx == oraw) {
                    nv = vp(nraw, cy);

                } else
                    if (cy == oraw) {
                    nv = vp(cx, nraw);
                }
                //          if(c.remaining()==0) cout << endl;

                if (v == -1) {
                } else
                    if (vpx(nv) > vpy(nv)) {
                    int x = vpx(nv);
                    int y = vpy(nv);
                    nv = vp(y, x);
                } else
                    if (vpx(nv) == vpy(nv)) {
                    nv = 0;
                }

                dat[curr] = nv;

            } while (++c);

        }

        bool buble() {
            bool done = false;

            cursor c(sz);
            int tr = 0;
            int other = 0;
            do {


                if (c.currloc() == 0) {
                    tr = 0;
                    other = 0;
                }

                int v = at(c);
                if (v != -1) {
                    if (v == 0) {
                        tr++;
                    } else {
                        other++;
                    }
                }


                if (c.currloc()) {
                    int curr = c.ptr();

                    if (dat[curr - 1] == -1) {
                    } else
                        if (dat[curr - 1] > dat[curr]) {
                        done = true;
                        dat[curr - 1] ^= dat[curr];
                        dat[curr] ^= dat[curr - 1];
                        dat[curr - 1] ^= dat[curr];
                    } else if (dat[curr - 1] == dat[curr]) {
                        dat [curr - 1] = nul;
                        dat[curr] = nul;
                        done = true;
                    }

                }

                if (c.remaining() == 0) {
                    if ((tr & 1) == 1 && other == 0) {
                        unsat = true;
                        return false;
                    }
                }

            } while (++c);


            return done;
        }

        string strCurrSolve() {
            std::ostringstream sout;

            // cout << "dat cap " << dat.capacity() << endl;
            // cout << "dat siz " << dat.size() << endl;

            for (int i = sz - 1; i >= 0; i--) {

                if (bound[i])
                    sout << one[i] << "|";
                else
                    sout << "(" << one[i] << ")" << "|";
            }
            sout << endl;

            return sout.str();
        }

        string str() {
            std::ostringstream sout;
            cursor c(sz);

            // cout << "dat cap " << dat.capacity() << endl;
            // cout << "dat siz " << dat.size() << endl;

            for (int i = sz - 1; i >= 0; i--) {

                if (bound[i])
                    sout << one[i] << "|";
                else
                    sout << "(" << one[i] << ")" << "|";
            }
            sout << endl;

            do {
                // cout << c.str() << " | " ;

                int v = at(c);
                int x = vpx(v);
                int y = vpy(v);

                if (v == nul) sout << "#";
                else
                    if (v == 0) sout << "T";
                else
                    sout << "[" << x << "." << y << "]";
                if (c.remaining() == 0) {
                    sout << endl;
                }



            } while (++c);

            return sout.str();
        }

    };

    class XSol3 {
        int sz;

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

        XSol3(string is) {

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

        XSol3(vector<bool> inp) : sz(in.size()) {
            in = inp;
        }

        void recsolve(int depth, equation& e, vector<int> stsol) {
            // cout << "input for depth " << depth << endl;
            // cout << e.str() << endl;
//
//            if (depth < 20) {
//                cout << "d" << depth << "--" << e.strCurrSolve() << endl;
//                for (int i = 0; i < stsol.size(); i++) {
//                    cout << stsol[i] << "|";
//                }
//                cout << endl;
//            }


            while (e.buble() || (!e.unsat && e.deduction()));


            if (e.unsat) {
                //     cout << "IT IS UNSAT " << endl;
                return;
            }

            //int f = e.findOneVar();
            int f = e.findUnboundVar();
            if (f == -1) {
                //cout << " no var found ending " << " depth " << depth << endl;
                //cout << endl << e.str();
                solution.push_back(e.getResult());

                return;
            }

            equation next = e;
            next.solveOneVar(false, f);
            stsol.push_back(-f);
            // cout << "depth " << depth << " exploring " << f << " to false " << endl;
            recsolve(depth + 1, next,stsol);
            e.solveOneVar(true, f);
            stsol.pop_back();
            stsol.push_back(f);
            //  cout << "depth " << depth << " setting " << f << " to true " << endl;
            recsolve(depth + 1, e,stsol);





        }

        vector<vector<bool>> solve() {
            
            vector<int> st;

            if (in[in.size() - 1]) return solution;

            equation e(in);

            // cout << "solving " << endl << e.str() << endl;

            recsolve(0, e,st);

            return solution;
        }

        void debugParcours(vector<bool> in) {



            equation e(in);

            //  cout << e.str() << endl;

            e.substitute(4, 0);

            // cout << "sub 4, 0" << endl << e.str() << endl;

            while (e.buble()) {
            }
            // cout << "bubled" << endl << e.str() << endl;

            e.substitute(2, 5);

            // cout << "sub 2, 5" << endl << e.str() << endl;

            while (e.buble()) {
            }
            // cout << "bubled" << endl << e.str() << endl;      

            e.substitute(0, 5);
            // cout << "sub 0, 5" << endl << e.str() << endl;
            while (e.buble()) {
            }
            // cout << "bubled" << endl << e.str() << endl; 

            e.substitute(3, -1);
            // cout << "sub 3, -1" << endl << e.str() << endl;
            while (e.buble()) {
            }
            // cout << "bubled" << endl << e.str() << endl;         

            e.substitute(7, -1);
            // cout << "sub 7, -1" << endl << e.str() << endl;
            while (e.buble()) {
            }
            cout << "bubled" << endl << e.str() << endl;

        }
    private:

    };

}
#endif	/* XSOL3_H */

