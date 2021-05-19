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

    QSqlQuery clearColumns;
    if (!clearColumns.exec(QString("UPDATE %1 SET manufacturer=NULL, type=NULL, origin=NULL, "
                                    "destination=NULL, route_length=NULL;").arg(table)))
    {
        qInfo() << "Error cleaning columns" << clearColumns.lastError().text();
        return -1;
    }

    // reading database
    while (getQuery.next())
    {
        const QString date = getQuery.value("date").toString();
        const QString time = getQuery.value("time").toString();

        const QString icao24 = getQuery.value("icao24").toString();
        const QString callsign = getQuery.value("callsign").toString();

        t_airplane airplane;
        bool airplaneOk = get_airplane(icao24, airplane);

        t_route route;
        bool routeOk = get_route(callsign, route);

        QString manufacturer = airplaneOk ? "'" + airplane.manufacturer + "'" : "NULL";
        QString type = airplaneOk ? "'" + airplane.type + "'" : "NULL";
        QString origin = routeOk ? "'" + route.airports.first() + "'" : "NULL";
        QString destination = routeOk ? "'" + route.airports.last() + "'" : "NULL";
        QString length = routeOk ? "'" + QString::number(route.length) + "'" : "NULL";

        QSqlQuery insertQuery;
        QString insertStr = QString("UPDATE %1 SET manufacturer=%2, type=%3, origin=%4, "
                                    "destination=%5, route_length=%6 WHERE "
                                    "date='%7' AND time='%8';").arg(table).
                                    arg(manufacturer).arg(type).arg(origin).arg(destination).
                                    arg(length).arg(date).arg(time);
        insertQuery.prepare(insertStr);
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
