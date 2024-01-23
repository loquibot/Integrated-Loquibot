#pragma once

#ifndef __HELPALERT_HPP
#define __HELPALERT_HPP

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

class HelpAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			
			web::openLinkInBrowser("https://loquibot.com/versionHelp.html");

		}
	};

};

#endif