#! /bin/bash

echo "Varying the number of threads..."

echo './memcpy-bandwidth.mic.exe -i 50 -t 1' ; ./memcpy-bandwidth.mic.exe -i 50 -t 1
echo './memcpy-bandwidth.mic.exe -i 50 -t 2' ; ./memcpy-bandwidth.mic.exe -i 50 -t 2
echo './memcpy-bandwidth.mic.exe -i 50 -t 4' ; ./memcpy-bandwidth.mic.exe -i 50 -t 4
echo './memcpy-bandwidth.mic.exe -i 50 -t 8' ; ./memcpy-bandwidth.mic.exe -i 50 -t 8
echo './memcpy-bandwidth.mic.exe -i 50 -t 16' ; ./memcpy-bandwidth.mic.exe -i 50 -t 16
echo './memcpy-bandwidth.mic.exe -i 50 -t 32' ; ./memcpy-bandwidth.mic.exe -i 50 -t 32
echo './memcpy-bandwidth.mic.exe -i 50 -t 64' ; ./memcpy-bandwidth.mic.exe -i 50 -t 64
echo './memcpy-bandwidth.mic.exe -i 50 -t 128' ; ./memcpy-bandwidth.mic.exe -i 50 -t 128
echo './memcpy-bandwidth.mic.exe -i 50 -t 228' ; ./memcpy-bandwidth.mic.exe -i 50 -t 228
echo './memcpy-bandwidth.mic.exe -i 50 -t 256' ; ./memcpy-bandwidth.mic.exe -i 50 -t 256
echo './memcpy-bandwidth.mic.exe -i 50 -t 512' ; ./memcpy-bandwidth.mic.exe -i 50 -t 512
