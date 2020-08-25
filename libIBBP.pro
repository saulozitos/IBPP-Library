TARGET       = ibpp
TEMPLATE     = lib

CONFIG      += shared plugin
CONFIG      -= qt

DEFINES     += IBPP_LINUX

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wshadow -Wno-invalid-memory-model -Wuninitialized -Wunknown-pragmas -Wsequence-point -Wno-switch-outside-range -Wpointer-arith -Wsizeof-pointer-memaccess -Wmissing-declarations -Wvector-operation-performance
#QMAKE_CXXFLAGS += -Werror

SOURCES     += source/all_in_one.cpp \
               source/array.cpp \
               source/blob.cpp \
               source/database.cpp \
               source/date.cpp \
               source/dbkey.cpp \
               source/_dpb.cpp \
               source/events.cpp \
               source/exception.cpp \
               source/_ibpp.cpp \
               source/_ibs.cpp \
               source/_rb.cpp \
               source/row.cpp \
               source/service.cpp \
               source/_spb.cpp \
               source/statement.cpp \
               source/time.cpp \
               source/_tpb.cpp \
               source/transaction.cpp \
               source/user.cpp

HEADERS     += source/ibase.h \
               source/iberror.h \
               source/_ibpp.h \
               source/ibpp.h

