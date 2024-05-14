#pragma once

#include <utility>
#include <memory>

namespace anim {
    template<typename T>
    class ValRef{
        std::shared_ptr<T> shared;
        bool shared_owner=false;

        T *val;

    public:
        ValRef(T &&val): ValRef(std::make_shared<T>(std::move(val)))
        {}

        ValRef(const std::shared_ptr<T> &ptr): shared(ptr),
                                         shared_owner(true),
                                         val(ptr.get())
        {}

        /*
        ** Keep *val alive for the lifetime of ValRef
        */
        ValRef(T* val): val(val)
        {};

        operator T&(){return *val;}


        ValRef(const ValRef &other){
            if(other.shared_owner){

                this->shared_owner=true;
                this->shared=other.shared;

                this->val=this->shared.get();
            } else {

                this->val=other.val;
            }
        }

    };

}
