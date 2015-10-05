# -*- makefile -*-
# configuration for Fedora Linux with distribution provided Torque RPMs.
CC=gcc
CPPFLAGS=
ARCHFLAGS= -g
GENFLAGS= 
OPTFLAGS=  -O
WARNFLAGS= -Wall

LD=$(CC)
LDFLAGS= 
LDLIBS= -ltorque
