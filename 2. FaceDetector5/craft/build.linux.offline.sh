#!/usr/bin/env bash

cmake .. -DSEETA_LOCK_MODEL=ON -DSEETA_LOCK_KEY=$1 -DSEETA_LOCK_FUNCID=1001

make -j16

