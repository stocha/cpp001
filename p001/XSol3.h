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

    inline const int vp(const int x,const int y) {
        return (x << 16) | y;
    }
    
    inline const int vpx(const int v){
        return (v>>16)&0xFFFF;
    }
    
    inline const int vpy(const int v){
        return (v)&0xFFFF;
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
        vector<int> dat;

        int sz;

    private:

        int at(cursor const c) {
            int v=dat[c.ptr()];            
           // cout << c.ptr() << "-->" << v;
            return v;
        }

        void setAt(cursor const c, int const x, int const y) {
            dat[c.ptr()] = vp(x, y);
            
            

           // cout << c.ptr() << "<--" << vp(x, y);
        }

    public:

        equation(vector<bool> in) : sz(in.size()), one(in.size()), dat((in.size() * in.size()) / 4 + in.size() + 2) {
            cursor c(sz);
            coefdiag coef(sz);

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

                    cout << c.str() << " | " << "{" << x << "," << y << "}";
                    setAt(c, x, y);

                    int v = at(c);
                    cout <<"'"<< vpx(v) << "." << vpy(v)<<"'"; 

                    if (c.remaining() == 0) {

                        cout << endl;
                    }

                }




            } while (++c);
        }

        string str() {
            std::ostringstream sout;
            cursor c(sz);

           // cout << "dat cap " << dat.capacity() << endl;
           // cout << "dat siz " << dat.size() << endl;
            
            do {
                // cout << c.str() << " | " ;

                int v = at(c);
                int x=vpx(v);
                int y=vpy(v);
                
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

