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

/*
 * the SqliteConnector is a singleton class. It makes the connection to the SqLite database
 */
class SqliteConnector final {

public:
    static SqliteConnector &instance(void) {
        static SqliteConnector instance;
        return instance;
    }

    void createDatabase(const QString &path);

    void openDatabase(const QString path);

    auto *sqlQuery(const QString query);


private:
    QSqlDatabase _db;

    SqliteConnector(void) = default;

    ~SqliteConnector(void) = default;

    SqliteConnector(const SqliteConnector &) = delete;  // copy Constructor
    SqliteConnector &operator=(const SqliteConnector &) = delete; // Kopierzuweisungsoperator


};


#endif //SPORTVERANSTALTUNG_SQLITECONNECTOR_H
