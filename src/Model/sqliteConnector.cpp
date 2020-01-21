/*!
* \file sqliteConnector.cpp
* \brief this class make the connection to the sqLite Database
* \author Simon Flörke
*/

#include "sqliteConnector.h"

/*
 * this function creates a new database at the path you selected.
 * when the new database is created, the DDL files would execute that the schema was build
 */
void SqliteConnector::createDatabase(const QString &path) {
    // TODO: checke ob datei bereits vorhanden ist und gebe eine warnung "wollen sie die datei wirklich überschreiben"
    //  aus. dann muss die Alte datei Gelöscht werden und eine neue angelegt werden"
    // TODO: logging system einführen
    qDebug() << "erstelle klasse an " << path;
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);
    if (!_db.open()) {
        qDebug() << "can't create or open database file on path " << path;
        return;
    }

    QSqlQuery query;

    // load DDL files to create schema
    QStringList scriptPathList;

    scriptPathList.append(":/database/DDL_DatabaseSchema.sql");
    scriptPathList.append(":/database/DDL_DatabaseInserts.sql");

    QFile file;

    for (const QString &scriptPath : scriptPathList) {
        if (!QFile::exists(scriptPath)) {
            qDebug() << "can't open script path " << scriptPath;
            return;
        }
        file.setFileName(scriptPath);
        file.open(QIODevice::ReadOnly);

        QTextStream stream(&file);

        QString sqlScript(stream.readAll());
        QStringList sqlStatements = sqlScript.split(";", QString::SkipEmptyParts);
        for (const QString &sqlStatement : sqlStatements) {
            if (!query.exec(sqlStatement)) {
                qDebug() << "Failed: " << sqlStatement << "\n Reason: " << query.lastError().driverText()
                         << query.lastError().databaseText();
            }
        }

        file.close();
    }
}
