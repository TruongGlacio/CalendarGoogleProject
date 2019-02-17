#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include<Google/CalendarService.h>
#define USER_LOGIN "buixuantruong.tb@gmail.com"
#define PASSWORD "@@@!Duongdai13061994"
class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = nullptr);

signals:

public slots:
    Q_INVOKABLE void Connect();
    Q_INVOKABLE void Login();
private:
   CalendarService *calenarService;
};

#endif // CALENDAR_H
