/* 
 * File:   NinXorSolv.h
 * Author: denis
 *
 * Created on 18 octobre 2015, 17:11
 */

#ifndef NINXORSOLV_H
#define	NINXORSOLV_H

    #include <vector>
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
        
public:
    
    NinXorSolv(int szp) : Sz(szp){
        
        
        for(int i=0;i<Sz/2;i++){
            diags.push_back(diag(i));
        }
        
    }
    
    NinXorSolv(const NinXorSolv& that){
        *this=that;
    }
    
    string debug(){
        string res="";
        for(int i=0;i<diags.size(  );i++){
            res+=diags[i].debug()+"\n";
        }
        return res;
    }
    
    static void test();
    
private:
    
    xorExpr diag(int i) {
        xorExpr res;
        
        int y=0;
        for(int x=i;x>=0;x--){
        
            res.e.push_back(Expr(AND,Expr(Var(x)),Expr(Var(y))));
            
            y++;
        }
                
        
        return res;
    
    }
    

    
    
};

#endif	/* NINXORSOLV_H */

