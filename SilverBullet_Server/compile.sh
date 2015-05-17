#!/bin/bash

BIN_DIR="./bin"
SRC_DIR="./"
OBJ_DIR="./obj"

rm $BIN_DIR/*
rm $OBJ_DIR/*

g++ -std=c++11 -I. -c Chameleon.cc  -o $OBJ_DIR/Chameleon.obj
g++ -std=c++11 -I. -c ConfigFile.cc -o $OBJ_DIR/ConfigFile.obj

g++ -std=c++11 -I. -c SilverBulletServer.cc -o $OBJ_DIR/SilverBulletServer.obj

g++ -std=c++11 -I. -c ClientThread.cc -o $OBJ_DIR/ClientThread.obj

g++ -std=c++11 -I. -c KeyMaintainer.cc -o $OBJ_DIR/KeyMaintainer.obj

g++ -std=c++11 -I. -c main.cc -o $OBJ_DIR/main.obj

g++ -std=c++11 -pthread -o $BIN_DIR/SilverBulletServer $OBJ_DIR/main.obj $OBJ_DIR/SilverBulletServer.obj $OBJ_DIR/KeyMaintainer.obj $OBJ_DIR/Chameleon.obj $OBJ_DIR/ConfigFile.obj $OBJ_DIR/ClientThread.obj 

