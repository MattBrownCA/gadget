#include "widget.h"
#include <stdexcept>

Widget::WidgetMap Widget::s_map;

Widget::Widget(const std::string &name, Widget *parent) :
    QWidget(parent, Qt::FramelessWindowHint), _name(name)
{
    setWindowFlag(Qt::WindowStaysOnBottomHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlag(Qt::Tool);

    if (s_map.count(name) > 0) {
        throw std::runtime_error("Widget already eixists");
    }

    s_map[name] = this;
}

Widget *Widget::get_widget_by_name(const std::string &name)
{
    if (s_map.count(name) == 0) {
        return nullptr;
    }

    return s_map.at(name);
}


