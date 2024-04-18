#include <Geode/Geode.hpp>
#include <Geode/modify/TextArea.hpp>

using namespace geode::prelude;

class $modify(MyTextArea, TextArea){

    gd::string m_text;

    bool init(gd::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor){
        m_fields->m_text = str;

        return TextArea::init(str, font, scale, width, anchor, lineHeight, disableColor);
    }

    void setString(gd::string p0){
        m_fields->m_text = p0;

        TextArea::setString(p0);
    }

    gd::string getString(){
        return m_fields->m_text;
    }

};