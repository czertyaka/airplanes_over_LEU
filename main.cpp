#include <main.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // check args
    if (argc < 3)
    {
        qInfo() << "Database and table are names required";
        return -1;
    }

    // check if db exists
    const QString dbName(argv[1]);
    QFileInfo dbInfo(dbName);
    if (!(dbInfo.exists() && dbInfo.isFile()))
    {
        qInfo() << "Database file" << QDir::currentPath() + "/" + dbName << "does not exist or isn't a file";
        return -1;
    }

    // open database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qInfo() << "Error opening database:" << db.lastError().text();
        return -1;
    }
    qInfo() << "Database" << QDir::currentPath() + "/" + dbName << "opened";

    // find table
    const QString table(argv[2]);
    const QStringList tables = db.tables();
    if (tables.indexOf(table) == -1)
    {
        qInfo() << "Error opening table" << table;
        qInfo() << "List of available tables:";
        for (auto& i : tables)
        {
            qInfo() << i;
        }
        return  -1;
    }
    qInfo() << "Found existing table" << table << "in database";

    process_table(table);

    a.exec();
}
