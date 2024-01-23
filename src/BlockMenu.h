#pragma once

#ifndef __BLOCKMENU_HPP
#define __BLOCKMENU_HPP

#include "BrownAlertDelegate.h"

class BlockMenu : public BrownAlertDelegate {
    protected:
        virtual void setup();
    public:
        static BlockMenu* create();
};

#endif