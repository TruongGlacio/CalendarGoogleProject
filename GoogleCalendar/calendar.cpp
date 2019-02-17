#include "calendar.h"

Calendar::Calendar(QObject *parent) : QObject(parent)
{
    calenarService=new CalendarService();
    Login();
}

void Calendar::Connect()
{

}

void Calendar::Login()
{
    if(!calenarService->login(USER_LOGIN, PASSWORD))
    {
        qDebug()<<"Login error"<<endl;
    }

}
