#!/bin/env bash

diff <(../src/config-manager get config.seca.key1) \
     <(cat <<EOF
value for oper
EOF
      )
