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
    set<int> e;


public:

    vp() {
       
    }

    vp(int i, int j) {
        e.insert(i);
        e.insert(j);
    }

    vp(int i) {       
        e.insert(i);
    }

    vp(const vp& ot) {
        e = ot.e;
    }

    vp(const set<int>& ot) {
        e = ot;
    }

    bool contains(const vp& ot) const {
        for( auto x : ot.e){
            if(e.find(x)==e.end()) return false;
        }

        return true;
    }

    bool isTrue()const noexcept {
        return e.empty();
    }

    int count()const {
        return e.size();
    }

    void operator|=(const vp& other)noexcept {
        for(auto x : other.e){
            e.insert(x);
        }
    }

    void operator&=(const vp& other)noexcept {
        set<int> res;
        
        for(auto x: other.e){
            if(e.find(x)!=e.end()){
                res.insert(x);
            }
        }
        e=res;
    }
    
    void andNotIn(const vp& other)noexcept {
        set<int> res;
        
        for(auto x: e){
            if(other.e.find(x)==other.e.end()){
                res.insert(x);
            }
        }
        e=res;
    }    

    void operator^=(const vp& other)noexcept {
        set<int> res;
        
        for(auto x: other.e){
            if(e.find(x)==e.end()){
                res.insert(x);
            }
        }
        for(auto x: e){
            if(other.e.find(x)==other.e.end()){
                res.insert(x);
            }
        }        
        
        e=res;        
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

    string str() const {
        std::ostringstream sout;

        bool t = false;
        for (auto x : e) {
            if (t) sout << "." << x;
            else  {
                sout << x;
                t = true;
            }


        }
        if (isTrue()) sout << "T";


        return sout.str();
    }


};

class vptwice {
    vp a;
    vp c;

public:

    vptwice() {
      // cout << "create vptwice " << endl; 
        
    }

    void add(const vp it) {
        c |= a&it;
        a |= it;

       // cout << " add " << it.str() << " curr a " << a.str() << " c " << c.str() << endl;
    }

    vp unique() {
       // cout << "vptwice unique a " << a.str() << " c " << c.str() << endl; 
        
        vp cf=a;
        cf.andNotIn(c);
        return cf;
    }

    vp vars() {
        vp cf = c;
        cf |= a;
        return cf;
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
      //  cout << "is unsat " << (e.size()) << "   " << (*e.begin()).isTrue() << endl;
        
        return (e.size() == 1 && (*e.begin()).isTrue());

    }

    vp isolatedBogoss() const {
        if (e.size() > 2) return vp();

        if (e.size() == 2 && !(*e.begin()).isTrue() ) return vp();

        if (e.size() == 2) return (*(++e.begin()));

        return (*e.begin());
    }

    vp singleBogoss() const {
        vp un = uniqueCandidate();
        
     //   cout << "singlebog un candidate " << un.str() << endl;
        
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
        long res=0;
        for (auto x : dirty) {
            res += x.nbProd();
        }

        for (auto x : clean) {
            res += x.nbProd();
        }

        return res;
    }

    long getNbVars() {
        long res=0;
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

    bool brushExtract() {
        if (dirty.empty()) return false;

        auto x = *dirty.begin();
        
        if (x.isunsat()) {

            
            bogossed.clear();
            dirty.clear();
            clean.clear();
            bogossed.insert(x);
            
          //  cout << " UNSAT DETECTED " << endl;
          //  cout << str() << endl;            
            return false;
        }        

        if (x.isFalse()) {
            dirty.erase(x);
            return true;
        }

        auto b = x.isolatedBogoss();
        if (!b.isTrue()) {
            dirty.erase(x);
            bogossed.insert(x);

            doApplySubstitution(b, x);

            return true;
        };
        dirty.erase(x);
        clean.insert(x);

        return true;
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
            dirty.clear();
            clean.clear();
            bogossed.insert(x);
            
          //  cout << " UNSAT DETECTED " << endl;
          //  cout << str() << endl;            
            return false;
        }

        // Isolated variable
        auto b = x.singleBogoss();

      //  cout << " brush single extract " << b.str() << endl; 

        if (!b.isTrue()) {
            dirty.erase(x);
            bogossed.insert(x);

            doApplySubstitution(b, x);

            return true;
        };


        dirty.erase(x);
        clean.insert(x);
        return true;

    }

    vp findFree() {
        for (auto x : clean) {
            if (x.isFalse()) continue;
            auto v = x.vars();
            //  cerr << "vars " << v.str() << endl;

            if (v.isTrue()) continue;
            
            
            return vp(*(v.e.begin()));
           
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
            cerr << "nothing found equation derive" << endl;
            cerr << str() << endl;
            exit(0);
        }


        return res;
    }

    vector<bool> extract() {

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

    const bool stataff = true;
    const int statbrushingshowfreq = 1000;

    vector<equation> match;
    vector<vector<bool>> sat;

    vector<equation> finalized;

    long countBranching = 0;
    long maxdepth = 0;
    long countBrushing = 0;
    double brushtime = 0;

    void recfinalize(equation eq) {

        while (eq.brushExtract()) {

                 //   cout <<  " BRUSHING   RESULT " << endl;
                    
                 //   cout << eq.str() << endl;
        }




        if (!eq.clean.empty()) {

            recfinalize(eq.derive(false));
            recfinalize(eq.derive(true));

        } else {
            if (eq.bogossed.size() > 1) {
                  //  cout <<  " PUSHING  FINALISED RESULT " << endl;
                    
                  //  cout << eq.str() << endl;

                sat.push_back(eq.extract());
                finalized.push_back(eq);
                return;
            } else {
                //  cout << " rejecting <<<<< " << endl;
                // cout << eq.str() << endl;

            }
        }
    }

    void recsolve(int depth, equation eq) {
        
       // // // if(depth > 4) exit(0);

       //  cout << " STEPPING " << endl;
       // cout << eq.str() << endl;

        if (depth > maxdepth) maxdepth = depth;

        clock_t start = clock();

        while (eq.brush()) {
            countBrushing++;

            if (stataff && countBrushing % statbrushingshowfreq == 1) {
            //  cout << "broching recsolve " << endl;
            // cout << eq.str();                
                
                cout << " brushing " << countBrushing << endl;
                cout << " nb prod " << eq.getNbProd() << endl;
                cout << " nb vars " << eq.getNbVars() << endl;
                cout << " curr depth " << depth << " max " << maxdepth << endl;
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

     //   cout << "DO SOLVE " << endl;
     //    cout << eq.str();
       // while (eq.brush()) {

            //cout << "broching " << endl;
            //cout << eq.str();

        //}
        
                
        clock_t start = clock();
        recsolve(0, eq);
        clock_t end = clock();
        double time = ((double) (end - start))* 1000.0 / CLOCKS_PER_SEC;
        


        // cout << " result " << endl;
        for (auto x : match) {
             //cout << "----------" << endl;
             //cout << x.str() << endl;
            
            x.dirty.insert(x.bogossed.begin(),x.bogossed.end());
            x.dirty.insert(x.clean.begin(),x.clean.end());
            x.bogossed.clear();
            x.clean.clear();
            recfinalize(x);

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

