#pragma once

#ifndef __BLOCKCREATORALERT_HPP
#define __BLOCKCREATORALERT_HPP

#include "ServerListener.h"
#include "GlobalVars.h"

class BlockCreatorAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("blockCreator " + GlobalVars::getSharedInstance()->creator);
		}
	};

};

#endif