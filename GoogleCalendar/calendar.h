#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include "o2google.h"
class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = nullptr);
signals:
    void extraTokensReady(const QVariantMap &extraTokens);
    void linkingFailed();
    void linkingSucceeded();
    void channelInfoReceived();
    void channelInfoFailed();

public slots:
    void doOAuth(O2::GrantFlow grantFlowType);
    void getUserChannelInfo();
    Q_INVOKABLE void Connect();
    Q_INVOKABLE void Login();

private slots:
    void onLinkedChanged();
    void onLinkingSucceeded();
    void onOpenBrowser(const QUrl &url);
    void onCloseBrowser();
    void onFinished(int, QNetworkReply::NetworkError, QByteArray);

private:
    O2Google *o2Google_;
    int requestId_;
private:
};

#endif // CALENDAR_H
