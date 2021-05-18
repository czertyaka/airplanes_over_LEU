#include "downloader.h"
#include <main.h>

Downloader::Downloader(QObject *parent) :
    QObject(parent), manager(new QNetworkAccessManager), loop(new QEventLoop)
{
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     loop, SLOT(quit()));
}

/**
 * @brief Downloader::Download
 * @param url
 * @return
 */
Downloader &Downloader::GetInstance()
{
    static Downloader instance;
    return instance;
}

QByteArray Downloader::Download(const QUrl &url)
{
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    loop->exec();

    if (reply->error())
    {
        qInfo() << reply->errorString();
        return "";
    }

    answer = reply->readAll();
    qInfo() << answer;

    return answer;
}
