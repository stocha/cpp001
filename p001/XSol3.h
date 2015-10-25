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

using namespace std;
namespace xsol3 {

    inline int vp(int x, int y) {
        return (x << 16) | y;
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

        int currloc() const {
            return vcurrloc;
        }

        int remaining() const {
            return left;
        }

        int diagnum() const {
            return diagind;
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

            return vcurr^(sz * sz + sz * 2);
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

    };

    class equation {
        vector<bool> one;
        vector<int> dat;

        int sz;

    private:

        int at(cursor const c) {
            return one[c.ptr()];
        }

        void setAt(cursor const c, int const x, int const y) {
            one[c.ptr()] = vp(x, y);
        }

    public:

        equation(vector<bool> in) : sz(in.size()), one(in.size()), dat(in.size()*(in.size() + 1) + 1) {
            cursor c(sz);
            coefdiag coef(sz);

            do {
                // cout << c.str() << " | " ;

                int v = at(c);

                if (c.currloc()) {
                    int x = coef.coefL(c.currloc() - 1, c.diagnum() / 2);
                    int y = coef.coefR(c.currloc() - 1, c.diagnum() / 2);
                    // coef.coefR(c.currloc(),c.diagnum()/2);
                    //  coef.coefR(c.diagnum()/2, c.currloc());

                    cout << c.str() << " | " << "(" << x << "," << y << ")";
                    setAt(c, x, y);

                    if (c.remaining() == 0) {

                        cout << endl;
                    }

                }




            } while (++c);
        }

        string str() {
            std::ostringstream sout;
            cursor c(sz);


            do {
                // cout << c.str() << " | " ;

                int v = at(c);
                sout << "[" << ((v << 16) & 0xFFFF) << "." << (v & 0xFFFF) << "]";
                if (c.remaining() == 0) {
                    sout << endl;
                }



            } while (++c);

            return sout.str();
        }

    };

    class XSol3 {
        int sz;

    public:

        XSol3(int size) : sz(size) {
        }

        void debugParcours(vector<bool> in) {

            equation e(in);

            cout << e.str() << endl;

        }
    private:

    };

}
#endif	/* XSOL3_H */

