
#include <Geode/Geode.hpp>
#include "GlobalVars.h"

GlobalVars* GlobalVars::instance = nullptr;

std::string creator;
std::string requester;
cocos2d::CCLayer* lastLayer;

int accountID;
int currentID;

bool isViewer = true;
bool isEmpty = false;
bool isStartLevel = true;
bool isButtonPressed = false;
bool loquiOpen = false;
bool isLoquiMenu = false;
bool deleting = false;
bool isSearchScene = false;

GJGameLevel* levelData = nullptr;