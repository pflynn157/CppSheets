; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CppSheets"
#define MyAppVersion "1.0"
#define MyAppPublisher "Patrick Flynn"
#define MyAppURL "https://github.com/patrickf2000/CppSheets"
#define MyAppExeName "CppSheets.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{2D20F824-C005-4FA7-87BA-3A42CDE1AE40}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=C:\Users\patrick\Projects_public\CppSheets\COPYING.txt
OutputDir=C:\Users\patrick\Projects_public\CppSheets\build
OutputBaseFilename=CppSheets_setup
Compression=lzma
SolidCompression=yes
DisableFinishedPage=no
DisableWelcomePage=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\CppSheets.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libxlnt.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\patrick\Projects_public\CppSheets\build\winbin\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

