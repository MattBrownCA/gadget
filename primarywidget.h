#ifndef PRIMARYWIDGET_H
#define PRIMARYWIDGET_H

#include <QWidget>

class PrimaryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrimaryWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

private:
    qintptr timer_id;

    void drawTime(QPainter *painter);
};

#endif // PRIMARYWIDGET_H
