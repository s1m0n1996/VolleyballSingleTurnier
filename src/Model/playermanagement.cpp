
#include "Model/playermanagement.h"


PlayerManagement::PlayerManagement()
{
    _db = &SqliteConnector::instance();
    _createPlayerTable();
}

/*!
 * \brief get all players
 *
 * \param[in]onlyAvailable default: true. get only players who is the available flag true.
 *
 * \return a list of player objects
 *
 * This method returned all players that are stored in the database.
 * For Example the player_list table from the database have this entries:
 *
 * id   name    birthday    country     is_available
 * 1	Klaus	2010-12-15	DE	        1
 * 2	Hilde	2000-12-15	DE	        1
 * 3	Peter	2020-12-15	DE	        1
 *
 * you get a list of 3 Player objects
 */
QList<Player> PlayerManagement::getAllStoredPlayers(bool onlyAvailable)
{

    QString sqlPrepare = R"(
SELECT id
FROM player_list
WHERE is_available >= ?
ORDER BY name;
    )";

    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(onlyAvailable));

    QList<QList<QVariant>> playerIds = _db->sqlQuery(sqlPrepare, sqlParameters);

    QList<Player> storedPlayers;

    for (const QVariant playerId : playerIds)
    {
        storedPlayers.append(playerId.toList()[0].toInt());
    }

    return storedPlayers;
}

/*!
 * \brief add a player to the actual turnier gameboard
 *
 * \param[in] addPlayer the player that would be added to the actual turnier gameboard
 */
void PlayerManagement::addPlayerForNewGame(const Player addPlayer)
{
    _selectedPlayersForNewGame.append(addPlayer);
}

/*!
 * \brief drop a player to the actual turnier gameboard
 *
 * \param[in] dropPlayer the player from the list that would be added to the actual turnier gameboard
 *
 * This method is the undo function for addPlayerForNewGame
 */
void PlayerManagement::dropPlayerForNewGame(Player dropPlayer)
{
    int i = 0;
    for (const Player& player : _selectedPlayersForNewGame)
    {
        if (dropPlayer.getId() == player.getId())
        {
            _selectedPlayersForNewGame.removeAt(i);
            return;
        }
        i++;
    }
}

/*!
 * \brief count the players they are on the list for the next game
 *
 * \return players of the list
 */
int PlayerManagement::countSelectedPlayersForNewGame()
{
    return _selectedPlayersForNewGame.length();
}

/*!
 * \brief count the players they are missing to start the game
 *
 * \return missing players of the list
 *
 * Count the players they are missing to the next 2^n compatible.
 *
 * Examples:
 * if you have 3 players the next 2^n compatible is 4 and the method returned 1.
 * if you have 4 players it is 2^n compatible and the method returned 0.
 * if you have 5 players the next 2^n compatible is 8 and the method returned 3.
 */
int PlayerManagement::countMissingPlayersForNewGame()
{
    int nActualPlayers = _selectedPlayersForNewGame.length();

    // the calculation not works with 0 because the log(<1) is negative.
    if (0 == nActualPlayers){
        return 2;
    }
    // the calculation not works with 1 because the log(1) = 0
    if (1 == nActualPlayers)
    {
        return 1;
    }

    double actualExponent = log(nActualPlayers) / log(2);
    qDebug() << "Exp: " << actualExponent;
    if ((actualExponent - static_cast<int>(actualExponent)) == 0)
    {
        // the exponent is direct number, so the list is complete
        return 0;
    }
    int nextExponent = static_cast<int>(actualExponent) + 1;
    return static_cast<int>(pow(2, nextExponent)) - nActualPlayers;

}

/*!
 * \brief print the player list for easier debugging
 * count the players they are missing to the next 2^n kompatibität
 */
void PlayerManagement::printPlayersList()
{
    for (const Player& player : _selectedPlayersForNewGame)
    {
        qDebug() << player.getId() << player.getName() << player.getBirthday() << player.getCountry();
    }
}

void PlayerManagement::_createPlayerTable(){

    // give the opened database to the table
    //_playerTable = new QSqlTableModel(nullptr, *_db->getDb());
    _playerTable = new QSqlQueryModel();

    //_playerTable->setTable("player_list");
    _playerTable->setQuery("SELECT name, birthday, country FROM player_list where is_available >= 1");
    //_playerTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //_playerTable->select();

    //_playerTable->setFilter("is_available>=1");

    //_playerTable->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    _playerTable->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    _playerTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Geburtstag"));
    _playerTable->setHeaderData(2, Qt::Horizontal, QObject::tr("Land"));
    //_playerTable->setHeaderData(0, Qt::Horizontal, QObject::tr("is_available"));
}
