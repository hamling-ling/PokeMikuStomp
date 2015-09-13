//
//  Singleton.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__Singleton__
#define __PokeMikuStompLib__Singleton__

#include <memory>

template <class T>
class Singleton
{
    public :
    static T& Instance()
    {
        static typename T::singleton_pointer_type s_singleton(T::createInstance()) ;
        
        return getReference(s_singleton) ;
    }
    
    private :
    typedef std::unique_ptr<T> singleton_pointer_type ;
    
    inline static T *createInstance() { return new T() ; }
    
    inline static T &getReference(const singleton_pointer_type &ptr) { return *ptr ; }
    
    protected :
    Singleton() {}
    
    private :
    Singleton(const Singleton &) = delete;
    Singleton& operator=(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton& operator=(Singleton &&) = delete;
};

#endif /* defined(__PokeMikuStompLib__Singleton__) */
