#!/bin/bash

if [ -z $1 ]
then
    echo "Suffix required!"
    exit
fi
SUFFIX=$1

PRUN="prun --optGridOutputSampleName=user.%nickname%.%in:name[1]%.%in:name[2]%.%in:name[3]%.${SUFFIX}"

if [ ! -z $2 ]
then
    DESTSITE=$2
    echo "Submitting to ${DESTSITE}"
    PRUN="${PRUN} --optGridDestSE=${DESTSITE}"
fi

# data15
./xAODAnaHelpers/scripts/xAH_run.py --config DSMP/scripts/config_DAODtoMT.py --files DSMP/etc/data15.EXOT8.list --inputList --inputDQ2 --submitDir ${SUFFIX}_data15 --force ${PRUN}

# data16
./xAODAnaHelpers/scripts/xAH_run.py --config DSMP/scripts/config_DAODtoMT.py --files DSMP/etc/data16.EXOT8.list --inputList --inputDQ2 --submitDir ${SUFFIX}_data16 --force ${PRUN}

# jetjet
./xAODAnaHelpers/scripts/xAH_run.py --config DSMP/scripts/config_DAODtoMT.py --files DSMP/etc/jetjet.EXOT8.list --inputList --inputDQ2 --submitDir ${SUFFIX}_jetjet --force --isMC ${PRUN}
