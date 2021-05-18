#include <main.h>

/**
 * @brief process_table Reading table and filling columns: m
 * @param db
 * @param table
 * @return
 */
bool process_table(const QString& table)
{
    QSqlQuery getQuery;
    getQuery.prepare(QString("SELECT * FROM %1").arg(table));
    if (!getQuery.exec())
    {
        qInfo() << "Error reading from" << table << "table:" << getQuery.lastError().text();
    }

    // reading database
    while (getQuery.next())
    {
        const QString icao24 = getQuery.value("icao24").toString();
        const QString callsign = getQuery.value("callsign").toString();

        t_airplane airplane;
        bool airplaneOk = get_airplane(icao24, airplane);

        t_route route;
        bool routeOk = get_route(callsign, route);

        QString manufacturer = airplaneOk ? airplane.manufacturer : "";
        QString type = airplaneOk ? airplane.type : "";
        QString origin = routeOk ? route.airports.first() : "";
        QString destination = routeOk ? route.airports.last() : "";
        QString length = routeOk ? QString::number(route.length) : "";

        QSqlQuery insertQuery;
        insertQuery.prepare(QString("INSERT INTO %1 (manufacturer, type, origin, "
                                    "destination, route_length) VALUES"
                                    "(:manufacturer, :type, :origin, :destination,"
                                    " :route_length);").arg(table));
        insertQuery.bindValue(":manufacturer", manufacturer);
        insertQuery.bindValue(":type", type);
        insertQuery.bindValue(":origin", origin);
        insertQuery.bindValue(":destination", destination);
        insertQuery.bindValue(":route_length", length);
        if (!insertQuery.exec())
        {
            qInfo() << "Error adding" << icao24 << callsign << "to database"
                    << insertQuery.lastError().text();
            return false;
        }
        qInfo() << "Record" << icao24 << callsign << manufacturer << type << origin << destination << length
                << "added to database";
    }

    return true;
}
