/* 
 * File:   Formulae.h
 * Author: denis
 *
 * Created on 18 octobre 2015, 17:05
 */

#ifndef FORMULAE_H
#define	FORMULAE_H

#include <cstdlib>
#include <iomanip>
#include <bitset>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream> //this is where istringstream is defined
#include <string>
#include <memory>

using namespace std;


    class Var {
    public:

        Var() {
            value = -1;
        }

        Var(int value) : value(value) {
        }
        int value;
        bool bar = false;

        string debug() {
            std::ostringstream sout;
            if (bar) sout << '!';
            sout << value;
            return sout.str();
        }
    };

    enum op {
        AND, OR, XOR, NOT, VAL, VOID
    };
   extern const string optos[];

    class Expr {
    public:
        op type;
        Var val;
        vector<Expr> dat;

    public:

        Expr() {
            type = VOID;
        }

        Expr(Var v) {
            type = VAL;
            val = v;
        }

        Expr(op t, Expr a) {
            type = t;
            dat.push_back(a);
        }

        Expr(op t, Expr a, Expr b) {
            if (t == NOT) {
                cerr << "NOT with two arguments ";
                exit(1);
            };
            type = t;
            dat.push_back(a);
            dat.push_back(b);
        }

        string dpad(int pad) {
            string res = "";
            for (int i = 0; i < pad; i++) res += "   ";
            return res;
        }

        void unxor() {
            if (type == XOR) {
                if (dat.size() != 2) {
                    cerr << "invariant fail XOR a deux arguments" << endl;
                }

                Expr l = dat[0];
                Expr r = dat[1];

                dat.clear();
                type = AND;
                dat.push_back(Expr(OR, l, r));
                dat.push_back(Expr(OR, Expr(NOT, l), Expr(NOT, r)));
            }
            for (int i = 0; i < dat.size(); i++) {
                dat[i].unxor();
            }
        }
        
        void unxor_dis() {
            if (type == XOR) {
                if (dat.size() != 2) {
                    cerr << "invariant fail XOR a deux arguments" << endl;
                }

                Expr l = dat[0];
                Expr r = dat[1];

                dat.clear();
                type = OR;
                dat.push_back(Expr(AND, Expr(NOT, l), r));
                dat.push_back(Expr(AND, Expr(NOT, r), l));
            }
            for (int i = 0; i < dat.size(); i++) {
                dat[i].unxor_dis();
            }
        }        

        void pushnot() {
            if (type == NOT) {
                this->delMono();
                this->invertExpr();
            }
            for (int i = 0; i < dat.size(); i++) {
                dat[i].pushnot();
            }
        }

        void pushor() {
            while (pushor_rec()) {
                // cout << "debug partiel pushor"<<endl;
                // debug(0);   
                //  cout << " fin debug partiel pushor"<<endl;
            };
        }

    private:

        int exprSz() {
            if (type != VAL) return dat.size();

            return 1;
        }

        Expr& valAt(int i) {
            if ((type == VAL && i != 0) || (type != VAL && dat[i].type != VAL)) {
                cerr << " Invariant casse valeur simple only " << endl;
                exit(1);
            }
            if (type == VAL) return *this;
            return dat[i];
        }

        void flattenNodeOnly() {
            if (type != AND && type != OR && type != VAL) {
                cerr << " flatten invariant fail : invert base expr";
                exit(0);
            }

            if (type == VAL) return;
            op me = type;

            while (true) {
                int cc = -1;
                for (int i = 0; i < dat.size(); i++) {
                    if (dat[i].type == me) {
                        cc = i;
                        break;
                    }
                }

                if (cc != -1) {
                    Expr e = dat[cc];
                    dat.erase(dat.begin() + cc);
                    for (int i = 0; i < e.dat.size(); i++) {
                        dat.push_back(e.dat[i]);
                    }
                    continue;
                }

                break;
            }
        }

        void pushor2rec() {
            for (Expr e : dat) {
                pushor2rec();
            }
            flattenNodeOnly();

            if (type != OR) {
                return;
            }


            Expr andOrNode;
            andOrNode.type = AND;
            
            vector<int> sz;
            vector<int> parc;
            for(int i=0;i<dat.size();i++){
                sz.push_back(dat[i].exprSz());
                parc.push_back(0);
            }
            
            while(parc[sz.size()-1]<sz[sz.size()-1]){
                
                Expr orNode;
                orNode.type=OR;
                
                for(int i=0;i<sz.size();i++){
                    orNode.dat.push_back(dat[i].valAt(i));
                }
                andOrNode.dat.push_back(orNode);
                
                for(int i=0;true;i++){
                    parc[i]+=1;
                    if(parc[i]<sz[i]){
                        break;
                    }
                    if(i==(sz.size()-1)){
                        break;
                    }         
                    parc[i]=0;
                }
            }

            this->type=AND;
            this->dat=andOrNode.dat;
        }
    public:

        void pushor2() {
            pushor2rec();



        }

        void flatten() {
            if (type != AND && type != OR && type != VAL) {
                cerr << " flatten invariant fail : invert base expr";
                exit(0);
            }

            if (type == VAL) return;
            op me = type;

            while (true) {
                int cc = -1;
                for (int i = 0; i < dat.size(); i++) {
                    if (dat[i].type == me) {
                        cc = i;
                        break;
                    }
                }

                if (cc != -1) {
                    Expr e = dat[cc];
                    dat.erase(dat.begin() + cc);
                    for (int i = 0; i < e.dat.size(); i++) {
                        dat.push_back(e.dat[i]);
                    }
                    continue;
                }

                break;
            }

            for (int i = 0; i < dat.size(); i++) {
                dat[i].flatten();
            }
        }

        string debug(int pad) {
            std::ostringstream sout;
            
            //sout << "pomme ";
            //return sout.str();
            
            if (type == VAL) {
                sout << dpad(pad);

                sout <<"|" <<val.debug();
                
                return sout.str();
            };

            sout << dpad(pad) << optos[type] << "{" ;
            for (int i = 0; i < dat.size(); i++) {
                sout << dat[i].debug(pad );
            }
            //return sout.str();
            sout << dpad(pad) << "}" ;
            return sout.str();
        }

        void normalizeAndOr() {
            this->unxor();
            this->pushnot();
            cout << "push not done " << endl;
            pushor();
            //pushor2();
            cout << "push or done " << endl;
            flatten();
        }

    private:

        bool pushor_rec() {
            if (type == VAL) return false;
            if (type == AND) {
                bool hasPushed = false;
                for (int i = 0; i < dat.size(); i++) {
                    hasPushed = hasPushed || dat[i].pushor_rec();
                }
                return hasPushed;
            };

            op me = type;

            bool hasPushed = false;

            int cc = -1;
            for (int i = 0; i < dat.size(); i++) {
                if (dat[i].type == AND) {
                    cc = i;
                    break;
                }
            }

            if (cc != -1) {
                Expr head = dat[cc];
                dat.erase(dat.begin() + cc);
                if (dat.size() == 1) {
                    delMono();
                    hasPushed = true;
                }

                Expr nth = *this;
                for (int i = 0; i < head.dat.size(); i++) {
                    Expr old = head.dat[i];
                    head.dat[i] = Expr(OR, head.dat[i], nth);
                    hasPushed = true;
                }
                copy(head);
            }



            for (int i = 0; i < dat.size(); i++) {
                hasPushed = hasPushed || dat[i].pushor_rec();
            }
            return hasPushed;
        }

    public:

        void invertExpr() {
            if (type != AND && type != OR && type != VAL && type != NOT) {
                cerr << "invariant fail : invert base expr";
                exit(0);
            }
            if (type == NOT) {
                this->delMono();
            } else if (type == AND || type == OR) {
                if (type == AND) type = OR;
                else
                    if (type == OR) type = AND;
                for (int i = 0; i < dat.size(); i++) {
                    dat[i].invertExpr();
                }
            } else if (type == VAL) {
                val.bar = !val.bar;
            }
        }

        void copy(Expr e) {
            type = e.type;
            val = e.val;
            dat = e.dat;
        }

        void delMono() {
            if (dat.size() != 1) {
                cerr << "invariant fail : del mono non mono";
                exit(0);
            }

            copy(dat[0]);
        }

    };

#endif	/* FORMULAE_H */

