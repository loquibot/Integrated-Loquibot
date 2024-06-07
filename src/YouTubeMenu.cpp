#include "YouTubeMenu.h"
#include "Loquibot.h"
#include <Geode/utils/web.hpp>

static std::unordered_map<std::string, web::WebTask> RUNNING_REQUESTS {};

void YouTubeMenu::setup() {

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    CCScale9Sprite* insideSprite = CCScale9Sprite::create("GJ_square05-uhd.png");
    insideSprite->setContentSize({320*m_scaleFactor, 180*m_scaleFactor});
    insideSprite->setPosition({winSize.width/2, winSize.height/2 + m_thumbnailTranslation});

    this->m_mainLayer->addChild(insideSprite);

    CCLabelBMFont* thumbnailText = CCLabelBMFont::create("Getting Thumbnail", "bigFont.fnt");
    thumbnailText->setPosition({winSize.width/2, winSize.height/2 + m_thumbnailTranslation});
    thumbnailText->setOpacity(128);
    thumbnailText->setScale(0.5f);

    this->m_mainLayer->addChild(thumbnailText);

    TextArea* titleArea = TextArea::create(m_videoTitle, "bigFont.fnt", 0.4f, 320*m_scaleFactor, {}, 12, false);
    titleArea->setPosition({winSize.width/2, winSize.height/2 - 20});
    titleArea->setAnchorPoint({0.5, 1});
    titleArea->setContentSize({320*m_scaleFactor, titleArea->getContentSize().height});
    
    this->m_mainLayer->addChild(titleArea);

    CCLabelBMFont* viewText = CCLabelBMFont::create(m_videoViews.c_str(), "bigFont.fnt");
    viewText->setPosition({winSize.width/2 - 96.5f, winSize.height/2 - 70});
    viewText->setOpacity(128);
    viewText->setScale(0.25f);
    viewText->setAnchorPoint({0, 0});

    this->m_mainLayer->addChild(viewText);

    CCLabelBMFont* creatorText = CCLabelBMFont::create(m_videoCreator.c_str(), "bigFont.fnt");
    creatorText->setPosition({winSize.width/2 - 96.5f, winSize.height/2 - 80});
    creatorText->setOpacity(128);
    creatorText->setScale(0.3f);
    creatorText->setAnchorPoint({0, 0});

    this->m_mainLayer->addChild(creatorText);

    ButtonSprite* watchSprite = ButtonSprite::create("Watch");

    CCMenuItemSpriteExtra* watchButton = CCMenuItemSpriteExtra::create(watchSprite, this, menu_selector(YouTubeMenu::openURL));
    watchButton->setPosition({winSize.width/2, winSize.height/2 - 100});
    
    CCMenu* menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->addChild(watchButton);

    this->m_mainLayer->addChild(menu);

    downloadImage();
    setTouchEnabled(true);
}

YouTubeMenu* YouTubeMenu::create(std::string thumbnailURL, std::string videoTitle, std::string videoCreator, std::string videoViews, std::string videoURL) {

    auto pRet = new YouTubeMenu();
    pRet->m_thumbnailURL = thumbnailURL;
    pRet->m_videoTitle = videoTitle;
    pRet->m_videoCreator = videoCreator;
    pRet->m_videoViews = videoViews;
    pRet->m_videoURL = videoURL;

    if (pRet && pRet->init(250, 250, "GJ_square01.png", "")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

void YouTubeMenu::downloadImage(){

    YouTubeMenu* self = this;

    auto req = web::WebRequest();

    RUNNING_REQUESTS.emplace("@thumbnailCheck", req.get(m_thumbnailURL.c_str())
        .map([self](web::WebResponse* response){
            if(response->ok()) {
                auto winSize = CCDirector::sharedDirector()->getWinSize();

                CCImage* img = new CCImage();
                img->initWithImageData((void*)response->data().data(), response->data().size());
                img->autorelease();

                CCTexture2D* imgTexture = new CCTexture2D();
                imgTexture->initWithImage(img);
                imgTexture->autorelease();

                CCSprite* pSprite = CCSprite::create();
                pSprite->initWithTexture(imgTexture);
                pSprite->setPosition({winSize.width/2, winSize.height/2 + self->m_thumbnailTranslation});
                
                pSprite->setScale(self->m_scaleFactor);
                CCScale9Sprite* border = CCScale9Sprite::create("videoBorder.png"_spr);
                border->setContentSize({320*self->m_scaleFactor, 180*self->m_scaleFactor});
                border->setPosition({winSize.width/2, winSize.height/2 + self->m_thumbnailTranslation});

                self->m_mainLayer->addChild(pSprite);
                self->m_mainLayer->addChild(border);
            }

            RUNNING_REQUESTS.erase("@thumbnailCheck");
            return *response;
        }
    ));
    
}
void YouTubeMenu::openURL(CCObject* obj){
	web::openLinkInBrowser(m_videoURL);
}

