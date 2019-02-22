#include "calendar.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QMetaEnum>
#include <QDebug>
#include "o0globals.h"
#include "o0settingsstore.h"
#include "o2requestor.h"

#define QENUM_NAME(o,e,v) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).valueToKey((v)))
#define GRANTFLOW_STR(v) QString(QENUM_NAME(O2, GrantFlow, v))
const char GOOGLE_API_KEY[]    = "226641755137-e296lcbd3co97lfobung6o9l3mgs72gl.apps.googleusercontent.com";
const char GOOGLE_APP_SECRET[] = "y9AOGTcf1VauLsrG8OrHRcUZ";
const char CALENDAR_SCOPE[] = "https://www.googleapis.com/auth/calendar.events";
const char CALENDAR_CHANNELS_LIST_URL[] = "https://calendar.google.com/calendar/ical/buixuantruong.tb%40gmail.com/private-d43d8fb17b020ee5766dd6e72ef81a80/basic.ics";
const char CALENDAR_USER_EMAIL[]= "buixuantruong.tb@gmail.com";
const char CALENDAR_USER_PASSWORD[]="@@@!Duongdai13061994";
const char CALENDAR_TOKEN_URI[]="https://oauth2.googleapis.com/token";
const char CALENDAR_AUTH_URI[]="https://accounts.google.com/o/oauth2/auth";
const char CALENDAR_CLIENT_ID[]="710766835746-evkjvu0i7nsjk0j3qbdqnec79s1p5od7.apps.googleusercontent.com";
const int localPort = 8888;


Calendar::Calendar(QObject *parent) : QObject(parent)
{
    o2Google_ = new O2Google(this);

    o2Google_->setClientId(CALENDAR_CLIENT_ID);
    o2Google_->setClientSecret(GOOGLE_APP_SECRET);
    o2Google_->setLocalPort(localPort);
    o2Google_->setScope(CALENDAR_SCOPE);
    o2Google_->setUsername(CALENDAR_USER_EMAIL);
    o2Google_->setPassword(CALENDAR_USER_PASSWORD);
    o2Google_->setApiKey(GOOGLE_API_KEY);
    o2Google_->setTokenUrl(CALENDAR_TOKEN_URI);
   // o2Google_-

    // Create a store object for writing the received tokens
    O0SettingsStore *store = new O0SettingsStore(O2_ENCRYPTION_KEY);
    store->setGroupKey("google");
    o2Google_->setStore(store);

    connect(o2Google_, SIGNAL(linkedChanged()), this, SLOT(onLinkedChanged()));
    connect(o2Google_, SIGNAL(linkingFailed()), this, SIGNAL(linkingFailed()));
    connect(o2Google_, SIGNAL(linkingSucceeded()), this, SLOT(onLinkingSucceeded()));
    connect(o2Google_, SIGNAL(openBrowser(QUrl)), this, SLOT(onOpenBrowser(QUrl)));
    connect(o2Google_, SIGNAL(closeBrowser()), this, SLOT(onCloseBrowser()));
}

void Calendar::Connect()
{

}

void Calendar::Login()
{
    doOAuth(O2::GrantFlowAuthorizationCode);
}
void Calendar::doOAuth(O2::GrantFlow grantFlowType) {
    qDebug() << "Starting OAuth 2 with grant flow type" << GRANTFLOW_STR(grantFlowType) << "...";
    o2Google_->setGrantFlow(grantFlowType);
    o2Google_->unlink();
    o2Google_->link();
}

void Calendar::getUserChannelInfo() {
    if (!o2Google_->linked()) {
        qWarning() << "ERROR: Application is not linked!";
        qDebug() << "Linking failed!";
        emit linkingFailed();
        return;
    }

    QString channelsListUrl = QString(CALENDAR_CHANNELS_LIST_URL);
    QNetworkRequest request;
    request.setUrl(QUrl(channelsListUrl));
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    O2Requestor *requestor = new O2Requestor(mgr, o2Google_, this);
    requestId_ = requestor->get(request);
    QNetworkReply *reply=mgr->get(request);
    qDebug()<<"data request="<<reply->readAll();
    connect(requestor, SIGNAL(finished(int, QNetworkReply::NetworkError, QByteArray)),
        this, SLOT(onFinished(int, QNetworkReply::NetworkError, QByteArray))
    );
    qDebug() << "Getting user channel info... Please wait.";
}

void Calendar::onOpenBrowser(const QUrl &url) {
    QDesktopServices::openUrl(url);
}

void Calendar::onCloseBrowser() {
}

void Calendar::onLinkedChanged() {
    qDebug() << "Link changed!";
}

void Calendar::onLinkingSucceeded() {
    O2Google *o2t = qobject_cast<O2Google *>(sender());
    if (!o2t->linked()) {
        return;
    }
    QVariantMap extraTokens = o2t->extraTokens();
    if (!extraTokens.isEmpty()) {
        emit extraTokensReady(extraTokens);
        qDebug() << "Extra tokens in response:";
        foreach (QString key, extraTokens.keys()) {
            qDebug() << "\t" << key << ":" << (extraTokens.value(key).toString().left(3) + "...");
        }
    }
    qDebug() << "Linking succeeded!";
    getUserChannelInfo();  
    emit linkingSucceeded();
}

void Calendar::onFinished(int requestId, QNetworkReply::NetworkError error, QByteArray replyData) {
    if (requestId != requestId_)
        return;

    if (error != QNetworkReply::NoError) {
        qWarning() << "Reply error:" << error;
        qDebug() << "Error getting channel info!";
        emit channelInfoFailed();
        return;
    }

    QString reply(replyData);
    bool errorFound = reply.contains("error");
    if (errorFound) {
        qDebug() << "Request failed";
        qDebug() << "Error getting channel info!";
        emit channelInfoFailed();
        return;
    }

    qDebug()<<"replyData="<<reply;
  //  qInfo() << "Channel info: " << reply;
    qDebug() << "Channel info received!";
    emit channelInfoReceived();
}

