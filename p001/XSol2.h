/* 
 * File:   XSol2.h
 * Author: denis
 *
 * Created on 22 octobre 2015, 14:07
 */

#ifndef XSOL2_H
#define	XSOL2_H

#include <vector>
#include <algorithm>    // std::min
#include <bitset> 
#include <set>
#include <sstream>


using namespace std;

class vptwice {
    bitset<512> a;
    bitset<512> c;
};

class vp {
private:
    bitset<512> e;


public:

    vp() {
        e.reset();
    }

    vp(int i, int j) {
        e.reset();
        e.set(i, true);
        e.set(j, true);
    }

    vp(const vp& ot) {
        e = ot.e;
    }

    vp(const bitset<512>& ot) {
        e = ot;
    }

    bool operator[](size_t pos) const noexcept {
        return e[pos];
    }

    void set(size_t pos, bool val)noexcept {
        e.set(pos, val);
    }

    bool isTrue()const noexcept {
        return e.none();
    }

    void operator|=(const vp& other)noexcept {
        e |= other.e;
    }

    vp operator|(const vp& other)noexcept {
        auto x = vp(*this);
        x |= other;

        return x;
    }

    bool operator<(const vp& ot) const {
        if (isTrue() && !ot.isTrue()) return true;
        if (isTrue() && ot.isTrue()) return false;
        if (!isTrue() && ot.isTrue()) return false;

        if (e.count() < ot.e.count()) return true;
        if (e.count() > ot.e.count()) return false;

        for (int i = 0; i < e.size(); ++i) {
            if (e[i] ^ ot.e[i]) return e[i];
        }
        return false;
    }

    string str() const {
        std::ostringstream sout;

        bool t = false;
        for (int i = 0; i < e.size(); i++) {
            if (e[i] && t) sout << "." << i;
            else if (e[i]) {
                sout << i;
                t = true;
            }


        }
        if (isTrue()) sout << "T";


        return sout.str();
    }


};

class vx {
private:
    set<vp> e;

public:

    void add(const vp& ot) {

        auto id = e.find(ot);
        if (id != e.end()) {
            e.erase(ot);
        } else {
            e.insert(ot);
        }
    }

    bool isFalse() const {
        return e.empty();
    }

    string str() const {
        ostringstream sout;

        bool t = false;
        for (auto i = e.begin(); i != e.end(); ++i) {
            if (t) sout << " + " << (i->str());
            else sout << i->str();

            t = true;
        }
        if (isFalse()) sout << "F";


        return sout.str();
    }
};

class equation {
private:
    int sz;

    vector<vx> e;

private:

    vx diag(int i, bool b) {
        vx res;

        int half = sz / 2;

        int y = max(0, i - half + 1);
        for (int x = min(i, half - 1); x >= 0 && y < half; x--) {

            res.add(vp(x, y + half));

            y++;
        }

        if(b) res.add(vp());

        return res;

    }

public:

    equation(const equation& ot) {
        sz = ot.sz;
        e = ot.e;
    }

    equation(vector<bool> in) : sz(in.size()) {
        for(int i=0;i<sz;i++){
            e.push_back(diag(i,in[i]));
        }
    }
    
    string str() const {
        ostringstream sout;

        for(int i=0;i<sz;i++){
            sout << e[i].str() << endl;
        }


        return sout.str();
    }    

};

class XSol2 {
public:
    XSol2();
    XSol2(const XSol2& orig);
    virtual ~XSol2();
private:

};

#endif	/* XSOL2_H */

