//
//  HashTable.h
//  Hash functions
//
//  global hash function added
//
//  Created by Parker Allen on 5/30/17.
//  Copyright © 2017 Parker Allen. All rights reserved.
//

#ifndef HashTable_h
#define HashTable_h

#include "FHvector.h"
#include "FHlist.h"
#include <cmath>

using namespace std;

int Hash(const string & word){
    const int BASE = 53;
    int hashVal = 0;
    
    long coeff = 1;
    for(int i = 0; i < word.length(); i++){
        hashVal += word[i] * coeff;
        coeff *= BASE;
    }
    return hashVal;
}

template <class T>
class MyHash {
    static const int INIT_TABLE_SIZE = 97;
    static const float INIT_MAX_LAMBDA;
    
private:
    FHvector<FHlist<T> > mLists;
    int mSize;
    int mTableSize;
    float mMaxLambda;
    
public:
    MyHash(int tableSize = INIT_TABLE_SIZE);
    bool contains(const T & x) const;
    void makeEmpty();
    bool insert(const T & x);
    bool remove(const T & x);
    static long nextPrime(long n);
    int size() const { return mSize; }
    bool setMaxLambda( float lm );
    
private:
    void rehash();
    int myHash(const T & x) const;
};

template <class T>
const float MyHash<T>::INIT_MAX_LAMBDA = 1.5;

// MyHash method definitions -------------------
template <class T>
MyHash<T>::MyHash(int tableSize) : mSize(0) {
    if (tableSize < INIT_TABLE_SIZE)
        mTableSize = INIT_TABLE_SIZE;
    else
        mTableSize = nextPrime(tableSize);
    
    mLists.resize(mTableSize);
    mMaxLambda = INIT_MAX_LAMBDA;
}

template <class T>
int MyHash<T>::myHash(const T & x) const {
    int hashVal;
    
    hashVal = Hash(x) % mTableSize;
    if(hashVal < 0)
        hashVal += mTableSize;
    
    return hashVal;
}

template <class T>
void MyHash<T>::makeEmpty() {
    int k, size = mLists.size();
    
    for(k = 0; k < size; k++)
        mLists[k].clear();
    mSize = 0;
}

template <class T>
bool MyHash<T>::contains(const T & x) const {
    const FHlist<T> & theList = mLists[myHash(x)];
    typename FHlist<T>::const_iterator iter;
    
    for (iter = theList.begin(); iter != theList.end(); iter++)
        if (*iter == x)
            return true;
    
    // not found
    return false;
}

template <class T>
bool MyHash<T>::remove(const T & x) {
    FHlist<T> &theList = mLists[myHash(x)];
    typename FHlist<T>::iterator iter;
    
    for (iter = theList.begin(); iter != theList.end(); iter++)
        if (*iter == x)
        {
            theList.erase(iter);
            mSize--;
            return true;
        }
    
    // not found
    return false;
}

template <class T>
bool MyHash<T>::insert(const T & x) {
    typename FHlist<T>::iterator iter;
    FHlist<T> &theList = mLists[myHash(x)];
    
    for (iter = theList.begin(); iter != theList.end(); iter++) {
        if (*iter == x)
            return false;
    }
    
    // not found so we insert
    theList.push_back(x);
    
    // check load factor
    if( ++mSize > mMaxLambda * mTableSize )
        rehash();
    
    return true;
}

template <class T>
void MyHash<T>::rehash() {
    FHvector< FHlist<T> > oldLists = mLists;
    typename FHlist<T>::iterator iter;
    int k, oldTableSize = mTableSize;
    
    mTableSize = nextPrime(2*oldTableSize);
    mLists.resize( mTableSize );
    
    // only the first old_size lists need be cleared
    for(k = 0; k < oldTableSize; k++ )
        mLists[k].clear();
    
    mSize = 0;
    for(k = 0; k < oldTableSize; k++)
        for(iter = oldLists[k].begin(); iter != oldLists[k].end(); iter++)
            insert(*iter);
}

template <class T>
bool MyHash<T>::setMaxLambda(float lam) {
    if (lam < .1 || lam > 100)
        return false;
    mMaxLambda = lam;
    return true;
}

template <class T>
long MyHash<T>::nextPrime(long n) {
    long k, candidate, loopLim;
    
    if (n <= 2 )
        return 2;
    else if (n == 3)
        return 3;
    
    for (candidate = (n % 2 == 0)? n+1 : n ; true ; candidate += 2) {
        loopLim = (long)( (sqrt((float)candidate) + 1)/6 );
        
        if (candidate % 3 == 0)
            continue;
        
        for (k = 1; k <= loopLim; k++) {
            if (candidate % (6*k - 1) == 0)
                break;
            if (candidate % (6*k + 1) == 0)
                break;
        }
        if (k > loopLim)
            return candidate;
    }
}


#endif /* HashTable_h */
