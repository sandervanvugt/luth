# Copyright (C) 2010 Richard Kettlewell
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
sbin_PROGRAMS=trivial.fingerd
trivial_fingerd_SOURCES=fingerd.c
dist_man_MANS=trivial.fingerd.8
EXTRA_DIST=autogen.sh fingerd.plist.in

# launchd config for OS X systems
# http://developer.apple.com/macosx/launchd.html
all: fingerd.plist

fingerd.plist: fingerd.plist.in Makefile
	sed < ${srcdir}/fingerd.plist.in > fingerd.plist \
		's,_sbindir_,${sbindir},g'

install-data-local:
	@if [ -w /Library/LaunchDaemons ]; then \
	  echo ${INSTALL} -m 644 fingerd.plist /Library/LaunchDaemons/uk.org.greenend.fingerd.plist ;\
	  ${INSTALL} -m 644 fingerd.plist /Library/LaunchDaemons/uk.org.greenend.fingerd.plist ;\
	fi

uninstall-local:
	@if [ -w /Library/LaunchDaemons ]; then \
	  echo launchctl unload /Library/LaunchDaemons/uk.org.greenend.fingerd.plist ;\
	  launchctl unload /Library/LaunchDaemons/uk.org.greenend.fingerd.plist || true ;\
	  echo rm -f /Library/LaunchDaemons/uk.org.greenend.fingerd.plist ;\
	  rm -f /Library/LaunchDaemons/uk.org.greenend.fingerd.plist ;\
	fi

clean-local:
	rm -f fingerd.plist
