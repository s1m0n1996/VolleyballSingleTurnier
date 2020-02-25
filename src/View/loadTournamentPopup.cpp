
#include "loadTournamentPopup.h"

class QComboBox;

class QGridLayout;

/*!
 * \brief Konstruktor
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 *
 */
LoadTournamentPopup::LoadTournamentPopup()
{
    _gameManagement = &GameManagement::instance();
    _createWidgets();
    _setLayout();
    _connect();
}

/*!
 * \brief Destruktor
 *
 * Löscht alle erzeugten objekte auf dem Heap.
 */
LoadTournamentPopup::~LoadTournamentPopup()
{
    delete _comboBox;
    delete _firstLabel;
    delete _loadButton;
};

/*!
 * \brief Erstelle widgets
 *
 * In dieser Methode werden alle widget objekte erzeugt.
 */
void LoadTournamentPopup::_createWidgets()
{
    setWindowTitle("Turnier Laden");
    setWindowIcon(QIcon(":/img/darts.png"));

    _firstLabel = new WindowLabel("Bitte ein Turnier auswählen");
    _firstLabel->setTitleStyel();
    _firstLabel->setMaximumHeight(100);

    _comboBox = new QComboBox;
    _comboBox->setMinimumHeight(50);
    _comboBox->setMaximumHeight(100);
    _comboBox->setMinimumWidth(300);
    _comboBox->setStyleSheet("QComboBox{"
                             "font-size: 30px;"
                             "font-family: Candara;}");

    _loadButton = new WindowButton("Turnier Laden");


    _loadAllTournamentsFromDatabase();
}

/*!
 * \brief Setze Layout
 *
 * In dieser Methode wird das Layout für das komplette Fester gesetzt.
 */
void LoadTournamentPopup::_setLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_firstLabel);
    mainLayout->addWidget(_comboBox);

    QHBoxLayout* footerLayout = new QHBoxLayout;
    footerLayout->setSpacing(1);
    footerLayout->addWidget(_loadButton, 0, Qt::AlignRight);

    mainLayout->addLayout(footerLayout);
    QSpacerItem* test = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addSpacerItem(test);

    setLayout(mainLayout);

    setFixedWidth(size().width());

}

/*!
 * \brief Verbinde
 *
 * In dieser Methode werden buttons mit funktionen verbunden
 */
void LoadTournamentPopup::_connect()
{
    connect(_loadButton, SIGNAL(released()), this, SLOT(_loadTournamentForGame()));
}

/*!
 * \brief Lade alle Tourniere der Aktuellen aus der Datenbank
 *
 * In dieser Methode werden alle verfügbaren Tourniere der aktuellen Sportart geladen
 * und der comboBox hinzugefügt
 */
void LoadTournamentPopup::_loadAllTournamentsFromDatabase()
{
    QList<QList<QString>> savedTournaments = _gameManagement->getSavedTournaments();

    for (QList<QString>& tournament : savedTournaments)
    {
        const QString name = tournament[0] + _splitNameAndDate + tournament[1];
        _comboBox->addItem(name);
    }
}

/*!
 * \brief Lade ausgewähltes Tournier
 *
 * In dieser Methode wird das ausgewählte Turnier geladen.
 * Dadurch werden die Membervariablen der Singleton-Klasse GameboardManagement mit dem des Aktuellen Toutnieres
 * überschrieben, und das Programm holt sich die Daten für das jetzt ausgewählte Tournier.
 */
void LoadTournamentPopup::_loadTournamentForGame()
{
    QList<QString> loadTournament = _comboBox->currentText().split(_splitNameAndDate);

    _gameManagement->loadOtherTournament(loadTournament[0], loadTournament[1]);
    window()->close();
}
