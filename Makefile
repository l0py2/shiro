SRC := main.c cgiutils.c input.c elements.c
HEADERS := cgiutils.h input.h elements.h
OBJ := ${SRC:.c=.o}
OUT := shiro.cgi
TEMPLATES := templates
INSTALL_OUT := /srv/http/cgi-bin
CFLAGS := -Wall -std=c17
LDLIBS := `pkg-config --libs sqlite3`

all: ${OUT}

clean:
	@rm -f ${OUT} ${OBJ}

install:
	@cp -f ${OUT} ${INSTALL_OUT}
	@cp -rfT ${TEMPLATES} ${INSTALL_OUT}/${TEMPLATES}

uninstall:
	@rm -rf ${INSTALL_OUT}/${OUT} ${INSTALL_OUT}/${TEMPLATES}

${OBJ}: ${HEADERS}

${OUT}: ${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^
