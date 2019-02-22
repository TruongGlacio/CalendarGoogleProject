#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define DTSTART "DTSTART"
#define VALUE "VALUE"
#define DTEND "DTEND"
#define SUMMARY "SUMMARY"
#define DESCRIPTION "DESCRIPTION"
#define SUMMARY "SUMMARY"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include<QList>
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

signals:

public slots:
    void replyFinished(QNetworkReply *reply);
    void slotReadyRead();
    void slotError();
    void slotSslErrors();
    void ReadFile(QString data);
    void WriteDataToFile(QString data);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    struct DataStruct{
        QString _DTSTART;
        QString _DTEND;
        QString _SUMMARY;
        QString _DESCRIPTION;
    };
    QList<QString> mSUMMARY;
};

#endif // NETWORKMANAGER_H
