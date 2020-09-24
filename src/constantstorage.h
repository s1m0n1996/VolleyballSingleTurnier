#ifndef BILLCONTROL_CONSTANTSTORAGE_H
#define BILLCONTROL_CONSTANTSTORAGE_H

#include <QDate>
#include <QDebug>
#include <QStandardPaths>

namespace Constants
{

// #####################################################################################################################
// project information
// #####################################################################################################################
    // TODO: better to make it on CMAKE
    //  https://riptutorial.com/cmake/example/32603/using-cmake-to-define-the-version-number-for-cplusplus-usage
    constexpr char projectName[] = "Volleyball Turnier";
    constexpr char author[] = "Simon Flörke";
    constexpr char version[] = "1.0.0";
    constexpr char qtVersion[] = "5.14.2";
    constexpr char buildDate[] = __DATE__  " "  __TIME__;
    static const QString standardDocumentPath =
            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "/" + Constants::projectName;
    static const QString standardTempPath =
            QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0] + "/" + Constants::projectName;
}

#endif //BILLCONTROL_CONSTANTSTORAGE_H
