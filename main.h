#ifndef MAIN_H
#define MAIN_H

#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QSqlQuery>
#include <QMap>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QObject>
#include <QJsonDocument>
#include <QVector>

#include <downloader.h>

typedef struct _airplane
{
    QString manufacturer;
    QString type;
} t_airplane;

typedef QMap<QString, t_airplane> t_airplanesMap;

typedef struct _route
{
    QVector<QString> airports;
    double length;
} t_route;

typedef QMap<QString, t_route> t_routesMap;

bool process_table(const QString& table);
void add_airport_codes();
bool get_airplane(const QString& icao24, t_airplane& airplane);
bool get_route(const QString& callsign, t_route& route);
QNetworkAccessManager& network_manager();

#endif // MAIN_H
