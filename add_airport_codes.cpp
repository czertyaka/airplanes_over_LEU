#include <main.h>

/**
 * @brief add_airport_codes Adding airports codes from https://datahub.io/core/airport-codes#data
 * @details File is supposed to be in working directory and be named as "airport-codes_csv.csv"
 * @warning NOT FOR USE
 * @param db
 */
void add_airport_codes()
{
    QSqlQuery createTable;
    createTable.prepare("CREATE TABLE IF NOT EXISTS airports (code TEXT PRIMARY KEY, "
                          "name TEXT, lat REAL, lon REAL);");
    if (!createTable.exec())
    {
        qDebug() << "Error creating table for airports:" << createTable.lastError().text();
    }
    qDebug() << "Table \"airports\" created or already exists";

    QString filename("airport-codes_csv.csv");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << filename << "isn't open";
    }
    qDebug() << filename << "opened";

    // skip header
    file.readLine();

    while(!file.atEnd())
    {
        QByteArray byteLine = file.readLine();

        QStringList line;
        for (QString s : byteLine.split(','))
        {
            line.append(s);
        }

        QString code(line[0]);
        QString name(line[2]);

        double lon = line[11].remove('\"').toDouble();

        double lat = line[12].remove(' ').remove('\"').remove('\r')
                        .remove('\n').toDouble();

        QSqlQuery insert;
        insert.prepare("INSERT INTO airports (code, name, lat, lon) "
                       "VALUES(:code, :name, :lat, :lon);");
        insert.bindValue(":code", code);
        insert.bindValue(":name", name);
        insert.bindValue(":lat", lat);
        insert.bindValue(":lon", lon);
        if (!insert.exec())
        {
            qDebug() << "Error inserting values to tabel \"airports\": code:" << code
                     << insert.lastError().text();
        }

        qDebug() << "Inserting airport with code" << code;
    }

    qDebug() << "Table airports initialized successfully";
}
