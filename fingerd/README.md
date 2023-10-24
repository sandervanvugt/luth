# Trivial finger daemon

[![Build Status](https://travis-ci.org/ewxrjk/fingerd.svg?branch=master)](https://travis-ci.org/ewxrjk/fingerd)

This is a trivial finger server that reveals only very limited amounts
of information.

## Installation

If you got it from git:

    autoreconf -si

In any case:

    ./configure
    make
    sudo make install

At this point you should be able to do `man trivial.fingerd`.

## inetd configuration (Linux etc)

Add the following line to your `/etc/inetd.conf`:

    finger stream tcp nowait nobody.nogroup /usr/local/sbin/trivial.fingerd /usr/local/sbin/trivial.fingerd

Then restart or reload inetd.

## systemd configuration

TODO

## launchd configuration (OS X)

`make install` will install a suitable plist into
`/Library/LaunchDaemons`.  To enable it you must use `launchctl`:

    $ sudo launchctl load /Library/LaunchDaemons/uk.org.greenend.fingerd.plist

To disable it do the same but with `unload`:

    $ sudo launchctl unload /Library/LaunchDaemons/uk.org.greenend.fingerd.plist

`make uninstall` will also perform this step.

# Administrivia

Please report bugs to Richard Kettlewell <rjk@greenend.org.uk>.

Copyright (C) 2010 Richard Kettlewell

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
