#include "networkmanager.h"
#include<QFile>
NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &NetworkManager::replyFinished);

   // manager->get(QNetworkRequest(QUrl("https://calendar.google.com/calendar/ical/buixuantruong.tb%40gmail.com/private-d43d8fb17b020ee5766dd6e72ef81a80/basic.ics")));
    request.setUrl(QUrl("https://calendar.google.com/calendar/ical/buixuantruong.tb%40gmail.com/private-d43d8fb17b020ee5766dd6e72ef81a80/basic.ics"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    reply = manager->get(request);

    connect(reply, &QIODevice::readyRead, this, &NetworkManager::slotReadyRead);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &NetworkManager::slotError);
    connect(reply, &QNetworkReply::sslErrors,
            this, &NetworkManager::slotSslErrors);
}

void NetworkManager::replyFinished(QNetworkReply *reply)
{
    QString replyData= QString(reply->readAll());
  //  qDebug()<<"Data="<<replyData;
    ReadFile(replyData);

}

void NetworkManager::slotReadyRead()
{
    qDebug()<<"Data readread=";
}

void NetworkManager::slotError()
{
     qDebug()<<"Data  slot error ";
}

void NetworkManager::slotSslErrors()
{
    qDebug()<<"Data ssl error=";
}

void NetworkManager::ReadFile(QString data)
{
       QTextStream in(&data);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.contains(SUMMARY)){
             { QString dataSumary=line.section(":",0,-1);
              mSUMMARY.append(dataSumary);
              qDebug()<<dataSumary;
              }
          }

       }
}

void NetworkManager::WriteDataToFile(QString data)
{
    QString filename="Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << data << endl;
        qDebug()<<"reply Data="<< data;
        file.close();
    }
}
