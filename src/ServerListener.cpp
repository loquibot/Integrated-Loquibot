
#include "easywsclient.hpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include "ServerListener.h"
#include "Loquibot.h"
#include <Geode/Geode.hpp>
#include <thread>
#include "GlobalVars.h"
#include "RequestsLayer.h"
#include "GJGameLevel.h"
#include "HelpAlertProtocol.h"
#include "YouTubeMenu.h"

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

GJGameLevel* parseJsonToGameLevel(matjson::Value);

void ServerListener::onMessage(std::string message) {
    geode::Loader::get()->queueInMainThread([message]() {

        matjson::Value levelJson = matjson::parse(message.c_str());

        //geode::log::info("{}", message);

        if (levelJson.contains("type")) {


            std::string type = levelJson["type"].as_string();

            if(type == "level_list"){
                
                CCArray* arr = CCArray::create();

                matjson::Array levelArray = levelJson["levels"].as_array();
                bool levelsEnabled = true;
                if(levelJson.contains("levels_enabled")){
                    levelsEnabled = levelJson["levels_enabled"].as_bool();
                }

                for (int i = 0; i < levelArray.size(); i++){

                    GJGameLevel* level = parseJsonToGameLevel(levelArray.at(i));

                    level->m_unkInt = i+1;
                    arr->addObject(level);
                }

                GlobalVars::getSharedInstance()->currentLevelList = levelArray;

                CCScene* scene = RequestsLayer::scene(arr, levelsEnabled);

                auto transition = CCTransitionFade::create(0.5f, scene);
            
                CCDirector::sharedDirector()->pushScene(transition);

            }
            if(type == "update_toggle"){
                bool levelsEnabled = levelJson["levels_enabled"].as_bool();
                RequestsLayer* reqLayer = RequestsLayer::get();
                if(reqLayer){
                    reqLayer->updateToggle(levelsEnabled);
                }
            }
            if(type == "youtube_info"){
                std::string videoTitle = levelJson["title"].as_string();
                std::string videoCreator = levelJson["creator"].as_string();
                std::string videoViews = levelJson["views"].as_string();
                std::string videoID = levelJson["id"].as_string();

                std::string thumbnailURL = fmt::format("https://img.youtube.com/vi/{}/maxresdefault.jpg", videoID);
                std::string videoURL = fmt::format("https://www.youtube.com/watch?v={}", videoID);

                YouTubeMenu* menu = YouTubeMenu::create(thumbnailURL, videoTitle, videoCreator, videoViews, videoURL);
                menu->show();
            }
            if(type == "has_youtube"){
                int id = levelJson["id"].as_int();

                CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

				LevelInfoLayer* levelInfoLayer = dynamic_cast<LevelInfoLayer*>(currentScene->getChildren()->objectAtIndex(0));

                if(levelInfoLayer){
                    if(((LevelInfoLayer*)levelInfoLayer)->m_level->m_levelID == id){
                        GlobalVars::getSharedInstance()->idWithYouTube = id;
                        Loquibot::getSharedInstance()->showYouTube(levelInfoLayer);
                    }
                }

            }
            return;
        }

        if (GlobalVars::getSharedInstance()->isButtonPressed) {
            
            std::string service = "none";

            if (levelJson.contains("type")) {
                return;
            }

            if (levelJson.contains("service")) {
                service = levelJson["service"].as_string();
            }

            if (service == "gd") {
                bool badUpdate = false;

                if (levelJson.contains("version")) {
                    int version = levelJson["version"].as_int();

                    if(!(version >= 2)){
                        badUpdate = true;
                    }
                }
                else{
                    badUpdate = true;
                }

                if(badUpdate){
                    auto alertLayer = FLAlertLayer::create(new HelpAlertProtocol, "Bad Version!", "Your Loquibot App is out of date!\nClick <cg>Help</c> for more info.", "Okay", "Help", 300);
                    alertLayer->show();
                    return;
                }

                if (levelJson.contains("status")) {

                    std::string status = levelJson["status"].as_string();

                    if(status == "empty"){
                        auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "The queue is empty!", "Okay", nullptr, 300);
                        alertLayer->show();
                        return;
                    }

                }

                if (levelJson.contains("next_status")) {

                    std::string status = levelJson["next_status"].as_string();

                    if(status == "empty"){
                        auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "There are no more levels in the queue!", "Okay", nullptr, 300);
                        alertLayer->show();

                        Loquibot::getSharedInstance()->showButtons();

                        return;
                    }

                }

                std::string creator = "-";
                std::string requester = "-";
                std::string name = "Unknown";

                GlobalVars::getSharedInstance()->isButtonPressed = false;

                name = levelJson["name"].as_string();
                creator =  levelJson["creator"].as_string();
                GlobalVars::getSharedInstance()->creator = creator;

                requester = levelJson["requester"].as_string();
                GlobalVars::getSharedInstance()->requester = requester;

                int ID = levelJson["id"].as_int();
                int songID = levelJson["songID"].as_int();

                int accountID = levelJson["accountID"].as_int();
                GlobalVars::getSharedInstance()->accountID = accountID;

                bool isCustomSong = levelJson["isCustomSong"].as_bool();
                int likes = levelJson["likes"].as_int();
                int downloads = levelJson["downloads"].as_int();
                int length = levelJson["lengthValue"].as_int();
                bool isDemon = levelJson["isDemon"].as_bool();
                bool isAuto = levelJson["isAuto"].as_bool();
                bool isEpic = levelJson["isEpic"].as_bool();
                int featuredScore = levelJson["featuredScore"].as_int();
                int difficulty = levelJson["difficulty"].as_int();
                int demonDifficulty = levelJson["demonDifficulty"].as_int();
                int stars = levelJson["stars"].as_int();

                CCObject* level = GameLevelManager::sharedState()
                    ->m_onlineLevels
                    ->objectForKey(std::to_string(ID));


                if (level != nullptr) GlobalVars::getSharedInstance()->levelData = reinterpret_cast<GJGameLevel*>(level);
                else {


                    GlobalVars::getSharedInstance()->levelData = GJGameLevel::create();

                    GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;

                    levelData->m_accountID = accountID;
                    levelData->m_levelID = ID;
                }

                GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;

                ((LoquiGJGameLevel*)levelData)->m_fields->m_requester = requester;
                ((LoquiGJGameLevel*)levelData)->m_fields->m_isRequest = true;

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

                requesterLabel->setString(("Requested by " + requester).c_str());

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

GJGameLevel* parseJsonToGameLevel(matjson::Value levelJson){
    std::string creator = "-";
    std::string requester = "-";
    std::string name = "Unknown";

    name = levelJson["name"].as_string();
    creator = levelJson["creator"].as_string();
    requester = levelJson["requester"].as_string();
    int ID = levelJson["id"].as_int();
    int songID = levelJson["songID"].as_int();
    int accountID = levelJson["accountID"].as_int();
    bool isCustomSong = levelJson["isCustomSong"].as_bool();
    int likes = levelJson["likes"].as_int();
    int downloads = levelJson["downloads"].as_int();
    int length = levelJson["lengthValue"].as_int();
    bool isDemon = levelJson["isDemon"].as_bool();
    bool isAuto = levelJson["isAuto"].as_bool();
    bool isEpic = levelJson["isEpic"].as_bool();
    int featuredScore = levelJson["featuredScore"].as_int();
    int difficulty = levelJson["difficulty"].as_int();
    int demonDifficulty = levelJson["demonDifficulty"].as_int();
    int stars = levelJson["stars"].as_int();

    GJGameLevel* levelData = GJGameLevel::create();

    ((LoquiGJGameLevel*)levelData)->m_fields->m_requester = requester;
    ((LoquiGJGameLevel*)levelData)->m_fields->m_isRequest = true;
    int pos = GameLevelManager::sharedState()->m_onlineLevels->count();
    levelData->m_levelIndex = pos;
    levelData->m_accountID = accountID;
    levelData->m_levelID = ID;

    levelData->m_levelType = (GJLevelType) 4;
    
    levelData->m_levelName = name;
    levelData->m_creatorName = creator;

    if(difficulty > 0){
        levelData->m_difficulty = (GJDifficulty) (difficulty-1);
    }

    int demonDifficultyValue = 0;

    switch(demonDifficulty){
        case 0:
            demonDifficultyValue = 3;
            break;
        case 1:
            demonDifficultyValue = 4;
            break;
        case 2:
            demonDifficultyValue = 2;
            break;
        case 3:
            demonDifficultyValue = 5;
            break;
        case 4:
            demonDifficultyValue = 6;
            break;
    }

    levelData->m_demonDifficulty = demonDifficultyValue;
    levelData->m_autoLevel = isAuto;
    levelData->m_demon = isDemon;
    levelData->m_ratingsSum = 10*(int)levelData->m_difficulty;
    levelData->m_ratings = 10;
    levelData->m_likes = likes;
    levelData->m_downloads = downloads;
    levelData->m_levelLength = length;
    levelData->m_stars = stars;
    levelData->m_featured = featuredScore;
    levelData->m_isEpic = isEpic;

    if (isCustomSong) levelData->m_songID = songID;
    levelData->m_audioTrack = songID;

    return levelData;
}


