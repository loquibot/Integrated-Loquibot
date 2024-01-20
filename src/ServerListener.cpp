
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
        _loquiOpen = true;

        while (ws->getReadyState() != WebSocket::CLOSED) {
            WebSocket::pointer wsp = &*ws; // a unique_ptr cannot be copied into a lambda
            ws->poll();
            ws->dispatch([wsp](const std::string& message) {
                onMessage(message);
                });
        }
    }

    //closed

    _loquiOpen = false;
    ws.release();

#ifdef _WIN32
    WSACleanup();
#endif

}

std::string _creator;
std::string _requester;
int _accountID;
int _rand;
bool _isViewer = true;
CCLayer* lastLayer;

bool _isEmpty = false;
bool _isStartLevel = true;
bool _isButtonPressed = false;
bool _loquiOpen = false;
bool _isLoquiMenu = false;
bool _deleting = false;
bool _isSearchScene = false;
int _currentID;

GJGameLevel* _levelData = nullptr;


void ServerListener::onMessage(std::string message) {
    geode::Loader::get()->queueInMainThread([message]() {


        geode::log::info("{}", message);

        if (_isButtonPressed) {
            rapidjson::Document levelJson;
            levelJson.Parse<0>(message.c_str());

            std::string service = "none";

            if (levelJson.HasMember("service")) {
                rapidjson::Value& valueS = levelJson["service"];
                service = valueS.GetString();
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
                    (new Loquibot())->showButtons(lastLayer);
                    return;
                }

            }

            if (service == "gd") {

                std::string creator = "-";
                std::string requester = "-";
                std::string name = "Unknown";

                _isButtonPressed = false;

                rapidjson::Value& nameValue = levelJson["name"];
                name = nameValue.GetString();

                rapidjson::Value& creatorValue = levelJson["creator"];
                creator = creatorValue.GetString();
                _creator = creator;

                rapidjson::Value& requesterValue = levelJson["requester"];
                requester = requesterValue.GetString();
                _requester = requester;

                rapidjson::Value& idValue = levelJson["id"];
                int ID = idValue.GetInt();

                rapidjson::Value& songIDValue = levelJson["songID"];
                int songID = songIDValue.GetInt();

                rapidjson::Value& accountIDValue = levelJson["accountID"];
                int accountID = accountIDValue.GetInt();
                _accountID = accountID;

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

                if(ID == _currentID){
                    auto alertLayer = FLAlertLayer::create(nullptr, "Loquibot", "There are no more levels in the queue!", "Okay", nullptr, 300);
                    alertLayer->show();
                    (new Loquibot())->showButtons(lastLayer);
                    return;
                }


                if (level != nullptr) _levelData = reinterpret_cast<GJGameLevel*>(level);
                else {
                    _levelData = GJGameLevel::create();
                    _levelData->m_accountID = accountID;

                    _levelData->m_levelID = ID;
                }

                _levelData->m_levelName = name;
                _levelData->m_creatorName = creator;

                _levelData->m_likes = likes;
                _levelData->m_downloads = downloads;
                _levelData->m_levelLength = length;
                _levelData->m_stars = stars;
                _levelData->m_featured = featuredScore;
                _levelData->m_isEpic = isEpic;
                _levelData->m_autoLevel = isAuto;

                if (isCustomSong) _levelData->m_songID = songID;
                _levelData->m_audioTrack = songID;

                _levelData->m_levelType = GJLevelType::Saved;
                _currentID = ID;

                LevelInfoLayer* layer = LevelInfoLayer::create(_levelData, false);

                if (_levelData->m_levelString.size() == 0) {
                    _levelData->m_levelNotDownloaded = true;
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
   
    if (_loquiOpen) {
        ws->send(message);
    }
}

void ServerListener::open() {


    ws.release();
    ws.reset(WebSocket::from_url("ws://127.0.0.1:19236"));

}