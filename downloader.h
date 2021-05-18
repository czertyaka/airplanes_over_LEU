#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <main.h>

class Downloader : public QObject
{
    Q_OBJECT
public:
    static Downloader& GetInstance();
    QByteArray Download(const QUrl& url);

private:
    explicit Downloader(QObject *parent = nullptr);
    QNetworkAccessManager* manager;
    QByteArray answer;
    QEventLoop* loop;
};

#endif // DOWNLOADER_H
