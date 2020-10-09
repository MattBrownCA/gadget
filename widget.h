#ifndef WIDGET_H
#define WIDGET_H

#include <unordered_map>
#include <memory>
#include <string>
#include <QWidget>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(const std::string &name, Widget *parent = nullptr);

    std::string name() const;

    static Widget *get_widget_by_name(const std::string &name);

private:
    using WidgetPtr = Widget*;
    using WidgetMap = std::unordered_map<std::string, WidgetPtr>;

    std::string _name;

    static WidgetMap s_map;
};

#endif // WIDGET_H
