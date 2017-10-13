# -*- makefile -*-
# configuration for Temple's Owl's Nest HPC cluster
CC=gcc
CPPFLAGS= -I/opt/torque/include -DUSE_SYSLOG=1
ARCHFLAGS= -g
GENFLAGS= 
OPTFLAGS=  -O
WARNFLAGS= -Wall

LD=$(CC)
LDFLAGS= -L/opt/torque/lib -Wl,-rpath,/opt/torque/lib
LDLIBS= -ltorque
