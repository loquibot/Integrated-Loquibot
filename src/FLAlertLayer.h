#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

using namespace geode::prelude;

class $modify(FLAlertLayer) {

    bool m_isDefault = false;

    bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7, float p8) {
        this->m_fields->m_isDefault = true;
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
        
        if(m_alertProtocol != nullptr || !this->m_fields->m_isDefault) {
            nextScene->addChild(this, zOrder);

            this->setOpacity(0);

            fadeTo = CCFadeTo::create(0.14, opacity);
            this->runAction(fadeTo);
            this->setKeypadEnabled(true);
        }
	}
};