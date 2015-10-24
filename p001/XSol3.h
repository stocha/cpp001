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
        int left = 2;

        int phase = 0;

        int sz;

    public:
        
        int currloc() const{
            return vcurrloc;
        }
        
        int remaining() const{
            return left;
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
                    vcurrloc=0;
                }
                left = cdiagsz++;
            }

            return vcurr^sz;
        }

        bool operator++() {
            return inc();
        }

        string str() const {
            std::ostringstream sout;

            
            sout << " [ " << cdiagsz << " " << vcurrloc << " ] ";
            
            
            return sout.str();
        }


    };

    class equation {
        vector<bool> one;
        vector<int> dat;

        int sz;

    private:

    public:

        equation(vector<bool> in) : sz(in.size()), one(in.size()), dat(in.size()*(in.size() + 1) + 1) {
            cursor c(sz);



        }

    };

    class XSol3 {
        int sz;
        
    public:
        XSol3(int size ) : sz(size){}
        
        
        

        void debugParcours() {
            cursor c(sz);
            
            do{
                if(!c.remaining()){
                    cout << endl;
                }                
                cout << c.str() << " | " ;

                
            }while(++c);

        }        
    private:

    };

}
#endif	/* XSOL3_H */

