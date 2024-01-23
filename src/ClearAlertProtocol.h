#pragma once

#ifndef __CLEARALERT_HPP
#define __CLEARALERT_HPP

#include "ServerListener.h"
#include <Geode/Geode.hpp>

class ClearAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("clear");
			
			CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
		}
	};

};

#endif