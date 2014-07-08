//
//  GameScene.cpp
//  Nyangame
//
//  Created by P on 2014/07/04.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"
#include "PlaySE.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Scene* GameScene::scene() {
    Scene* scene = Scene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    
    // タップイベントを取得する
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    // 変数初期化
    initForVariables();
    
    // 背景表示
    showBackground();
    
    // コマ表示
    showBlock();
    
    // 効果音の事前読み込み
    SimpleAudioEngine::sharedEngine()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}


// 変数初期化
void GameScene::initForVariables() {
    // 乱数初期化 TODO:要理解
    srand((unsigned)time(NULL));
    
    // コマの一変の長さを取得
    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    m_blocksize = pBlock->getContentSize().height;
    
    // コマ種類の配列生成
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);
    
    // 変数初期化
    m_animating = false;
}


// タッチ開始イベント
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent){
    
    // アニメーション中はタップ処理を受け付けない
    return !m_animating;
}
// タッチ終了イベント
void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent){
    // タップポイント取得
    Point touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    // タップしたコマのTagとコマの種類を取得
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    
    if (tag != 0) {
        // 隣接するコマを検索する
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        
        if (sameColorBlockTags.size() > 1) {
            // アニメーション開始（アニメーション中フラグを立てる）
            m_animating = true;
            
            // 隣接するコマを削除する
            removeBlock(sameColorBlockTags, blockType);
            
            // コマ削除後のアニメーション
            movingBlocksAnimation1(sameColorBlockTags);
        }
    }

}

// 背景表示
void GameScene::showBackground() {
    Size winSize = Director::sharedDirector()->getWinSize();
    
    // 背景を生成
    m_background = Sprite::create(PNG_BACKGROUND);
    m_background->setPosition(winSize.width / 2,winSize.height / 2);
    addChild(m_background, kZOrderBackground, kTagBackground);
    
}

// 位置取得
Point GameScene::getPosition(int posIndexX, int posIndexY) {
    float offsetX = m_background->getContentSize().width * 0.168;
    float offsetY = m_background->getContentSize().height * 0.029;
    
    // 配置するインデックスの中心座標を取得してるっぽい
    return Point((posIndexX + 0.5) * m_blocksize + offsetX, (posIndexY + 0.5) * m_blocksize + offsetY);
}

// タグ取得
int GameScene::getTag(int posIndexX, int posIndexY) {
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

// コマのインデックス取得
GameScene::PositionIndex GameScene::getPositionIndex(int tag) {
    int pos1_x = (tag - kTagBaseBlock) / 100;
    int pos1_y = (tag - kTagBaseBlock) % 100;
    
    return PositionIndex(pos1_x, pos1_y);
}


// コマ表示
void GameScene::showBlock() {
    // 8 × 8 のコマを作成する
    for (int x = 0; x < MAX_BLOCK_X; x++) {
        for (int y = 0; y < MAX_BLOCK_Y; y++) {
            // ランダムでコマを作成 TODO:要理解
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            // 対応するコマ配列にタグを追加
            int tag = getTag(x, y);
            m_blockTags[blockType].push_back(tag);
            
            // コマを作成
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            m_background->addChild(pBlock, kZOrderBlock, tag);
        }
    }
}

//タップされたコマのタグを取得
void GameScene::getTouchBlockTag(Point touchPoint, int &tag, kBlock &blockType){
    for (int x = 0; x < MAX_BLOCK_X; x++) {
        for (int y = 0; y < MAX_BLOCK_Y; y++) {
            int currentTag = getTag(x,y);
            Node* node = m_background->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint)) {
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
                
                return;
            }
        }
    }
}

// コマ配置にあるか検索
bool GameScene::hasSameColorBlock(list<int> blockTagList, int searchBlockTag){
    list<int>::iterator it;
    for (it = blockTagList.begin(); it != blockTagList.end(); ++it) {
        if (*it == searchBlockTag) {
            return true;
        }
    }
    return false;
}

