//
//  Config.h
//  Nyangame
//
//  Created by P on 2014/07/04.
//
//

#ifndef __Nyangame__Config__
#define __Nyangame__Config__

#include <iostream>

enum kBlock
{
    // コマの種類の定義
    kBlockRed,
    kBlockBlue,
    kBlockYellow,
    kBlockGreen,
    kBlockGray,
    
    // コマの種類数
    kBlockCount,
};

#define ccsf(...) String::createWithFormat(__VA_ARGS__)->getCString();

#endif /* defined(__Nyangame__Config__) */
