#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJGameLevel.hpp>

class $modify(LoquiGJGameLevel, GJGameLevel){

    struct Fields {
        std::string m_requester = "";
        std::string m_message = "";
        bool m_isRequest = false;
    };
};
