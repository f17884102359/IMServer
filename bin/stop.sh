#!/bin/sh
#停止IMServer
kill -9 `ps -ef | grep server.app | grep -v grep | awk '{print $2}'`