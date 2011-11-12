#!/bin/bash
TOOLS=`dirname $0`
VENV=$TOOLS/../.pyblga-venv
source $VENV/bin/activate && $@
