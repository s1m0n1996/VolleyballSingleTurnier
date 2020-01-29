/*!
* \file sqliteConnector.cpp
* \brief this class make the connection to the sqLite Database
* \author Simon Flörke
*/

#ifndef SPORTVERANSTALTUNG_SQLITECONNECTOR_H
#define SPORTVERANSTALTUNG_SQLITECONNECTOR_H

#include <QtCore>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QList>

/*!
 * \file sqliteConnector.cpp
 * \author Simon Flörke
 * \brief A singleton class for the connection to the database.
 *
 * This Class handles the full connection to the database.
 *
*/
class SqliteConnector final {

public:
    static SqliteConnector &instance(void) {
        static SqliteConnector instance;
        return instance;
    }

    void createDatabase(const QString &path);

    void openDatabase(const QString &path);

    QList<QList<QVariant>> sqlQuery(const QString& sqlStatement);

    QList<QList<QVariant>> sqlQuery(const QString &sqlPrepare, const QStringList &parameters);

    static void printTable(const QList<QList<QVariant>> &table);


private:
    QSqlDatabase _db;

    SqliteConnector(void) = default;

    ~SqliteConnector(void) = default;

    SqliteConnector(const SqliteConnector &) = delete;  // copy Constructor
    SqliteConnector &operator=(const SqliteConnector &) = delete; // Kopierzuweisungsoperator

    static QList<QList<QVariant>> _executeQuery(QSqlQuery& sqlQueryObject, const QString& sqlQueryString = "");

};


#endif //SPORTVERANSTALTUNG_SQLITECONNECTOR_H
