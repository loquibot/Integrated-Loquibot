
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
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

        geode::Result<matjson::Value, matjson::ParseError> levelJsonRes = matjson::parse(message.c_str());

        if (levelJsonRes.isOk()) {
            matjson::Value levelJson = levelJsonRes.unwrap();

            if (levelJson.contains("type")) {

                std::string type = levelJson["type"].asString().unwrapOr("");

                if(type == "level_list"){
                    
                    CCArray* arr = CCArray::create();

                    geode::Result<std::vector<matjson::Value>&> levelArrayRes = levelJson["levels"].asArray();
                    if (levelArrayRes.isOk()) {
                        std::vector<matjson::Value> levelArray = levelArrayRes.unwrap();
                        bool levelsEnabled = true;
                        if(levelJson.contains("levels_enabled")){
                            levelsEnabled = levelJson["levels_enabled"].asBool().unwrapOr(false);
                        }

                        for (int i = 0; i < levelArray.size(); i++){

                            GJGameLevel* level = parseJsonToGameLevel(levelArray.at(i));

                            level->m_listPosition = i+1;
                            arr->addObject(level);
                        }

                        GlobalVars::getSharedInstance()->currentLevelList = levelArray;

                        CCScene* scene = RequestsLayer::scene(arr, levelsEnabled);

                        auto transition = CCTransitionFade::create(0.5f, scene);
                    
                        CCDirector::sharedDirector()->pushScene(transition);
                    }
                }
                if(type == "update_toggle"){
                    bool levelsEnabled = levelJson["levels_enabled"].asBool().unwrapOr(false);

                    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

                    RequestsLayer* reqLayer = dynamic_cast<RequestsLayer*>(currentScene->getChildren()->objectAtIndex(0));

                    if(reqLayer){
                        reqLayer->updateToggle(levelsEnabled);
                    }

                }
                if(type == "youtube_info"){
                    std::string videoTitle = levelJson["title"].asString().unwrapOr("");
                    std::string videoCreator = levelJson["creator"].asString().unwrapOr("");
                    std::string videoViews = levelJson["views"].asString().unwrapOr("");
                    std::string videoID = levelJson["id"].asString().unwrapOr("");

                    std::string thumbnailURL = fmt::format("https://img.youtube.com/vi/{}/maxresdefault.jpg", videoID);
                    std::string videoURL = fmt::format("https://www.youtube.com/watch?v={}", videoID);

                    YouTubeMenu* menu = YouTubeMenu::create(thumbnailURL, videoTitle, videoCreator, videoViews, videoURL);
                    menu->show();
                }
                if(type == "has_youtube"){
                    int id = levelJson["id"].asInt().unwrapOr(0);

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
                    service = levelJson["service"].asString().unwrapOr("");
                }

                if (service == "gd") {
                    bool badUpdate = false;

                    if (levelJson.contains("version")) {
                        int version = levelJson["version"].asInt().unwrapOr(0);

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
                        Loquibot::getSharedInstance()->m_isClickable = true;
                        return;
                    }

                    if (levelJson.contains("status")) {

                        std::string status = levelJson["status"].asString().unwrapOr("");

                        if(status == "empty"){
                            auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "The queue is empty!", "Okay", nullptr, 300);
                            alertLayer->show();
                            Loquibot::getSharedInstance()->m_isClickable = true;
                            return;
                        }

                    }

                    if (levelJson.contains("next_status")) {

                        std::string status = levelJson["next_status"].asString().unwrapOr("");

                        if(status == "empty"){
                            auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "There are no more levels in the queue!", "Okay", nullptr, 300);
                            alertLayer->show();
                            Loquibot::getSharedInstance()->m_isClickable = true;
                            Loquibot::getSharedInstance()->showButtons();
                            return;
                        }

                    }

                    std::string creator = "-";
                    std::string requester = "-";
                    std::string name = "Unknown";
                    std::string chatMessage = "";

                    GlobalVars::getSharedInstance()->isButtonPressed = false;

                    name = levelJson["name"].asString().unwrapOr("");
                    creator =  levelJson["creator"].asString().unwrapOr("");
                    requester = levelJson["requester"].asString().unwrapOr("");
                    chatMessage = levelJson["message"].asString().unwrapOr("");

                    GlobalVars::getSharedInstance()->requester = requester;
                    GlobalVars::getSharedInstance()->creator = creator;

                    int ID = levelJson["id"].asInt().unwrapOr(0);
                    int songID = levelJson["songID"].asInt().unwrapOr(0);

                    int accountID = levelJson["accountID"].asInt().unwrapOr(0);

                    if (levelJson.contains("userID")) {
                        int userID = levelJson["userID"].asInt().unwrapOr(0);
                        GameLevelManager::sharedState()->storeUserName(userID, accountID, creator);
                    }

                    bool isCustomSong = levelJson["isCustomSong"].asBool().unwrapOr(false);
                    int likes = levelJson["likes"].asInt().unwrapOr(0);
                    int downloads = levelJson["downloads"].asInt().unwrapOr(0);
                    int length = levelJson["lengthValue"].asInt().unwrapOr(0);
                    bool isDemon = levelJson["isDemon"].asBool().unwrapOr(false);
                    bool isAuto = levelJson["isAuto"].asBool().unwrapOr(false);
                    bool isEpic = levelJson["isEpic"].asBool().unwrapOr(false);
                    int featuredScore = levelJson["featuredScore"].asInt().unwrapOr(0);
                    int difficulty = levelJson["difficulty"].asInt().unwrapOr(0);
                    int demonDifficulty = levelJson["demonDifficulty"].asInt().unwrapOr(0);
                    int stars = levelJson["stars"].asInt().unwrapOr(0);

                    CCObject* level = GameLevelManager::sharedState()
                        ->m_onlineLevels
                        ->objectForKey(std::to_string(ID));


                    if (level != nullptr) GlobalVars::getSharedInstance()->levelData = reinterpret_cast<GJGameLevel*>(level);
                    else {
                        GlobalVars::getSharedInstance()->levelData = GJGameLevel::create();
                        GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;
                        levelData->m_levelID = ID;
                    }

                    GJGameLevel* levelData = GlobalVars::getSharedInstance()->levelData;

                    ((LoquiGJGameLevel*)levelData)->m_fields->m_requester = requester;
                    ((LoquiGJGameLevel*)levelData)->m_fields->m_isRequest = true;
                    ((LoquiGJGameLevel*)levelData)->m_fields->m_message = chatMessage;

                    levelData->m_levelName = name;
                    levelData->m_creatorName = creator;
                    levelData->m_accountID = accountID;

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
                    auto requesterLabel = reinterpret_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("requester-label"_spr));
                    auto requesterButton = reinterpret_cast<CCMenuItemSpriteExtra*>(layer->getChildByIDRecursive("requester-button"_spr));

                    requesterLabel->setString(("Requested by " + requester).c_str());

                    auto winSize = CCDirector::sharedDirector()->getWinSize();

                    float heightOffset = 60.f;

                    if (Loader::get()->isModLoaded("n.level-pronouns")) {
                        heightOffset += 5.f;
                    }

                    requesterButton->setPosition({ winSize.width / 2,
                        winSize.height - heightOffset });

                    auto scene = CCScene::create();
                    scene->addChild(layer);

                    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
                }
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

    name = levelJson["name"].asString().unwrapOr("");
    creator = levelJson["creator"].asString().unwrapOr("");
    requester = levelJson["requester"].asString().unwrapOr("");
    int ID = levelJson["id"].asInt().unwrapOr(0);
    int songID = levelJson["songID"].asInt().unwrapOr(0);
    int accountID = levelJson["accountID"].asInt().unwrapOr(0);
    bool isCustomSong = levelJson["isCustomSong"].asBool().unwrapOr(false);
    int likes = levelJson["likes"].asInt().unwrapOr(0);
    int downloads = levelJson["downloads"].asInt().unwrapOr(0);
    int length = levelJson["lengthValue"].asInt().unwrapOr(0);
    bool isDemon = levelJson["isDemon"].asBool().unwrapOr(false);
    bool isAuto = levelJson["isAuto"].asBool().unwrapOr(false);
    bool isEpic = levelJson["isEpic"].asBool().unwrapOr(false);
    int featuredScore = levelJson["featuredScore"].asInt().unwrapOr(0);
    int difficulty = levelJson["difficulty"].asInt().unwrapOr(0);
    int demonDifficulty = levelJson["demonDifficulty"].asInt().unwrapOr(0);
    int stars = levelJson["stars"].asInt().unwrapOr(0);


    if (levelJson.contains("userID")) {
        int userID = levelJson["userID"].asInt().unwrapOr(0);
        GameLevelManager::sharedState()->storeUserName(userID, accountID, creator);
    }

    GJGameLevel* levelData = GJGameLevel::create();

    ((LoquiGJGameLevel*)levelData)->m_fields->m_requester = requester;
    ((LoquiGJGameLevel*)levelData)->m_fields->m_isRequest = true;
    int pos = GameLevelManager::sharedState()->m_onlineLevels->count();
    levelData->m_levelIndex = pos;
    levelData->m_accountID = accountID;
    levelData->m_levelID = ID;

    //todo make loquibot app send user ID

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


