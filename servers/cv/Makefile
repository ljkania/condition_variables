PROG=	cv
SRCS=	main.c mutex.c cv.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

MAN=

BINDIR?= /usr/sbin
FILES=cv.conf
FILESNAME=cv
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>
