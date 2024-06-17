SRC := main.c cgiutils.c input.c elements.c handles.c
HEADERS := cgiutils.h input.h elements.h handles.h
OBJ := ${SRC:.c=.o}
OUT := shiro.cgi
TEMPLATES := templates
STYLES := styles
HTTP_SRV := /srv/http
CFLAGS := -Wall -std=c17
LDLIBS := `pkg-config --libs sqlite3`

all: ${OUT}

clean:
	@rm -f ${OUT} ${OBJ}

install:
	@cp -f ${OUT} ${HTTP_SRV}/${OUT}
	@cp -rfT ${TEMPLATES} ${HTTP_SRV}/${TEMPLATES}
	@cp -rfT ${STYLES} ${HTTP_SRV}/${STYLES}

uninstall:
	@rm -rf ${HTTP_SRV}/${OUT} ${HTTP_SRV}/${TEMPLATES} ${HTTP_SRV}/${STYLES}

${OBJ}: ${HEADERS}

${OUT}: ${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

.PHONY: all clean install uninstall
