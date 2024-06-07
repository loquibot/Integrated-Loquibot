#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include "TextArea.h"

using namespace geode::prelude;

class $modify(FLAlertLayer) {

    struct Fields {
        bool m_isDefault;
        bool m_isSpecial;

        char const* m_title;
        std::string m_desc;
        char const* m_btn1;
        char const* m_btn2;
        float m_width;
        bool m_scroll;
        float m_height;
        float m_textScale;
    };

    bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7, float p8) {
        this->m_fields->m_isDefault = true;
        this->m_fields->m_isSpecial = !p1;

        this->m_fields->m_title = p1;
        this->m_fields->m_desc = p2;
        this->m_fields->m_btn1 = p3;
        this->m_fields->m_btn2 = p4;
        this->m_fields->m_width = p5;
        this->m_fields->m_scroll = p6;
        this->m_fields->m_height = p7;
        this->m_fields->m_textScale = p8;
        
        return FLAlertLayer::init(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }


	void show() {
        
        CCDirector* director = CCDirector::sharedDirector();

        unsigned int opacity;
        int zOrder;

        CCScaleTo* scaleTo = CCScaleTo::create(0.5, 1.0);
        CCEaseElasticOut* easeElasticOut = CCEaseElasticOut::create((CCActionInterval*)scaleTo, 0.6);
        CCFadeTo* fadeTo;

        CCScene* runningScene;
        CCScene* nextScene;

        if(m_noElasticity) {
            runningScene = director->m_pRunningScene;
            nextScene = (CCScene*) m_scene;
            if(nextScene == nullptr) {
                zOrder = runningScene->getHighestChildZ() + 1;
                if(zOrder < 105){
                    zOrder = 105;
                }
                m_ZOrder = zOrder;
                nextScene = runningScene;
            }
            if(m_ZOrder == 0) {
                m_ZOrder = 105;
            }
            nextScene->addChild(this, m_ZOrder);
            this->setKeypadEnabled(true);
            return;
        }

        opacity = this->getOpacity();

        m_mainLayer->setScale(0.1f);
        m_mainLayer->runAction(easeElasticOut);

        runningScene = director->m_pRunningScene;
        nextScene = (CCScene*) m_scene;

        if(nextScene == nullptr) {
            zOrder = runningScene->getHighestChildZ() + 1;
            if(zOrder < 105) {
                zOrder = 105;
            }
            m_ZOrder = zOrder;
            nextScene = runningScene;
        }
        if(m_ZOrder == 0) {
            m_ZOrder = 105;
        }

        //fix crash if vanilla alert protocol is nullptr
        
        bool isScroll = !!getChildByIDRecursive("scroll-layer");
        bool hasTextArea = !!getChildByIDRecursive("content-text-area");
        bool hasAlertProtocol = !!m_alertProtocol;
        bool isDefault = this->m_fields->m_isDefault;
        bool isSpecial = this->m_fields->m_isSpecial;

        if(hasAlertProtocol || !isDefault || isSpecial || !hasTextArea || isScroll) {
            nextScene->addChild(this, zOrder);

            this->setOpacity(0);

            fadeTo = CCFadeTo::create(0.14, opacity);
            this->runAction(fadeTo);
            this->setKeypadEnabled(true);
            return;
        }

        if(m_alertProtocol == nullptr && this->m_fields->m_isDefault) {

            auto textNode = getChildByIDRecursive("content-text-area");
            auto titleNode = getChildByIDRecursive("title");

            std::string text = this->m_fields->m_desc;
            std::string titleText = this->m_fields->m_title;

            if(textNode){
                MyTextArea* area = dynamic_cast<MyTextArea*>(textNode);
                if(area){
                    text = area->getString();
                }
            }
            if(titleNode){
                titleText = (dynamic_cast<CCLabelBMFont*>(titleNode))->getString();
            }

            geode::createQuickPopup(this->m_fields->m_title, this->m_fields->m_desc, this->m_fields->m_btn1, this->m_fields->m_btn2, nullptr, true);
        }
	}
};