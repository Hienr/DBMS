TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    parser.cpp \
    stokenizer.cpp \
    token.cpp \
    fnc_state_machine.cpp \
    main.cpp \
    table.cpp \
    record.cpp \
    fnc_file.cpp \
    set.cpp \
    rpn.cpp \
    utility.cpp \
    sql.cpp \
    filesignature.cpp


HEADERS += \
    bplustree.h \
    map.h \
    mmap.h \
    parser.h \
    stokenizer.h \
    token.h \
    utility.h \
    w_stk_constants.h \
    w_table_constants.h \
    w_parser_constants.h \
    fnc_state_machine.h \
    fnc_array.h \
    table.h \
    record.h \
    fnc_file.h \
    set.h \
    fnc_list.h \
    queue.h \
    list.h \
    stack.h \
    rpn.h \
    sql.h \
    bplustree.h \
    fnc_array.h \
    fnc_file.h \
    fnc_list.h \
    fnc_state_machine.h \
    list.h \
    map.h \
    mmap.h \
    parser.h \
    queue.h \
    record.h \
    rpn.h \
    set.h \
    sql.h \
    stack.h \
    stokenizer.h \
    table.h \
    token.h \
    utility.h \
    w_parser_constants.h \
    w_stk_constants.h \
    w_table_constants.h \
    w_exceptions.h \
    filesignature.h \
    z_work_report.h \
    z_output.h


DISTFILES += \
    ../build-Database-Desktop_Qt_5_11_1_MinGW_32bit-Debug/sample.batch \
    ../build-Database-Desktop_Qt_5_11_1_MinGW_32bit-Debug/Pokemon_List.txt

SUBDIRS += \
    Database.pro \
    Database.pro
