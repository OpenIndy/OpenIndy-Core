TEMPLATE = subdirs

SUBDIRS = sensorconfig

INSTALLS = 

QT = core

QMAKE_EXTRA_TARGETS += run-test
win32-msvc* {
run-test.commands = \
    if not exist reports mkdir reports & if not exist reports exit 1 $$escape_expand(\n\t)\
    cd $$shell_quote($$OUT_PWD/sensorconfig) && $(MAKE) run-test
} else:win32-g++ {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C $$shell_quote($$OUT_PWD/sensorconfig) run-test
} else:linux {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C sensorconfig run-test
}
