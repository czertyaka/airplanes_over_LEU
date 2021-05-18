#include <main.h>

/**
 * @brief airplanesMap Conatiner for airplanes with icao24 as key
 */
static t_airplanesMap airplanesMap;

/**
 * @brief get_airplane Get airplane characteristics via http request or from @ref airplanesMap
 * @param icao24
 * @param airplane
 * @return
 */
bool get_airplane(const QString& icao24, t_airplane& airplane)
{
    if (icao24.isEmpty())
    {
        return false;
    }

    auto airplaneIt = airplanesMap.find(icao24);
    if (airplaneIt != airplanesMap.end())
    {
        qInfo() << "icao24" << icao24 << "were found in QMap";
        airplane = *airplaneIt;
        return true;
    }

    Downloader& downloader = Downloader::GetInstance();
    const QByteArray answer = downloader.Download(QUrl(QString("https://api.joshdouch.me/api/aircraft/%1").
                                                    arg(icao24)));

    QJsonDocument jsonDoc = QJsonDocument::fromJson(answer);

    if (jsonDoc["status"].toString() == "404")
    {
        return false;
    }

    airplane.manufacturer = jsonDoc["Manufacturer"].toString();
    airplane.type = jsonDoc["Type"].toString();

    airplanesMap.insert(icao24, airplane);
    return true;
}
