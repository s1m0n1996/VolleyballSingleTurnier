/*!
* \file sqliteConnector.cpp
* \brief this class make the connection to the sqLite Database
* \author Simon Flörke
*/

#include "sqliteConnector.h"

SqliteConnector::SqliteConnector(void)
{
    _loadLastDatabase();
}


/*!
 * \brief Create a new database.
 *
 * \param[in] path the absolute file path and file name with data type (.sqlite)
 * \return void
 *
 * This method creates a new database at the path you selected.
 * when the new database is created, the DDL files would execute and the schema was build.
 *
 */
void SqliteConnector::createDatabase(const QString& path)
{
    // TODO: checke ob datei bereits vorhanden ist und gebe eine warnung "wollen sie die datei wirklich überschreiben"
    //  aus. dann muss die Alte datei Gelöscht werden und eine neue angelegt werden"
    // TODO: logging system einführen
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);
    if (!_db.open())
    {
        qCritical() << "can't create or open database file on path " << path;
        return;
    }

    QSqlQuery query;

    // load DDL files to create schema
    QStringList scriptPathList;

    scriptPathList.append(":/database/DDL_DatabaseSchema.sql");
    scriptPathList.append(":/database/DDL_DatabaseInserts.sql");

    QFile file;

    for (const QString& scriptPath : scriptPathList)
    {
        if (!QFile::exists(scriptPath))
        {
            qCritical() << "can't open script path " << scriptPath;
            return;
        }
        file.setFileName(scriptPath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream stream(&file);

        QString sqlScript(stream.readAll());
        QStringList sqlStatements = sqlScript.split(";", QString::SkipEmptyParts);
        for (const QString& sqlStatement : sqlStatements)
        {
            if (sqlStatement != "")
            {
                if (!query.exec(sqlStatement))
                {
                    qWarning() << "Failed: " << sqlStatement << "\n Reason: " << query.lastError().driverText()
                               << query.lastError().databaseText();
                }
            }
        }

        file.close();
    }
    _saveLastPath(const_cast<QString&>(path));
}

/*!
 * \brief Open an existing database.
 *
 * \param[in] path the absolute file path and file name with data type (.sqlite)
 * \return void
 *
 * This Method open an existing database.
 * If you want to open a database file that not exist, the a error was aborted the opening process.
 */
bool SqliteConnector::openDatabase(const QString& path)
{
    if (!QFile::exists(path))
    {
        qCritical() << "can't open database file from location " << path;
        return false;
    }
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);
    if (!_db.open())
    {
        qCritical() << "can't create or open database file on path " << path;
        return false;
    }
    _saveLastPath(const_cast<QString&>(path));
    return true;
}

/*!
 * \brief Execute a sqlQuery With a QString.
 *
 * \param[in] sqlStatement a String with the pure SQL-Query like: "SELECT * FROM player_list;"
 * \return the requested data from the sqlStatement
 *
 * With this method you can execute a pure QString in the database.
 * You get the requested information from the table in a matrix form (2-dimensional list).
 * If the Query fails, the function aborted with an error message and returned an empty list.
 */
QList<QList<QVariant>> SqliteConnector::sqlQuery(const QString& sqlStatement)
{

    QSqlQuery sqlQuery;

    if (!_db.open())
    {
        qCritical() << "database is not opened";
    }

    return _executeQuery(sqlQuery, sqlStatement);
}

/*!
 * \brief Execute a sqlQuery With a incomplete sql string and a list of parameter to complete the sql string.
 *
 * \param[in] sqlPrepare the SQL-Query with "?" as Placeholder for the variables from the parameters list. For example
 * "INSERT INTO player_list (name, birthday, country) VALUES (?, ?, ?)"
 * \param[in] parameters a list to replace the elements with the "?" in the sqlPrepare string
 * \return the requested data from the sqlStatement
 *
 * With this method you can execute a SQL query with 2 parameter.
 * You get the requested information from the table in a matrix form (2-dimensional list).
 * If the Query fails, the function aborted with an error message and returned an empty list.
 *
 * The sqlPrepare parameter needs a string in this format:
 * "INSERT INTO player_list (id, name, birthday, country, is_available) VALUES (?, ?, ?, ?, 0);"
 * You see that this string have 4 placeholder in form of ? char.
 * So the second parameters parameter list needs 4 elements.
 */
