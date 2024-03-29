#pragma once

#ifndef __BLOCKLEVELALERT_HPP
#define __BLOCKLEVELALERT_HPP

#include "ServerListener.h"
#include "GlobalVars.h"

class BlockLevelAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("block " + std::to_string(GlobalVars::getSharedInstance()->currentID));
		}
	};

};

#endif