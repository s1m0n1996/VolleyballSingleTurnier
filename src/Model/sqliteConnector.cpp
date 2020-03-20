
#include "Model/sqliteConnector.h"

SqliteConnector::SqliteConnector(void)
{
    _loadLastDatabase();
}


/*!
 * \brief Erstelle eine neue Datenbank.
 *
 * \param[in] path der absolute pfad, wo die neue Datenbank gespeichert werden soll.
 * \return void
 *
 * Es wird eine neue sqLite an dem eingegebenen Pfad Datenbank erstellt.
 * Wird das Datenbankschema erstellt, sodass die Datenbank benutzt werden kann.
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
 * \brief Öffne existierende Datenbank
 *
 * \param[in] path der absolute pfad der sqLite Datenbank.
 * \return void
 *
 * Wenn ein pfad üpbergeben wird, der nicht existiert, oder wo keine Datenbank hinter liegt,
 * wird eine Warnung ausgegeben.
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
 * \brief Fphre die sql Anfrage aus
 * \param[in, out] sqlQuery Objekt
 * \return die zurückgegebenen daten in einer 2 dimensionalen liste
 *
 * Es werden die angefragten daten in tabellenform zurückgegeben. Die Tabelle ist als 2 Dimensionalen Liste aufgebaut.
 * Dabei steht die erste Dimension für die Zeilen und die zweite für die Spalten.
 *
 * Wenn die anfrage fehlschlägt stürzt das programm mit einem qFatal() ab.
 *
 * Es kann auch sein dass die angefragten daten nicht existieren, dann wird eine leere Liste zurückgegeben.
 */
QList<QList<QVariant>> SqliteConnector::sqlQuery(QSqlQuery& sqlQuery)
{
    return _executeQuery(sqlQuery);
}

/*!
 * \brief Führe eine sql anfrage aus
 *
 * \param[in,out] sqlQueryObject das query object welches ausgeführt werden soll.
 *
 * This Function executes an sql query. If the Query fails you get a error message why it fails.
 * If the sqlQuery successful runs, you get the data in form a matrix form.
 */
QList<QList<QVariant>> SqliteConnector::_executeQuery(QSqlQuery& sqlQueryObject)
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

    // convert the returned data in a pretty 2-dimensional list
    return _convertReturnedData(sqlQueryObject);
}

/*!
 * \brief Gebe tabelle auf der Konsole aus
 *
 * \param[in] table zurückgegebene daten der Datenbank
 *
 * Die einegebene Tabelle oder 2 Dimensionale liste wird in der console ausgegeben.
 * Dies dient nur zum besseren debugging.
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
 * \brief Speicher den Pfad in Datei
 *
 * \param[in] Dateipfad von der letzten geladenen Datenbank
 *
 * Es wird die letzte Geladene Datenbank in einer Datei auf der Festplatte Gespeichert, um bei einem erneuten
 * Programstart diesen pfad wieder auszulesen und zu öffnen.
 *
 * Die .tmp datei wird neben der .exe gespeichert
 * This method save the last db file path in a .tmp file near the .exe
 *
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
 * \brief Lase zuletzt benutzte Datenbank
 *
 * Es wird die letzte Benutzte Datenbank geladen.
 * Dazu wird der letzte pfad aus der .temp datei geladen, welcher in der Methode _saveLastPath gespeichert wurde.
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
 * \brief Konvetiere daten
 *
 * \param[in] sqlQuery object
 *
 * die zurückgegebenen daten der Datenbank werden in eine 2 dimensionale Liste konvetiert.
 * Das sql Objekt muss voher ausgeführt werden.
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
 * \brief Gibt zurück ob die Datenbank geöffnet ist.
 *
 * \return true, wenn die Datenbank geöffet ist sonst false
 *
 */
bool SqliteConnector::isDatabaseOpen(void)
{
    return _db.isOpen();
}
