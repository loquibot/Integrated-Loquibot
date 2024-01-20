
#include "easywsclient.hpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <document.h>
#include "ServerListener.h"
#include "Loquibot.h"
#include <Geode/Geode.hpp>
#include <thread>
#include "GlobalVars.h"

using namespace cocos2d;

using namespace easywsclient;

std::unique_ptr<WebSocket> ws(nullptr);

void ServerListener::connectAsync() {
    std::thread t = std::thread(connect);
    t.detach();
}


void ServerListener::connect(){

    open();

    using easywsclient::WebSocket;

#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        return;
    }
#endif

    if (ws) {
        GlobalVars::getSharedInstance()->loquiOpen = true;

        while (ws->getReadyState() != WebSocket::CLOSED) {
            WebSocket::pointer wsp = &*ws; // a unique_ptr cannot be copied into a lambda
            ws->poll();
            ws->dispatch([wsp](const std::string& message) {
                onMessage(message);
                });
        }
    }

    //closed

    GlobalVars::getSharedInstance()->loquiOpen = false;
    ws.release();

#ifdef _WIN32
    WSACleanup();
#endif

}


void ServerListener::onMessage(std::string message) {
    geode::Loader::get()->queueInMainThread([message]() {


        geode::log::info("{}", message);

        if (GlobalVars::getSharedInstance()->isButtonPressed) {
            rapidjson::Document levelJson;
            levelJson.Parse<0>(message.c_str());

            std::string service = "none";

            if (levelJson.HasMember("service")) {
                rapidjson::Value& valueS = levelJson["service"];
                service = valueS.GetString();
            }

            if (service == "gd") {
                bool badUpdate = false;

                if (levelJson.HasMember("version")) {
                    rapidjson::Value& valueS = levelJson["version"];
                    int version = valueS.GetInt();

                    if(!(version >= 2)){
                        badUpdate = true;
                    }
                }
                else{
                    badUpdate = true;
                }

                if(badUpdate){
                    auto alertLayer = FLAlertLayer::create(nullptr, "Bad Version!", "Your Loquibot App is out of date!\nGo to <cg>loquibot.com/versionHelp.html</c> for more info.", "Okay", nullptr, 300);
                    alertLayer->show();
                    return;
                }

                if (levelJson.HasMember("status")) {

                    rapidjson::Value& valueS = levelJson["status"];
                    std::string status = valueS.GetString();

                    if(status == "empty"){
                        auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "The queue is empty!", "Okay", nullptr, 300);
                        alertLayer->show();
                        return;
                    }

                }

                if (levelJson.HasMember("next_status")) {

                    rapidjson::Value& valueS = levelJson["next_status"];
                    std::string status = valueS.GetString();

                    if(status == "empty"){
                        auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "The queue is empty!", "Okay", nullptr, 300);
                        alertLayer->show();

                        Loquibot::getSharedInstance()->showButtons(GlobalVars::getSharedInstance()->lastLayer);

                        return;
                    }

                }

                std::string creator = "-";
                std::string requester = "-";
                std::string name = "Unknown";

                GlobalVars::getSharedInstance()->isButtonPressed = false;

                rapidjson::Value& nameValue = levelJson["name"];
                name = nameValue.GetString();

                rapidjson::Value& creatorValue = levelJson["creator"];
                creator = creatorValue.GetString();
                GlobalVars::getSharedInstance()->creator = creator;

                rapidjson::Value& requesterValue = levelJson["requester"];
                requester = requesterValue.GetString();
                GlobalVars::getSharedInstance()->requester = requester;

                rapidjson::Value& idValue = levelJson["id"];
                int ID = idValue.GetInt();

                rapidjson::Value& songIDValue = levelJson["songID"];
                int songID = songIDValue.GetInt();

                rapidjson::Value& accountIDValue = levelJson["accountID"];
                int accountID = accountIDValue.GetInt();
                GlobalVars::getSharedInstance()->accountID = accountID;

                rapidjson::Value& isCustomSongValue = levelJson["isCustomSong"];
                bool isCustomSong = isCustomSongValue.GetBool();

                rapidjson::Value& likesValue = levelJson["likes"];
                int likes = likesValue.GetInt();

                rapidjson::Value& downloadsValue = levelJson["downloads"];
                int downloads = downloadsValue.GetInt();

                rapidjson::Value& lengthValue = levelJson["lengthValue"];
                int length = lengthValue.GetInt();

                rapidjson::Value& isDemonValue = levelJson["isDemon"];
                bool isDemon = isDemonValue.GetBool();

                rapidjson::Value& isAutoValue = levelJson["isAuto"];
                bool isAuto = isAutoValue.GetBool();

                rapidjson::Value& isEpicValue = levelJson["isEpic"];
                bool isEpic = isEpicValue.GetBool();

                rapidjson::Value& featuredScoreValue = levelJson["featuredScore"];
                int featuredScore = featuredScoreValue.GetInt();

                rapidjson::Value& difficultyValue = levelJson["difficulty"];
                int difficulty = difficultyValue.GetInt();

                rapidjson::Value& demonDifficultyValue = levelJson["demonDifficulty"];
                int demonDifficulty = demonDifficultyValue.GetInt();

                rapidjson::Value& starsValue = levelJson["stars"];
                int stars = starsValue.GetInt();

                CCObject* level = GameLevelManager::sharedState()
                    ->m_onlineLevels
                    ->objectForKey(std::to_string(ID));

                if(ID == GlobalVars::getSharedInstance()->currentID){
                    auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "There are no more levels in the queue!", "Okay", nullptr, 300);
                    alertLayer->show();

                    Loquibot::getSharedInstance()->showButtons(GlobalVars::getSharedInstance()->lastLayer);

                    return;
                }


                if (level != nullptr) GlobalVars::getSharedInstance()->levelData = reinterpret_cast<GJGameLevel*>(level);
                else {


                    GlobalVars::getSharedInstance()->levelData = GJGameLevel::create();

                    GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;

                    levelData->m_accountID = accountID;
                    levelData->m_levelID = ID;
                }

                GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;

                levelData->m_levelName = name;
                levelData->m_creatorName = creator;

                levelData->m_likes = likes;
                levelData->m_downloads = downloads;
                levelData->m_levelLength = length;
                levelData->m_stars = stars;
                levelData->m_featured = featuredScore;
                levelData->m_isEpic = isEpic;
                levelData->m_autoLevel = isAuto;

                if (isCustomSong) levelData->m_songID = songID;
                levelData->m_audioTrack = songID;

                levelData->m_levelType = GJLevelType::Saved;
                GlobalVars::getSharedInstance()->currentID = ID;

                LevelInfoLayer* layer = LevelInfoLayer::create(levelData, false);

                if (levelData->m_levelString.size() == 0) {
                    levelData->m_levelNotDownloaded = true;
                    layer->downloadLevel();
                }
                auto requesterLabel = reinterpret_cast<CCLabelBMFont*>(layer->getChildByTag(357832));

                requesterLabel->setColor(ccGREEN);
                requesterLabel->setString(("Sent By " + requester).c_str());

                auto winSize = CCDirector::sharedDirector()->getWinSize();

                requesterLabel->setPosition({ winSize.width / 2,
                    winSize.height - 60 });

                auto scene = CCScene::create();
                scene->addChild(layer);

                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
            }
        }
    });
}

void ServerListener::sendMessage(std::string message) {
   
    if (GlobalVars::getSharedInstance()->loquiOpen) {
        ws->send(message);
    }
}

void ServerListener::open() {

    ws.release();
    ws.reset(WebSocket::from_url("ws://127.0.0.1:19236"));

}