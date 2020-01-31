#!/bin/bash
cat $2 | tr `cat $1 | head -1 ` a-z | tr `cat $1 | tail -1 ` A-Z
