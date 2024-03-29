#ifndef SPORTVERANSTALTUNG_SQLITECONNECTOR_H
#define SPORTVERANSTALTUNG_SQLITECONNECTOR_H

#include <QtCore>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QList>
#include <QDate>

/*!
 * \file sqliteConnector.h
 * \author Simon Flörke
 * \brief Eine Singleton Klasse, welche die verbindung zur datenbank übernimmt.
 *
 * Die Klasse kümmert sich um die verbindung der Datenbank.
 * Schlägt etwas fehl werden sollvolle Fehlermeldungen ausgegeben.
 *
*/
class SqliteConnector : public QObject
{
Q_OBJECT

public:
    static SqliteConnector &instance(void) {
        static SqliteConnector instance;
        return instance;
    }

    bool isDatabaseOpen(void);

    void createDatabase(const QString path);

    bool openDatabase(const QString path);

    QString getDatabaseName(void);

    QList<QList<QVariant>> sqlQuery(QSqlQuery& sqlQuery);

    static void printTable(const QList<QList<QVariant>> &table);

signals:
    void databaseChanged(void);



private:
    QSqlDatabase _db;
    const QString _lastOpenedDbFilePath = QCoreApplication::applicationDirPath() + "/" + "lastOpenedDb.tmp";

    SqliteConnector(void);

    ~SqliteConnector(void) = default;

    SqliteConnector(const SqliteConnector &) = delete;  // copy Constructor
    SqliteConnector &operator=(const SqliteConnector &) = delete; // Kopierzuweisungsoperator

    static QList<QList<QVariant>> _executeQuery(QSqlQuery& sqlQueryObject);
    static QList<QList<QVariant>> _convertReturnedData(QSqlQuery& sqlQuery);
    void _saveLastPath(QString& path);
    bool _loadLastDatabase(void);

};


#endif //SPORTVERANSTALTUNG_SQLITECONNECTOR_H
