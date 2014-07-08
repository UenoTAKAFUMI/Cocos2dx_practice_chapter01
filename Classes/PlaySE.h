//
//  PlaySE.h
//  Nyangame
//
//  Created by P on 2014/07/05.
//
//

#ifndef __Nyangame__PlaySE__
#define __Nyangame__PlaySE__

#include "cocos2d.h"

class CCPlaySE : public cocos2d::ActionInstant
{
public:
    CCPlaySE(std::string sound);
    virtual ~CCPlaySE(){}
    
    virtual void update(float time);
    
    // copyWithZoneメソッドは3.0以降消滅
//    virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* pZone);
    
    // ver2系からの変更点を確認して実装してみる
    // どうやらActionInstantクラスを返す純粋仮想関数が実装されているので、
    // ActionInstantクラスは抽象クラスっぽい
    virtual ActionInstant * reverse(void) const;
    virtual ActionInstant* clone(void) const;
    
public:
    static CCPlaySE* create(std::string sound);
    
protected:
    std::string m_sound;
};


#endif /* defined(__Nyangame__PlaySE__) */
