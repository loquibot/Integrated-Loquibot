#pragma once

#ifndef __BLOCKLEVELALERT_HPP
#define __BLOCKLEVELALERT_HPP

#include "includes.h"
#include "ServerListener.h"

class BlockLevelAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("block " + std::to_string(_currentID));
		}
	};

};

#endif