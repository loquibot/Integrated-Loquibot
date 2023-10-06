#pragma once

#ifndef __SERVERLISTENER_H
#define __SERVERLISTENER_H

#include "includes.h"
#include <Geode/Geode.hpp>

using namespace cocos2d;

extern GJGameLevel* _levelData;

extern std::string _creator;
extern std::string _requester;
extern int _accountID;
extern int _rand;
extern bool _isViewer;
extern CCLayer* lastLayer;

extern bool _isEmpty;
extern bool _isStartLevel;
extern bool _isButtonPressed;
extern bool _isLoquiMenu;
extern bool _deleting;
extern bool _loquiOpen;
extern bool _isSearchScene;

extern int _currentID;

class ServerListener {
protected:
public:

    static void connectAsync();
    static void connect();
    static void open();
    static void onMessage(std::string message);
    static void sendMessage(std::string message);



private:
};

#endif