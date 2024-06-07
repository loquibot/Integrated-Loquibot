#include <Geode/Geode.hpp>
#include <Geode/modify/TextArea.hpp>

using namespace geode::prelude;

class $modify(MyTextArea, TextArea){

    struct Fields {
        gd::string m_text;
    };

    static TextArea* create(gd::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor){
        auto ret = TextArea::create(str, font, scale, width, anchor, lineHeight, disableColor);
        auto myRet = static_cast<MyTextArea*>(ret);
        myRet->m_fields->m_text = str;
        return ret;
    }

    void setString(gd::string p0){
        m_fields->m_text = p0;
        TextArea::setString(p0);
    }

    gd::string getString(){
        return m_fields->m_text;
    }

};