QList<QList<QVariant>> SqliteConnector::sqlQuery(const QString& sqlPrepare, const QStringList& parameters)
{

    if (!_db.open())
    {
        qCritical() << "database is not opened";
    }

    QSqlQuery sqlQuery;
    // set prepare string to sqlQuery Object
    if (!sqlQuery.prepare(sqlPrepare))
    {
        qWarning() << sqlQuery.lastError();
    }

    // check parameter count
    if (parameters.count() != sqlPrepare.count("?"))
    {
        qWarning() << "the number of parameters is wrong. You need " << sqlPrepare.count("?") << "in the sql string "
                   << sqlPrepare << " but in the list was " << parameters.count();
    }

    // add parameter to the prepare string
    for (const QString& parameter: parameters)
    {
        sqlQuery.addBindValue(parameter);
    }

    return _executeQuery(sqlQuery);
}

/*!
 * \brief executes a sql query.
 *
 * This Function executes an sql query. If the Query fails you get a error message why it fails.
 * If the sqlQuery successful runs, you get the data in form a matrix form.
 */
QList<QList<QVariant>> SqliteConnector::_executeQuery(QSqlQuery& sqlQueryObject, const QString& sqlQueryString)
{

    // if no sql string was given call the .exec() method without parameter because the parameters was
    // bind at the object before
    if (sqlQueryString == "")
    {
        if (!sqlQueryObject.exec())
        {
            qWarning() << "Failed: " << sqlQueryObject.executedQuery() << "\n Reason: "
                       << sqlQueryObject.lastError().driverText()
                       << sqlQueryObject.lastError().databaseText();
        }
    }
        // if a sql string was given call the .exec() method with this parameter
    else
    {
        if (!sqlQueryObject.exec(sqlQueryString))
        {
            qWarning() << "Failed: " << sqlQueryObject.executedQuery() << "\n Reason: "
                       << sqlQueryObject.lastError().driverText()
                       << sqlQueryObject.lastError().databaseText();
        }
    }

    // convert the returned data in a pretty 2-dimensional list
    QList<QList<QVariant>> data; // the complete returned table
    while (sqlQueryObject.next())
    {

        int column_id = 0;
        QList<QVariant> columns;
        while (sqlQueryObject.value(column_id).isValid())
        {
            columns.append(sqlQueryObject.value(column_id));
            column_id++;
        }
        data.append(columns);
    }
    return data;
}

/*!
 * \brief Print a table.
 * This method print a requested table in qDebug() for easier debugging.
 */
void SqliteConnector::printTable(const QList<QList<QVariant>>& table)
{

    for (auto& row : table)
    {
        QString printedColumn;
        for (auto& column : row)
        {
            printedColumn.append(column.toString());
            printedColumn.append(", ");
        }
        qDebug() << printedColumn;
    }
}

/*!
 * \brief save the last loaded db file path
 *
 * \param[in]path the path from the last loaded db
 *
 * This method save the last db file path in a .tmp file near the .exe.
 */
void SqliteConnector::_saveLastPath(QString& path)
{
    QFile file;
    file.setFileName(_lastOpenedDbFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream fileStream(&file);
    fileStream << path;

    file.close();
}

/*!
 * \brief load the last db file
 *
 * This method load the last used db file path.
 * load the path from the .tmp file that was stored in the _saveLastPath method.
 */
bool SqliteConnector::_loadLastDatabase()
{
    QFile file;
    file.setFileName(_lastOpenedDbFilePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fileStream(&file);
        QString path = fileStream.readLine();
        file.close();

        return openDatabase(path);
    } else
    {
        return false;
    }
}
