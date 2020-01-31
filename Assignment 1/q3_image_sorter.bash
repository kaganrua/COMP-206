#!/bin/bash
ls --full-time  -ltRr $1 | grep .jpg | sort -k8 | cut -d' ' -f9 > file.txt 
tr '\n' ' ' < file.txt > file2.txt
for i in `cat file2.txt`
do
find . -type f -name "$i"
done > file3.txt
tr '\n' ' ' < file3.txt > file4.txt
convert -append `cat file4.txt` `echo $1 | tr ‘/‘ ‘_’ `.jpg
rm file.txt
rm file2.txt
rm file3.txt
rm file4.txt
