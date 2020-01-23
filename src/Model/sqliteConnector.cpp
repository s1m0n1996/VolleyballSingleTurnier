/*!
* \file sqliteConnector.cpp
* \brief this class make the connection to the sqLite Database
* \author Simon Flörke
*/

#include "sqliteConnector.h"

void SqliteConnector::createDatabase(const QString &path) {
    // TODO: checke ob datei bereits vorhanden ist und gebe eine warnung "wollen sie die datei wirklich überschreiben"
    //  aus. dann muss die Alte datei Gelöscht werden und eine neue angelegt werden"
    // TODO: logging system einführen
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
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream stream(&file);

        QString sqlScript(stream.readAll());
        QStringList sqlStatements = sqlScript.split(";", QString::SkipEmptyParts);
        for (const QString &sqlStatement : sqlStatements) {
            if (sqlStatement != "") {
                if (!query.exec(sqlStatement)) {
                    qDebug() << "Failed: " << sqlStatement << "\n Reason: " << query.lastError().driverText()
                             << query.lastError().databaseText();
                }
            }
        }

        file.close();
    }
}

void SqliteConnector::openDatabase(const QString &path) {
    if (!QFile::exists(path)) {
        qDebug() << "can't open database file from location " << path;
        return;
    }
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);
    if (!_db.open()) {
        qDebug() << "can't create or open database file on path " << path;
        return;
    }
}

QList<QList<QVariant>> SqliteConnector::sqlQuery(const QString& sqlStatement) {

    QSqlQuery sqlQuery;

    if (!_db.open()) {
        qDebug() << "database is not opened";
    }

    return _executeQuery(sqlQuery, sqlStatement);
}

QList<QList<QVariant>> SqliteConnector::sqlQuery(const QString &sqlPrepare, const QStringList &parameters) {

    if (!_db.open()) {
        qDebug() << "database is not opened";
    }

    QSqlQuery sqlQuery;
    // set prepare string to sqlQuery Object
    if (!sqlQuery.prepare(sqlPrepare)) {
        qDebug() << sqlQuery.lastError();
    }

    // add parameter to the prepare string
    for (const QString &parameter: parameters) {
        sqlQuery.addBindValue(parameter);
    }

    return _executeQuery(sqlQuery);
}

QList<QList<QVariant>> SqliteConnector::_executeQuery(QSqlQuery &sqlQueryObject, const QString& sqlQueryString) {

    // if no sql string was given call the .exec() method without parameter
    if (sqlQueryString == "") {
        if (!sqlQueryObject.exec()) {
            qDebug() << "Failed: " << sqlQueryObject.executedQuery() << "\n Reason: "
                     << sqlQueryObject.lastError().driverText()
                     << sqlQueryObject.lastError().databaseText();
        }
    }
    // if a sql string was given call the .exec() method with this parameter
    else {
        if (!sqlQueryObject.exec(sqlQueryString)) {
            qDebug() << "Failed: " << sqlQueryObject.executedQuery() << "\n Reason: "
                     << sqlQueryObject.lastError().driverText()
                     << sqlQueryObject.lastError().databaseText();
        }
    }

    // convert the returned data in a pretty 2-dimensional list
    QList<QList<QVariant>> data; // the complete returned table
    while (sqlQueryObject.next()) {

        int column_id = 0;
        QList<QVariant> columns;
        while (sqlQueryObject.value(column_id).isValid()) {
            columns.append(sqlQueryObject.value(column_id));
            column_id++;
        }
        data.append(columns);
    }
    return data;
}

void SqliteConnector::printTable(const QList<QList<QVariant>> &table) {

    for (auto &row : table) {
        QString printedColumn;
        for (auto &column : row) {
            printedColumn.append(column.toString());
            printedColumn.append(", ");
        }
        qDebug() << printedColumn;
    }
}
