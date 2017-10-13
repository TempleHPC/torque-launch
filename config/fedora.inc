# -*- makefile -*-
# configuration for Fedora Linux with distribution provided Torque RPMs.
CC=gcc
CPPFLAGS= -I/usr/include/torque -DUSE_SYSLOG=1
ARCHFLAGS= -g
GENFLAGS= 
OPTFLAGS=  -O
WARNFLAGS= -Wall

LD=$(CC)
LDFLAGS= 
LDLIBS= -ltorque
