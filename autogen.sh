#!/bin/sh
#gtkdocize || exit 1
autoreconf -i -f || exit 1
exec ./configure "$@"