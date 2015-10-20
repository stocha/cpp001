/* 
 * File:   NinXorSolv.h
 * Author: denis
 *
 * Created on 18 octobre 2015, 17:11
 */

#ifndef NINXORSOLV_H
#define	NINXORSOLV_H

    #include <vector>
#include <algorithm>    // std::min

using namespace std;

#include "NinInterf.h"
#include "Formulae.h"


class xorExpr {
    
    
public : 
    


    vector<Expr> e;
    bool neg=false;
    
    string debug(){
        string res="(";
        
        //cerr << " e.sz "<<e.size()<<endl;
        for(int i=0;i<e.size();i++){
           
            
             res+=e[i].debug(0)+" ) XOR (" ;
        }
        
        return res+")";
    
    }
};

class NinXorSolv {
public:
    
    int Sz;
private :
        vector<xorExpr> diags;
        vector<Expr> solvedVar;
        bitField bound;
        bitField ovec;
public:
    
    NinXorSolv(int szp, bitField ovecp) : Sz(szp), solvedVar(szp),bound(szp), ovec(szp){
        
        ovec=ovecp;
        for(int i=0;i<Sz;i++){
            diags.push_back(diag(i));
        }
        
    }
    
    NinXorSolv(const NinXorSolv& that) : bound(that.Sz), ovec(that.Sz){
        *this=that;
    }
    
    string debug(){
        string res="";
        for(int i=0;i<diags.size(  );i++){
            res+=diags[i].debug();
            res+=" <-> ";
            res+=(ovec[i]?"T":"F");
            res+=+"\n";
        }
        return res;
    }
    
    static void test();
    
private:
    
    xorExpr diag(int i) {
        xorExpr res;
        
        int half=Sz/2;
        
        int y=max(0,i - half+1 );
        for(int x=min (i,half-1);x>=0 && y<half;x--){
        
            res.e.push_back(Expr(AND,Expr(Var(x)),Expr(Var(y+half))));
            
            y++;
        }
                
        
        return res;
    
    }
    

    
    
};

#endif	/* NINXORSOLV_H */

