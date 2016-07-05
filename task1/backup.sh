#!/bin/bash

CFG="backup.blacklist"

SRC=$1
DST=$2
AR=$3

if [ ! -e "$SRC" ]
then
    echo "Invalid argument."
    exit 1
fi

if [ ! -e "$DST" ]
then
    echo "Invalid argument."
    exit 1
fi

DST=`realpath $DST`
SRC=`realpath $SRC`

case "$AR" in
    "")
        AR=none
;;
    "tar")
        AR=tar
;;
esac

if [ -z "$AR" ]
then
    echo "Invalid argument."
    exit 1
fi

PWD_O=$PWD

cd "$SRC"

ID=`date +%H%M%S`

FILES=

for file in `ls`
do
    BLACKED=`cat $CFG | grep \^$file\$`
    if [ -z "$BLACKED" ]
    then
        FILES="$file $FILES"
    fi
done

BKPFILE="$DST/bkp$ID"

case "$AR" in
    "tar")
        tar --create --recursion --listed-incremental=tar.inc --file=$BKPFILE.tar $FILES
        ;;

    "none")
        mkdir $BKPFILE
        
        NEWFILES=
        
        if [ -e cp.inc ]
        then
            for file in $FILES
            do
                CKSUM=`cksum $file`
                CKSUM=( $CKSUM )

                CKOLD=`cat cp.inc | grep $file`
                CKOLD=( $CKOLD )

                if [ ! ${CKSUM[0]} = ${CKOLD[0]} ]
                then
                    NEWFILES="$file $NEWFILES"
                fi
            done
        else
            NEWFILES=$FILES
        fi
        
        if [ ! $NEWFILES = "" ]
        then
            cp -r $NEWFILES $BKPFILE
        fi

        cksum $FILES > cp.inc
        ;;
esac

cd "$PWD_O"


