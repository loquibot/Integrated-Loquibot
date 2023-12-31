#pragma once

#ifndef __BLOCKCREATORALERT_HPP
#define __BLOCKCREATORALERT_HPP

#include "includes.h"
#include "ServerListener.h"

class BlockCreatorAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			ServerListener::sendMessage("blockCreator " + _creator);
		}
	};

};

#endif