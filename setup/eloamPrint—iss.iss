; -- Example2.iss --
; Same as Example1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=良田1比1打印软件
AppVersion=1.0.1
DefaultDirName={pf}\EloamPrint
DefaultGroupName=良田1比1打印软件V1.0
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\EloamPrint.exe
OutputBaseFilename=EloamPrint
OutputDir=..\outputSetupIconFile=..\Win32\Release\EloamPrint.ico
Compression=lzma
SolidCompression=yes
ChangesEnvironment=true

UsePreviousGroup = no
UsePreviousAppDir = no

[Languages]
Name: "cn"; MessagesFile: "compiler:Languages/Chinese.isl" 

[Files]
Source: "..\Win32\Release\avcodec-55.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\avdevice-55.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\avfilter-4.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\avformat-55.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\avutil-52.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\ed.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\el.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\lang.xml"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\language.ini"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\QDilgCameraConfig.ini"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\hook.dll"; DestDir: "{app}"; Flags: replacesameversion
 Source: "..\Win32\Release\capturebase.dll"; DestDir: "{app}"; Flags: replacesameversion

Source: "..\Win32\Release\eloamDll.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\EloamUtility.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\FreeImage.dll"; DestDir: "{app}"; Flags: replacesameversionSource: "..\Win32\Release\eSPCapLib.dll"; DestDir: "{app}"; Flags: replacesameversion                           

;Source: "..\Win32\Release\libiconv-2.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\libzbar-0.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\Log.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\postproc-52.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\swresample-0.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\swscale-2.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\UVCXUControl.dll"; DestDir: "{app}"; Flags: replacesameversion
;定制PID
Source: "..\Win32\Release\video.flt"; DestDir: "{app}"; Flags: replacesameversionSource: "..\Win32\Release\EloamPrint.exe"; DestDir: "{app}"; Flags: replacesameversionSource: "..\Win32\Release\EloamPrint.chm"; DestDir: "{app}"; Flags: replacesameversion


Source: "..\Win32\Release\videoCap.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\edcom.dll"; DestDir: "{app}"; Flags: replacesameversionSource: "..\Win32\Release\ExtensionInterFaceDll.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\1500DLL\*"; DestDir: "{app}\1500DLL"; Flags: replacesameversion

Source: "..\Win32\Release\platforms\*"; DestDir: "{app}\platforms"; Flags:replacesameversion
;二代证相关库
Source: "..\Win32\Release\elr.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\sdtapi.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\trendcote.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\WltRS.dll"; DestDir: "{app}"; Flags: replacesameversion

Source: "..\Win32\Release\idcard_trend.dll"; DestDir: "{app}"; Flags: replacesameversion
;Source: "..\Win32\Release\plugin.ini"; DestDir: "{app}"; Flags: replacesameversion

Source: "..\Win32\Release\msvcp100.dll"; DestDir: "{app}"
Source: "..\Win32\Release\msvcr100.dll"; DestDir: "{app}"
Source: "..\Win32\Release\mfc100.dll"; DestDir: "{app}"

Source: "..\Win32\Release\Qt5Core.dll"; DestDir: "{app}"
Source: "..\Win32\Release\Qt5Gui.dll"; DestDir: "{app}"
Source: "..\Win32\Release\Qt5Widgets.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icudt51.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icuin51.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icuuc51.dll"; DestDir: "{app}"
;Source: "..\Win32\Release\depends\Qt5Xml.dll"; DestDir: "{app}"
 
Source: "..\Win32\Release\MJPGDriver.exe"; DestDir: "{app}"

[Icons]
Name: "{group}\良田1比1打印软件V1.0"; Filename: "{app}\EloamPrint.exe"
Name: "{group}\{cm:UninstallProgram, 良田高拍仪软件}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\良田1比1打印软件V1.0"; Filename: "{app}\EloamPrint.exe"

[Run] 
Filename: "{app}\MJPGDriver.exe"; Parameters: "/verysilent /norestart"; 
Filename: "{app}\EloamPrint.exe"; Description: "{cm:LaunchProgram,良田1比1打印软件}" ;Flags: postinstall nowait skipifsilent

