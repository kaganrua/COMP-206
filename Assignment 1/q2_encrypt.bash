#!/bin/bash
cat $2 | tr a-z `cat $1 | head -1 ` | tr A-Z `cat $1 | tail -1 `
