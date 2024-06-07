#pragma once

#ifndef __LOQUIGJGAMELEVEL_H
#define __LOQUIGJGAMELEVEL_H

#include <Geode/Geode.hpp>
#include <Geode/modify/GJGameLevel.hpp>

class $modify(LoquiGJGameLevel, GJGameLevel){

    struct Fields {
        std::string m_requester = "";
        bool m_isRequest = false;
    };
};

#endif