#!/bin/bash

mic_num=${1:-0}

make mic && scp mic-prefix-offset.exe xeonphi@mic$mic_num:~/ && ssh xeonphi@mic$mic_num <<'ENDSSH'
  ./mic-prefix-offset.exe
ENDSSH
