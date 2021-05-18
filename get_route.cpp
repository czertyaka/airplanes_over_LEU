#include <main.h>
#include <cmath>

/**
 * @brief routesMap Conatiner for routes with callsign as key
 */
QMap<QString, t_route> routesMap;

bool calculate_route_length(t_route& route);

static const double earth_rad = 6371000; ///< earth radius in meters

static inline double haversine_formula(const double lat1, const double lat2,
                                       const double lon1, const double lon2,
                                       const double rad = earth_rad);

/**
 * @brief get_route
 * @param callsign
 * @param route
 * @return
 */
bool get_route(const QString& callsign, t_route& route)
{
    if (callsign.isEmpty())
    {
        return false;
    }

    auto routeIt = routesMap.find(callsign);
    if (routeIt != routesMap.end())
    {
        route = *routeIt;
        return true;
    }

    Downloader& downloader = Downloader::GetInstance();
    QByteArray answer = downloader.Download(QUrl(QString("https://api.joshdouch.me/callsign-route.php?callsign=%1").
                                                 arg(callsign)));
    if (answer == "n/a")
    {
        return false;
    }

    QList<QByteArray> list = answer.split('-');
    for (auto& i : list)
    {
        route.airports.push_back(i);
    }

    return calculate_route_length(route);
}

/**
 * @brief calculate_route_length
 * @param route
 * @return
 */
bool calculate_route_length(t_route& route)
{
    route.length = 0;

    double prevLat = 0;
    double prevLon = 0;

    for (auto& i : route.airports)
    {
        QSqlQuery getQuery;
        if (!getQuery.exec(QString("SELECT * FROM airports WHERE code='%1';").arg(i)))
        {
            qInfo() << "Airport" << i << "not found in database";
            return false;
        }
        qInfo() << "Airport" << i << "found in database";

        getQuery.first();
        double lat = getQuery.value("lat").toDouble();
        double lon = getQuery.value("lon").toDouble();

        if (prevLat && prevLon)
        {
            route.length += haversine_formula(prevLat, lat, prevLon, lon);
        }

        prevLat = lat;
        prevLon = lon;
    }

    return true;
}

/**
 * @brief haversine_formula Implements haversine formula for distance between
 *        two points on shpere
 * @details https://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1 Latitude of point 1, radians
 * @param lat2 Latitude of point 2, radians
 * @param lon1 Longitude of point 1, radians
 * @param lon2 Longitude of point 2, radians
 * @param rad Radius of sphere, meters
 * @return Distance beyween point 1 and point 2
 */
static inline double haversine_formula(const double lat1, const double lat2,
                                       const double lon1, const double lon2,
                                       const double rad)
{
    using namespace std;

    double h = pow( sin((lat2 - lat1) / 2), 2 ) + cos(lat1) * cos(lat2)
                * pow( sin((lon2 - lon1) / 2), 2 );
    assert(h >= 0 && h < 1);
    return 2 * rad * asin(sqrt(h));
}
