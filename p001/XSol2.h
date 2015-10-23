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

    vp(int i) {
        e.reset();
        e.set(i, true);
    }

    vp(const vp& ot) {
        e = ot.e;
    }

    vp(const bitset<512>& ot) {
        e = ot;
    }

    bool contains(const vp& ot) const {
        bitset<512> x = ot.e;
        x &= e;
        x ^= ot.e;

        return x.none();
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

    void operator^=(const vp& other)noexcept {
        e ^= other.e;
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

    vx() {
    }

    vx(vp a) {
        e.insert(a);
    }

    vx(vp x, vp y) {
        e.insert(x);
        e.insert(y);
    }

public:

    long nbProd() {
        long res = 0;

        for (auto v : e) {
            res += 1;
        }

        return res;

    }

    long nbVars() {
        long res = 0;

        for (auto v : e) {
            res += v.count();
        }

        return res;

    }

    vx applySub(const vp& bog, const vx& src) const {
        vx res;

        bool fal = src.isFalse();

        //  cout << " applying sub " << bog.str() << " <-> " << src.str() << " to " << str() << endl;

        for (auto cc : e) {
            if (!cc.contains(bog)) {
                //  cout << " no " << bog.str() << " in " << cc.str() << endl;

                res.add(cc);
            } else {
                if (fal) {
                    // cout << " droping " << cc.str() << endl;
                    // droped
                } else {
                    //    cout << " moding " << cc.str() << endl;
                    for (auto s : src.e) {
                        vp x(s);
                        x |= cc;

                        x ^= bog;
                        res.add(x);
                        //    cout << " inserting " << x.str() << endl;
                    }
                }


            }
        }
        //  cout << " resuylt " << res.str() << endl;
        return res;

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

    bool isunsat() const {
        return (e.size() == 1 && (*e.begin()).isTrue());

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

class equation {
private:

public:
    set<vx> dirty;
    set<vx> clean;
    set<vx> bogossed;

    set<vx> process;

private:

    vx diag(int i, bool b, int sz) {
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

    equation(vector<bool> in) {
        for (int i = 0; i < in.size(); i++) {
            dirty.insert(diag(i, in[i], in.size()));
        }
    }

    long getNbProd() {
        long res;
        for (auto x : dirty) {
            res += x.nbProd();
        }

        for (auto x : clean) {
            res += x.nbProd();
        }

        return res;
    }

    long getNbVars() {
        long res;
        for (auto x : dirty) {
            res += x.nbVars();
        }
        for (auto x : clean) {
            res += x.nbVars();
        }

        return res;
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

    void doApplySubstitution(const vp& bog, vx eq) {
        process.insert(dirty.begin(), dirty.end());
        process.insert(clean.begin(), clean.end());

        dirty.clear();
        clean.clear();

        eq.add(bog);

        while (!process.empty()) {
            auto in = *process.begin();

            process.erase(process.begin());

            auto appli = in.applySub(bog, eq);

            //cout << " substi " << bog.str() << " <-> " << eq.str() << " on " << in.str() << endl;
            //cout << " result in " << appli.str() << endl;

            dirty.insert(appli);

        }

    }

    bool brush() {

        if (dirty.empty()) return false;

        auto x = *dirty.begin();

        if (x.isFalse()) {
            dirty.erase(x);
            return true;
        }

        if (x.isunsat()) {
            bogossed.clear();
            bogossed.insert(x);
            return false;
        }

        // Isolated variable
        auto b = x.singleBogoss();



        if (!b.isTrue()) {
            dirty.erase(x);
            bogossed.insert(x);

            doApplySubstitution(b, x);

            return true;
        };


        dirty.erase(x);
        clean.insert(x);
        return true;
        cerr << "INVARIANT BROKEN BRUSH END OF FILE " << endl;
        exit(1);
    }

    vp findFree() {
        for (auto x : clean) {
            if (x.isFalse()) continue;
            auto v = x.vars();
            //  cerr << "vars " << v.str() << endl;

            if (v.isTrue()) continue;

            for (int i = 0; i < 512; i++) {
                if (v[i]) {
                    return vp(i);
                } else {
                    //        cerr << i << " is " << v[i] << endl;

                }
            }
        }
        return vp();
    }

    equation derive(bool phase) {
        equation res = *this;

        auto v = findFree();

        if (!v.isTrue()) {
            if (phase)
                res.dirty.insert(vx(vp(), v));
            else
                res.dirty.insert(vx(v));
        } else {
            cerr << "nothing found" << endl;
        }


        return res;
    }

    vector<bool> extract() {
        dirty.insert(bogossed.begin(), bogossed.end());
        bogossed.clear();

        while (brush()) {
            cout << " extracting " << endl << str() << endl;

        }

        //cout << " extracted " << endl << str() << endl;

        vector<bool> res;
        for (auto x : bogossed) {
            auto b = x.singleBogoss();

            x.add(b);
            bool t = !x.isFalse();

            res.push_back(t);

        }



        return res;
    }

}; // equation

class XSol2 {
    vector<bool> in;

public:

    XSol2(vector<bool> inp) : in(inp) {


    }

    const bool stataff = false;

    vector<equation> match;
    vector<vector<bool>> sat;

    long countBranching = 0;
    long maxdepth = 0;
    long countBrushing = 0;
    double brushtime = 0;

    void recsolve(int depth, equation eq) {

        // cout << " STEPPING " << endl;
        //cout << eq.str() << endl;

        if (depth > maxdepth) maxdepth = depth;

        clock_t start = clock();

        while (eq.brush()) {
            countBrushing++;
            //  cout << "broching " << endl;
            // cout << eq.str();
            if (stataff && countBrushing % 100 == 0) {
                cout << " brushing " << countBrushing << endl;
                cout << " nb prod " << eq.getNbProd() << endl;
                cout << " nb vars " << eq.getNbVars() << endl;
            }

        }

        clock_t end = clock();
        double time = ((double) (end - start))* 1000.0 / CLOCKS_PER_SEC;
        if (time > brushtime) brushtime = time;

        if (!eq.clean.empty()) {

            countBranching++;
            recsolve(depth + 1, eq.derive(false));
            recsolve(depth + 1, eq.derive(true));

        } else {
            if (eq.bogossed.size() > 1) {
                //    cout <<  " PUSHING  RESULT " << endl;

                // sat.push_back(eq.extract());
                match.push_back(eq);
                return;
            } else {
                //  cout << " rejecting <<<<< " << endl;
                // cout << eq.str() << endl;

            }
        }
    }

    vector<vector<bool>> dosolve() {


        match.clear();
        countBranching = 0;

        equation eq(in);

        // cout << eq.str();
        while (eq.brush()) {

            //cout << "broching " << endl;
            //cout << eq.str();

        }
        clock_t start = clock();
        recsolve(0, eq);
        clock_t end = clock();
        double time = ((double) (end - start))* 1000.0 / CLOCKS_PER_SEC;

        // cout << " result " << endl;
        for (auto x : match) {
            // cout << "----------" << endl;
            // cout << x.str() << endl;

        }

        if (stataff) {
            cout << " nb End graph " << match.size() << endl;


            cout << "Tot time " << time << endl;
            cout << " number_branch " << countBranching << " max depth " << maxdepth << endl;
            cout << " number_brosse " << countBrushing << endl;
            cout << " brush time " << brushtime << endl;

            cout << " brush time borne " << brushtime * countBrushing / 1000.0 << endl;
        }
        return sat;
    }


private:

};

#endif	/* XSOL2_H */

