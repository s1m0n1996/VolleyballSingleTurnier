; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "VolleyballPlanner"
#define MyAppVersion "1.0.1"
#define MyAppExeName "Sportveranstaltung.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{9F2AA9D0-17C9-4EE8-AE6F-71A18FCF5DD5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=C:\Users\simon\OneDrive\Dokumente\01_Schule\PHWT\01_Vorlesungen\07_SWE Software Engeneering\Sportveranstaltung\Sportveranstaltung\setup
OutputBaseFilename=VolleyballPlanerSetup
SetupIconFile=C:\Users\simon\OneDrive\Dokumente\01_Schule\PHWT\01_Vorlesungen\07_SWE Software Engeneering\Sportveranstaltung\Sportveranstaltung\rsrc\img\volley.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\simon\OneDrive\Dokumente\01_Schule\PHWT\01_Vorlesungen\07_SWE Software Engeneering\Sportveranstaltung\Sportveranstaltung\src\cmake-build-release-qtmingw\Sportveranstaltung.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\simon\OneDrive\Dokumente\01_Schule\PHWT\01_Vorlesungen\07_SWE Software Engeneering\Sportveranstaltung\Sportveranstaltung\lib\qt\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

