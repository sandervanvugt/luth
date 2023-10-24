dnl Copyright (C) 2010,13 Richard Kettlewell
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.
AC_INIT([trivialfingerd], [0.0+], [rjk@greenend.org.uk])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([fingerd.c])
AM_CONFIG_HEADER([config.h])
AC_PROG_CC
if test "x$GCC" = xyes; then
  CC="${CC} -Wall -W -Wpointer-arith -Wbad-function-cast -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wshadow"
  CC="${CC} -Werror"
  CC="${CC} -std=gnu99"
fi
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
