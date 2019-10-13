#!/bin/sh
# Add your startup script

# DO NOT DELETE
/etc/init.d/xinetd start;
while true; do rm -rf /tmp/sc; mkdir /tmp/sc; sleep 60; done;
