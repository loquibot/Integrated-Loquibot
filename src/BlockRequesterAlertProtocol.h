#pragma once

#ifndef __BLOCKREQUESTERALERT_HPP
#define __BLOCKREQUESTERALERT_HPP

#include "ServerListener.h"
#include "GlobalVars.h"

class BlockRequesterAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("blockRequester " + GlobalVars::getSharedInstance()->requester);
		}
	};

};

#endif