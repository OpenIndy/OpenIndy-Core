
# include paths
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/geometry
INCLUDEPATH += $$PWD/include/util
INCLUDEPATH += $$PWD/include/plugin
INCLUDEPATH += $$PWD/include/plugin/exchange
INCLUDEPATH += $$PWD/include/plugin/function
INCLUDEPATH += $$PWD/include/plugin/networkAdjustment
INCLUDEPATH += $$PWD/include/plugin/sensor
INCLUDEPATH += $$PWD/include/plugin/simulation
INCLUDEPATH += $$PWD/include/plugin/tool
INCLUDEPATH += $$PWD/lib/OpenIndy-Math/include


# libraries
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/bin/release/ -lopenIndyCore1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/bin/debug/ -lopenIndyCore1
else:linux:CONFIG(release, debug|release): LIBS += -L$$PWD/bin/release/ -lopenIndyCore
else:linux:CONFIG(debug, debug|release): LIBS += -L$$PWD/bin/debug/ -lopenIndyCore
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1
else:linux:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release/ -lopenIndyMath
else:linux:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath

