#ifndef _EFFECT_COMMON_HPP
#define _EFFECT_COMMON_HPP

#define listSize 32  //~ May be changed, I estimated that 32 was enough

template <typename Param>
struct Effect {
    Param listParams[listSize];
    void* funct(Param);
};

#endif //_EFFECT_COMMON_HPP
