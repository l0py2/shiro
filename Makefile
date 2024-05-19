SRC := main.c
OBJ := ${SRC:.c=.o}
OUT := shiro.cgi
INSTALL_OUT := /srv/http/cgi-bin
CFLAGS := -Wall
LDLIBS := `pkg-config --libs sqlite3`

all: ${OUT}

clean:
	@rm -f ${OUT} ${OBJ}

install:
	@cp -f ${OUT} ${INSTALL_OUT}

uninstall:
	@rm -f ${INSTALL_OUT}

${OBJ}:

${OUT}: ${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^
