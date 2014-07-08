//
//  GameScene.h
//  Nyangame
//
//  Created by P on 2014/07/04.
//
//

#ifndef __Nyangame__GameScene__
#define __Nyangame__GameScene__

#include "cocos2d.h"
#include "Config.h"

#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8
#define REMOVING_TIME 0.1f
#define MOVING_TIME 0.2f

#define PNG_BACKGROUND "background.png"
#define MP3_REMOVE_BLOCK "removeBlock.mp3"

#define FONT_RED "redFont.fnt"
#define FONT_BLUE "blueFont.fnt"
#define FONT_YELLOW "yellowFont.fnt"
#define FONT_GREEN "greenFont.fnt"
#define FONT_GRAY "grayFont.fnt"
#define FONT_WHITE "whiteFont.fnt"

class GameScene : public cocos2d::Layer
{
protected:
    // 画像タグ
    enum kTag
    {
        kTagBackground = 1,
        kTagRedLabel,
        kTagBlueLabel,
        kTagYellowLabel,
        kTagGreenLabel,
        kTagGrayLabel,
        kTagScoreLabel,
        kTagBaseBlock = 10000,
    };
    
    // Zオーダー
    enum kZOrder
    {
        kZOrderBackground,
        kZOrderLabel,
        kZOrderBlock,
    };
    
    // コマの位置（インデックス）
    struct PositionIndex {
        PositionIndex(int x1, int y1){
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    cocos2d::Sprite* m_background;
    void showBackground();
    
    float m_blocksize;
    std::map<kBlock, std::list<int> > m_blockTags;
    void initForVariables();
    void showBlock();
    cocos2d::Point getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    
    void getTouchBlockTag(cocos2d::Point touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);
    
    void removingBlock(cocos2d::Node* block);
    
    std::vector<kBlock> blockTypes;
    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveBlock();
    void movingBlocksAnimation1(std::list<int> blocks);
    
    bool m_animating;

    // V3.1対応のためfloat型の仮引数を入れてみる
    void movedBlocks(float delta);

    std::map<int, bool> getExistsBlockColumn();
    void searchNewPosition2();
    void setNewPosition2(int tag, PositionIndex posIndex);

    // V3.1対応のためfloat型の仮引数を入れてみる
    void movingBlocksAnimation2(float delta);
    
    void showLabel();
    
    int m_score;
    
public:
    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(GameScene);
    
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
};

#endif /* defined(__Nyangame__GameScene__) */
