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

/// The connection to the SQLite database.
/*
 * the SqliteConnector is a singleton class. It makes the connection to the SqLite database
 */
class SqliteConnector final {

public:
    static SqliteConnector &instance(void) {
        static SqliteConnector instance;
        return instance;
    }

    /// Create a new database.
    /*
     * This method creates a new database at the path you selected.
     * when the new database is created, the DDL files would execute and the schema was build.
     */
    void createDatabase(const QString &path);

    /// Open an existing database.
    /*
     * This Method open an existing database.
     * If you want to open a database file that not exist, the a error was aborted the opening process.
     */
    void openDatabase(const QString &path);

    /// Execute a sqlQuery With a QString.
    /*
     * With this method you can execute a pure QString in the database.
     * You get the requested information from the table in a matrix form (2-dimensional list).
     * If the Query fails, the function aborted with an error message and returned an empty list.
     */
    QList<QList<QVariant>> sqlQuery(const QString& sqlStatement);

    /// Execute a sqlQuery With a incomplete sql string and a list of parameter to complete the sql string.
    /*
     * With this method you can execute a SQL query with 2 parameter.
     * You get the requested information from the table in a matrix form (2-dimensional list).
     * If the Query fails, the function aborted with an error message and returned an empty list.
     *
     * The sqlPrepare parameter needs a string in this format:
     * "INSERT INTO player_list (id, name, birthday, country, is_available) VALUES (?, ?, ?, ?, 0);"
     * You see that this string have 4 placeholder in form of ? char.
     * So the second parameters parameter list needs 4 elements.
     */
    QList<QList<QVariant>> sqlQuery(const QString &sqlPrepare, const QStringList &parameters);

    /// Print a table.
    /*
     * This method print a requested table in qDebug() for easier debugging.
     */
    static void printTable(const QList<QList<QVariant>> &table);


private:
    QSqlDatabase _db;

    SqliteConnector(void) = default;

    ~SqliteConnector(void) = default;

    SqliteConnector(const SqliteConnector &) = delete;  // copy Constructor
    SqliteConnector &operator=(const SqliteConnector &) = delete; // Kopierzuweisungsoperator

    /// executes a sql query.
    /*
     * This Function executes an sql query. If the Query fails you get a error message why it fails.
     * If the sqlQuery successful runs, you get the data in form a matrix form.
     */
    static QList<QList<QVariant>> _executeQuery(QSqlQuery& sqlQueryObject, const QString& sqlQueryString = "");

};


#endif //SPORTVERANSTALTUNG_SQLITECONNECTOR_H