// タップされたコマと同色で且つ接しているコマの配列を返す
list<int> GameScene::getSameColorBlockTags(int baseTag, kBlock blockType){
    // 同色のコマを格納する配列の初期化
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end()) {
        int tags[] = {
            *it + 100,  // right block tag
            *it - 100,  // left block tag
            *it + 1,    // up block tag
            *it - 1,    // down block tag
        };
        
        for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++) {
            // すでにリストにあるか検索
            if (!hasSameColorBlock(sameColorBlockTags, tags[i])) {
                // コマ配列にあるか検索
                if (hasSameColorBlock(m_blockTags[blockType], tags[i])) {
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        it++;
    }
    return sameColorBlockTags;
}

// 配列のコマを削除
void GameScene::removeBlock(list<int> blockTags, kBlock blockType){
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while (it != blockTags.end()) {
        // 既存配列から該当コマを削除
        m_blockTags[blockType].remove(*it);
        
        // 対象となるコマを取得
        Node* block = m_background->getChildByTag(*it);
        if (block){
            // コマが消えるアニメーションを生成
            ScaleTo* scale = ScaleTo::create(REMOVING_TIME, 0);
            
            // コマを削除するアクションを生成
            CallFuncN* func = CallFuncN::create(this, callfuncN_selector(GameScene::removingBlock));
            
            //アクションをつなげる
            FiniteTimeAction* sequence = sequence = Sequence::create(scale, func, NULL);
            
            FiniteTimeAction* action;
            if (first) {
                // コマが消えるサウンドアクションを生成
                CCPlaySE* playSe = CCPlaySE::create(MP3_REMOVE_BLOCK);
                
                // アクションをつなげる
                action = CCSpawn::create(sequence, playSe, NULL);
                
                first = false;
            } else {
                action = sequence;
            }
            
            // アクションをセットする
            // これによりblockオブジェクト（対象となるコマのBlockSpriteオブジェクト？）のアクションが実行される
            // CallFuncN で引数として渡したthisの実態は、このblockオブジェクトになる（ぽいんだけど正直ちゃんとわかってない）
            block->runAction(action);
//            block->removeFromParentAndCleanup(true);
        }
        
        it++;
    }
    
    SimpleAudioEngine::sharedEngine()->playEffect(MP3_REMOVE_BLOCK);
}

// コマの削除
void GameScene::removingBlock(Node* block){
    block->removeFromParentAndCleanup(true);
}

// 新しい位置をセット(第一引数：対象となるコマのタグ値 第二引数：対象となるコマの現在のインデックス)
void GameScene::setNewPosition1(int tag, PositionIndex posIndex){
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if (nextPosY == -1) {
        nextPosY = posIndex.y;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(posIndex.x, --nextPosY);
}


// 消えたコマを埋めるように新しい位置をセット
void GameScene::searchNewPosition1(list<int> blocks){
    // 消えるコマ数分のループ
    list<int>::iterator it1 = blocks.begin();
    while (it1 != blocks.end()) {
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        // コマ種類のループ
        vector<kBlock>::iterator it2 = blockTypes.begin();
        while (it2 != blockTypes.end()) {
            // 各種類のコマ数分のループ
            list<int>::iterator it3 = m_blockTags[*it2].begin();
            while (it3 != m_blockTags[*it2].end()) {
                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if (posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y) {
                    // 消えるコマの上に位置するコマに対して、移動先の位置をセットする
                    setNewPosition1(*it3, posIndex2);
                }
                it3++;
            }
            it2++;
        }
        it1++;
    }
}

// コマを移動する
void GameScene::moveBlock(){
    // コマ種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        // 各種類のコマ数分のループ
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(*it2);
            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();
            
            if (nextPosX != -1 || nextPosY != -1) {
                // 新しいタグをセットする
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);
                
                // タグ一覧の値も新しいタグに変更する
                *it2 = newTag;
                
                // アニメーションをセットする
                MoveTo* move = MoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
            }
            it2++;
        }
        it1++;
    }
}

// コマ削除後のアニメーション
void GameScene::movingBlocksAnimation1(list<int> blocks){
    // コマの新しい位置をセットする
    searchNewPosition1(blocks);
    
    // 新しい位置がセットされたコマのアニメーション
    moveBlock();
    
    // アニメーション終了時に次のアニメーション処理を開始する
    scheduleOnce(schedule_selector(GameScene::movingBlocksAnimation2), MOVING_TIME)
}

// コマの移動完了
void GameScene::movedBlocks(){
    // アニメーション終了
    m_animating = false;
}

// 新しい位置をセット
void GameScene::setNewPosition2(int tag, GameScene::PositionIndex posIndex){
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosY();
    if (nextPosX == -1) {
        nextPosX = posIndex.x;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(--nextPosX, posIndex.y);
}

// 存在する列を取得する
map<int, bool> GameScene::getExistsBlockColumn(){
    // 検索配列初期化
    map<int, bool> xBlock;
    for (int i = 0; i < MAX_BLOCK_X; i++) {
        xBlock[i] = false;
    }
    
    // コマ種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        // 各種類のコマ数分のループ
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            // 存在するコマのx位置を記録
            xBlock[getPositionIndex(*it2).x] = true;
            
            it2++;
        }
        it1++;
    }
    return xBlock;
}

// 消えたコマ列を埋めるように新しい位置をセット
void GameScene::searchNewPosition2(){
    // 存在する列を取得する
    map<int, bool> xBlock = getExistsBlockColumn();
    
    // コマが存在しないx位置を埋める
    bool first = true;
    for (int i = MAX_BLOCK_X - 1; i >= 0; i--) {
        if (xBlock[i]) {
            // コマが存在する
            first = false;
            continue;
        } else {
            // コマが存在しない
            if (first){
                // 右側にコマがない
                continue;
            } else {
                // この位置より右側にあるコマを左に１つ寄せる
                // コマ種類のループ
                vector<kBlock>::iterator it1 = blockTypes.begin();
                while (it1 != blockTypes.end()) {
                    // 各種類のコマ数分のループ
                    list<int>::iterator it2 = m_blockTags[*it1].begin();
                    while (it2 != m_blockTags[*it1].end()) {
                        PositionIndex posIndex = getPositionIndex(*it2);
                        
                        if (i < posIndex.x) {
                            // 移動先の位置をセットする
                            setNewPosition2(*it2, posIndex);
                        }
                        it2++;
                    }
                    it1++;
                }
            }
        }
    }
}

// コマのアニメーション
void GameScene::movingBlocksAnimation2(){
    // コマの新しい位置をセットする
    searchNewPosition2();
    
    // 新しい位置がセットされたコマのアニメーション
    moveBlock();
    
    // アニメーション終了時に次の処理を開始する
    scheduleOnce(schedule_selector(GameScene::movedBlocks), MOVING_TIME);
}



