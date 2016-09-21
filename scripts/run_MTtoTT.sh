#!/bin/bash

if [ -z $1 ]
then
    echo "Output dir required!"
    exit
fi

OUTDIR=$1
DATAPATH="eos/atlas/unpledged/group-wisc/users/lkaplan/DSMP/prod_DSMP_20160915"

if [ -d $OUTDIR ]
then
    echo "Output dir already exists!"
    exit
fi

mkdir $OUTDIR

# data_15
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/data15.root  --treeName tree --submitDir ${OUTDIR}/TT_data15  --force        direct

# data_16
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/data16.root  --treeName tree --submitDir ${OUTDIR}/TT_data16  --force        direct

# jetjet
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ0W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ0W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ1W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ1W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ2W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ2W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ3W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ3W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ4W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ4W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ5W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ5W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ6W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ6W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ7W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ7W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ8W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ8W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ9W.root    --treeName tree --submitDir ${OUTDIR}/TT_JZ9W    --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ10W.root   --treeName tree --submitDir ${OUTDIR}/TT_JZ10W   --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ11W.root   --treeName tree --submitDir ${OUTDIR}/TT_JZ11W   --force --isMC direct
xAH_run.py --config DSMP/scripts/config_MTtoTT.py --files ${DATAPATH}/JZ12W.root   --treeName tree --submitDir ${OUTDIR}/TT_JZ12W   --force --isMC direct
