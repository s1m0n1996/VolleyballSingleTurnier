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
void SqliteConnector::createDatabase(const QString path)
{
    if (_db.isOpen())
    {
        _db.close();
    }

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
            if (sqlStatement.length() > 5)
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

    // save default picture in database
    QFile defaultPicture;
    defaultPicture.setFileName(":/img/user.png");
    if (defaultPicture.open(QIODevice::ReadOnly))
    {
        QString sqlPrepare = R"(
INSERT INTO player_pictures_list (picture)
VALUES (:picture)
)";
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":picture", defaultPicture.readAll());
        sqlQuery.exec();
    }

    emit databaseChanged();
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
bool SqliteConnector::openDatabase(QString path)
{
    if (!QFile::exists(path))
    {
        qCritical() << "can't open database file from location " << path;
        return false;
    }

    if (_db.isOpen())
    {
        _db.close();
    }

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);
    if (!_db.open())
    {
        qCritical() << "can't create or open database file on path " << path;
        return false;
    }
    _saveLastPath(const_cast<QString&>(path));
    emit databaseChanged();
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
 * \brief Execute a sql query with sqlQuery oject
 * \param[in] sqlQuery object
 * \return the requested data from the sqlStatement
 */
QList<QList<QVariant>> SqliteConnector::sqlQuery(QSqlQuery& sqlQuery)
{
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
            QString errorMsg;
            errorMsg += "Query: " + sqlQueryObject.executedQuery();
            errorMsg += "\nReason: " + sqlQueryObject.lastError().driverText();
            errorMsg += sqlQueryObject.lastError().databaseText();
            errorMsg += "\n With parameters:";
            for (const auto& key : sqlQueryObject.boundValues().keys())
            {
                errorMsg += "\n";
                errorMsg += "    " + key + " = " + sqlQueryObject.boundValues()[key].toString();
            }

            // wenn das programm hier abstürzt muss die sqlQuery überprüft werden. Um erst einmal weiter
            // zu machen kann das qFatal zu QWarning geändert werden. Dann gibt es lediglich eine warnung
            qFatal("%s", errorMsg.toLatin1().constData());
        }
    }
        // if a sql string was given call the .exec() method with this parameter
    else
    {
        if (!sqlQueryObject.exec(sqlQueryString))
        {
            QString errorMsg;
            errorMsg += "Query: " + sqlQueryObject.executedQuery();
            errorMsg += "\nReason: " + sqlQueryObject.lastError().driverText();
            errorMsg += sqlQueryObject.lastError().databaseText();
            errorMsg += "\n With parameters:";
            for (const auto& key : sqlQueryObject.boundValues().keys())
            {
                errorMsg += "\n";
                errorMsg += "    " + key + " = " + sqlQueryObject.boundValues()[key].toString();
            }

            qFatal("%s", errorMsg.toLatin1().constData());
        }
    }

    // convert the returned data in a pretty 2-dimensional list
    return _convertReturnedData(sqlQueryObject);
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
    }
    else
    {
        return false;
    }
}

/*!
 * \brief convert the returnes sql data
 *
 * \param[in] the sql query data object
 *
 * This method converts the sql query objekt in a 2 dimensional list.
 * The sqlQuery object must be executed before.
 */
QList<QList<QVariant>> SqliteConnector::_convertReturnedData(QSqlQuery& sqlQuery)
{
    // convert the returned data in a pretty 2-dimensional list
    QList<QList<QVariant>> data; // the complete returned table
    while (sqlQuery.next())
    {

        int column_id = 0;
        QList<QVariant> columns;
        while (sqlQuery.value(column_id).isValid())
        {
            columns.append(sqlQuery.value(column_id));
            column_id++;
        }
        data.append(columns);
    }
    return data;
}

/*!
 * \brief Gibt den aktuell geladenen Datenbanknamen aus
 *
 * \return aktuellen Namen der Datenbank
 *
 */
QString SqliteConnector::getDatabaseName()
{
    QString path = _db.databaseName();

    if (QFile(path).exists())
    {
        return QFileInfo(path).fileName().split(".").first();
    }
    return path;
}

/*!
 * \brief Gibt den 7zurück ob die Datenbank geöffnet ist.
 *
 * \return ob dieDatenbank offen ist
 *
 */
bool SqliteConnector::isDatabaseOpen(void)
{
    return _db.isOpen();
}
