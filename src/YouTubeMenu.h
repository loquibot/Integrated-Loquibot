#pragma once

#ifndef __YOUTUBEMENU_HPP
#define __YOUTUBEMENU_HPP

#include "BrownAlertDelegate.h"

class YouTubeMenu : public BrownAlertDelegate {
    protected:
        std::string m_thumbnailURL;
        std::string m_videoTitle;
        std::string m_videoCreator;
        std::string m_videoViews;
        std::string m_videoURL;
        void downloadImage();
        virtual void setup();
        float m_scaleFactor = 0.6f;
        float m_thumbnailTranslation = 50.0f;
        void openURL(CCObject* obj);
    public:
        static YouTubeMenu* create(std::string thumbnailURL, std::string videoTitle, std::string videoCreator, std::string videoViews, std::string videoURL);
};

#endif