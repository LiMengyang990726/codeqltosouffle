BISON = bison
BISONFLAGS = d
FLEX = flex
CC = g++

PROGRAM = translator
HELPER = helper

.PHONY : clean

all: ${PROGRAM}

${PROGRAM}.tab.c ${PROGRAM}.tab.h: ${PROGRAM}.y
	${BISON} -${BISONFLAGS} $<

lex.yy.c: ${PROGRAM}.l
	${FLEX} $<

${PROGRAM}.tab.o: ${PROGRAM}.tab.c
	${CC} $< -o $@ -c

lex.yy.o: lex.yy.c
	${CC} $< -o $@ -c

${HELPER}/ASTNode.o: ${HELPER}/ASTNode.cpp
	${CC} $< -o $@ -c

${HELPER}/translate.o: ${HELPER}/translate.cpp
	${CC} $< -o $@ -c

${HELPER}/populateMainSouffle.o: ${HELPER}/populateMainSouffle.cpp
	${CC} $< -o $@ -c

${HELPER}/populateRelSouffle.o: ${HELPER}/populateRelSouffle.cpp
	${CC} $< -o $@ -c

${HELPER}/populateVersionSouffle.o: ${HELPER}/populateVersionSouffle.cpp
	${CC} $< -o $@ -c

${HELPER}/symbolStore.o: ${HELPER}/symbolStore.cpp
	${CC} $< -o $@ -c

${HELPER}/utils.o: ${HELPER}/utils.cpp
	${CC} $< -o $@ -c


${PROGRAM}: ${PROGRAM}.tab.o \
			lex.yy.o \
			${HELPER}/ASTNode.o \
			${HELPER}/translate.o \
			${HELPER}/populateMainSouffle.o \
			${HELPER}/populateRelSouffle.o \
			${HELPER}/populateVersionSouffle.o \
			${HELPER}/symbolStore.o \
			${HELPER}/utils.o 
	${CC} -o $@ $^

clean:
	$(RM) ${PROGRAM} \
		  ${PROGRAM}.tab.h \
		  ${PROGRAM}.tab.c \
		  ${PROGRAM}.tab.o \
		  lex.yy.c \
		  lex.yy.o \
		  ${HELPER}/ASTNode.o \
		  ${HELPER}/translate.o \
		  ${HELPER}/populateMainSouffle.o \
		  ${HELPER}/populateRelSouffle.o \
		  ${HELPER}/populateVersionSouffle.o \
		  ${HELPER}/symbolStore.o \
		  ${HELPER}/utils.o 
