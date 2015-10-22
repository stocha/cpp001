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
#include <iostream>


using namespace std;

class vp {
public:
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
    
    vp(int i){
        e.reset();
        e.set(i,true);
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

    int count()const {
        return e.count();
    }

    void operator|=(const vp& other)noexcept {
        e |= other.e;
    }

    void operator&=(const vp& other)noexcept {
        e &= other.e;
    }

    vp operator|(const vp& other)noexcept {
        auto x = vp(*this);
        x |= other;

        return x;
    }

    vp operator&(const vp& other)noexcept {
        auto x = vp(*this);
        x &= other;

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

class vptwice {
    bitset<512> a;
    bitset<512> c;

public:

    vptwice() {
        a.reset();
        c.reset();
    }

    void add(const vp it) {
        bitset<512> cf = a;
        cf &= it.e;
        a ^= it.e;

        c |= cf;

    }

    vp unique() {
        bitset<512> cf = c;
        cf.flip();
        cf &= a;
        return vp(cf);
    }

    vp vars() {
        bitset<512> cf = c;
        cf |= a;
        return vp(cf);
    }

};

class vx {
private:
    set<vp> e;

public:
    
    vx(){
    }
    
    vx(vp a){
        e.insert(a);
    }
    
    vx(vp x, vp y){
        e.insert(x);
        e.insert(y);
    }    

    vp uniqueCandidate() const {
        vptwice t;

        for (auto x : e) {
            t.add(x);
        }
        return t.unique();
    }

    vp vars() const {
        vptwice t;

        for (auto x : e) {
            t.add(x);
        }
        return t.vars();
    }

    vp singleBogoss() const {
        vp un = uniqueCandidate();
        for (auto x : e) {

            if (x.count() > 1) return vp();


            if (!(x & un).isTrue()) {
                return x;
            }

        }
        return vp();
    }

    bool operator<(const vx& ot) const {
        auto mev = vars();
        auto hev = ot.vars();

        if (mev < hev) return true;
        if (hev < mev) return false;


        if (isFalse() && !ot.isFalse()) return true;
        if (isFalse() && ot.isFalse()) return false;
        if (!isFalse() && ot.isFalse()) return false;

        if (e.size() < ot.e.size()) return true;
        if (e.size() > ot.e.size()) return false;

        auto me = e.begin();
        auto he = ot.e.begin();

        while (me != e.end()) {
            bool inf1 = ((*me) < (*he));
            bool inf2 = ((*he) < (*me));
            if (inf1) return true;
            if (inf2) return false;
            ++me;
            ++he;
        }
        return false;
    }

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

//class eqLine {
//    vx l;
//    bool dirty = true;
//    bool bog = false;
//
//public:
//
//    bool operator<(const eqLine& ot) const {
//        return l < ot.l;
//    }
//
//    string str() const {
//        ostringstream sout;
//
//        sout << (dirty ? " DIRT " : "") << (bog ? "BOGOSSED " : "") << l.str();
//
//
//        return sout.str();
//    }
//
//    eqLine(vx lp) : l(lp) {
//    }
//
//    vp singleBogoss() const {
//        return l.singleBogoss();
//    }
//    
//    vp uniqueCandidate() const {
//        return l.uniqueCandidate();
//    }
//};

class equation {
private:
    int sz;


public:
    set<vx> dirty;
    set<vx> clean;
    set<vx> bogossed;

private:

    vx diag(int i, bool b) {
        vx res;

        int half = sz / 2;

        int y = max(0, i - half + 1);
        for (int x = min(i, half - 1); x >= 0 && y < half; x--) {

            res.add(vp(x, y + half));

            y++;
        }

        if (b) res.add(vp());

        return res;

    }



public:

    equation() {
    }

    equation(vector<bool> in) : sz(in.size()) {
        for (int i = 0; i < sz; i++) {
            dirty.insert(diag(i, in[i]));
        }
    }

    string str() const {
        ostringstream sout;

        sout << "DIRTY" << endl;
        for (auto x : dirty) {
            sout << x.str() << endl;
        }
        sout << "CLEAN" << endl;
        for (auto x : clean) {
            sout << x.str() << endl;
        }

        sout << "BOGOSSED" << endl;
        for (auto x : bogossed) {
            sout << x.str() << endl;
        }

        sout << " --- end equation --- " << endl;


        return sout.str();
    }

    string debugUnique() const {
        ostringstream sout;

        for (auto x : dirty) {
            vp s = x.singleBogoss();
            sout << x.uniqueCandidate().str() << " BOGOSS " << s.str() << endl;
        }


        return sout.str();
    }

    bool brush() {

        for (auto x : dirty) {

            auto b = x.singleBogoss();

            if (b.isTrue()) {
                dirty.erase(x);
                clean.insert(x);
                
                return true;
            } else {

                dirty.erase(x);
                bogossed.insert(x);
                
                return true;
            }
        }

        return false;
    }

    vp findFree() {
        for (auto x : clean) {
            if (x.isFalse()) continue;
            auto v = x.vars();
          //  cerr << "vars " << v.str() << endl;
            
            if(v.isTrue() ) continue;
            
            for (int i = 0; i < 512; i++) {
                if (v[i]) {
                    return vp(i);
                }else{
            //        cerr << i << " is " << v[i] << endl;
                
                }
            }
        }
        return vp();
    }

    equation derive(bool phase) {
        equation res = *this;

        auto v=findFree();

        if(!v.isTrue()){
            if(phase)
                res.dirty.insert(vx(vp(),v));
            else
                res.dirty.insert(vx(v));
        }else{
            cerr << "nothing found" << endl;
        }


        return res;
    }

};

class XSol2 {
    vector<bool> in;

public:

    XSol2(vector<bool> inp) : in(inp) {


    }

    void dosolve() {
        equation eq(in);

        cout << eq.str();
        while (eq.brush()) {

            cout << "broching " << endl;
            cout << eq.str();

        }

        
        equation next1 = eq.derive(true);
        equation next0 = eq.derive(false);
        cout << "next T " << endl;
        cout << next1.str();
        
        cout << "next F " << endl;
        cout << next0.str();    
        
        while (next0.brush()) {

            cout << "broching false " << endl;
            cout << next0.str();

        }      
        
        while (next1.brush()) {

            cout << "broching true " << endl;
            cout << next1.str();

        }               

    }


private:

};

#endif	/* XSOL2_H */

