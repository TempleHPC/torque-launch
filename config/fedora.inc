# -*- makefile -*-
# configuration for Fedora Linux with distribution provided Torque RPMs.
CC=gcc
CPPFLAGS= -I/usr/include/torque
ARCHFLAGS= -g
GENFLAGS= 
OPTFLAGS=  -O
WARNFLAGS= -Wall

LD=$(CC)
LDFLAGS= 
LDLIBS= -ltorque
