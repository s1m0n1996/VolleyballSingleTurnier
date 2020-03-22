#include <QDateEdit>
#include <QGridLayout>

#include "Model/gameManagement.h"

#include "View/createTournamentPopup.h"
#include <View/tournamentWindow.h>
#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"

/*!
 * \brief Konstruktor
 *
 * Initialisiert die Klasse
 */
CreateTournamentPopUp::CreateTournamentPopUp(QWidget* parent) : QWidget(parent)
{
    _gameManagement = &GameManagement::instance();
    setWindowTitle("Turnier erstellen");
    setWindowIcon(QIcon(":/img/darts.png"));
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);

    createWidgets();

    setAllLayout();

    connecting();
}

/*!
 * \brief Setze enable Status vom tournier erstellen Button
 *
 * Das Turnier kann erst erstellt werden, wenn in beiden Feldern etwas eingegeben wurde
 */
void CreateTournamentPopUp::enableCreateTournamentButton(void)
{
    QRegExp re(R"(^\d{4}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$)");

    if ((re.exactMatch(_dateEdit->text())) && !(_nameEdit->text().isEmpty()))
    {
        _createTournament->setEnabled(true);
    }
}

/*!
 * \brief Öffne das Fenster um ein anderes Turnuer zu laden
 *
 * Es wird das aktuell ausgewählte Turnier Geladen.
 */
void CreateTournamentPopUp::openTournamentWindow(void)
{
    setWindowFlags(Qt::Window);
    QString tournamentName = _nameEdit->text();
    QDate tournamentDate = QDate::fromString(_dateEdit->text(), "yyyy-MM-dd");

    _gameManagement->createNewTournament(tournamentName, tournamentDate);
    _gameManagement->loadOtherTournament(tournamentName, tournamentDate);
}

/*!
 * \brief Erstelle alle Objekte die Benötigt werden
 *
 * Es werden Objekte erstellt, die für die Klasse benötigt werden.
 */
void CreateTournamentPopUp::createWidgets(void)
{
    _dateLabel = new WindowLabel("Turnierdatum");
    _dateEdit = new WindowEdit((QDate::currentDate().toString("yyyy-MM-dd")), DataType::date);
    _dateEdit->setText(QDate::currentDate().toString("yyyy-MM-dd"));

    _date = new QDateEdit();
    _date->setCalendarPopup(true);
    _date->setStyleSheet("QDateEdit::drop-down {"
                         "image:url(:/img/calendar.png);"
                         "spacing:5px;"
                         "width:60px;"
                         "height:25px;"
                         "subcontrol-position: right top;"
                         "subcontrol-origin:margin; }"
                         "QDateEdit{"
                         "font-size: 20px;"
                         "font-family: Arial;"
                         "spacing: 5px;}"
                         "QCalendarWidget{"
                         "font-size: 20px;"
                         "font-family: Arial;}");
    QDate currentDate = QDate::currentDate();
    _date->setDate(currentDate);

    _nameLabel = new WindowLabel("Turniername");
    _nameEdit = new WindowEdit("Turniername", DataType::name);
    _nameEdit->setTournamentPopUpStyle();
    _nameEdit->setMaxLength(20);

    _createTournament = new WindowButton("Erstelle Turnier");
    _createTournament->setEnabled(false);

}

void CreateTournamentPopUp::setAllLayout(void)
{
    QGridLayout* layout = new QGridLayout;

    layout->addWidget(_dateLabel, 0, 0);
    layout->addWidget(_date, 0, 1);
    layout->addWidget(_nameLabel, 1, 0);
    layout->addWidget(_nameEdit, 1, 1);
    layout->addWidget(_createTournament, 2, 2);
    setLayout(layout);

}

void CreateTournamentPopUp::connecting(void)
{
    connect(_createTournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));
    connect(_dateEdit, SIGNAL(textChanged(
                                      const QString &)), this, SLOT(enableCreateTournamentButton()));
    connect(_nameEdit, SIGNAL(textChanged(
                                      const QString &)), this, SLOT(enableCreateTournamentButton()));
}
