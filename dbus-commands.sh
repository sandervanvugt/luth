#!/bin/bash
echo this script is not meant to be running as such, just copy individual commands to see what they're doing
exit 3

# show object paths for networkmanager
qdbus --system --literal org.freedesktop.NetworkManager

# will send boolean true or false to show if networking is enabled
dbus-send --system --print-reply \
            --dest=org.freedesktop.NetworkManager \
            /org/freedesktop/NetworkManager \
            org.freedesktop.DBus.Properties.Get \
            string:"org.freedesktop.NetworkManager" \
            string:"NetworkingEnabled"

# list all active connections
dbus-send --system --print-reply \
            --dest=org.freedesktop.NetworkManager \
            /org/freedesktop/NetworkManager \
            org.freedesktop.DBus.Properties.Get \
            string:"org.freedesktop.NetworkManager" \
            string:"ActiveConnections"

# which interface is the active connection using
# change number  to the connection number found above
dbus-send --system --print-reply \
            --dest=org.freedesktop.NetworkManager \
            /org/freedesktop/NetworkManager/ActiveConnection/6 \
            org.freedesktop.DBus.Properties.Get \
            string:"org.freedesktop.NetworkManager.Connection.Active" \
            string:"Devices"

# this will show the device number. Match the following to the device number found 
dbus-send --system --print-reply \
            --dest=org.freedesktop.NetworkManager \
            /org/freedesktop/NetworkManager/Devices/2 \
            org.freedesktop.DBus.Properties.Get \
            string:"org.freedesktop.NetworkManager.Device" \
            string:"Interface"

# notice that this approach allows you to change properties as well, but that requires
# authenticated connections
