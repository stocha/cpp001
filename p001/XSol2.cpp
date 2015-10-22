/* 
 * File:   XSol2.cpp
 * Author: denis
 * 
 * Created on 22 octobre 2015, 14:07
 * 
 * Prod (a,b) (bitset)
 * - substitute (x,true| false)  -> si x et false alors prod = false sinon x=false
 * - null prod (false)
 * - empty prod (true)
 * - addAll (prod) ( x | y)
 * 
 * Xor + ( Prod ) : ajoute un produit a un xor
 * sub (x, empty| false | other).  for each prod, each other (si x add prod_i,other - x)
 * 
 */

#include "XSol2.h"



