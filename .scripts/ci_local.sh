#!/bin/bash

boards="-b esp32dev -b d1_mini -b uno -b ATmega1280 -b leonardo"
PLATFORMIO_CI_SRC="examples/LedControllerDemoRocket/LedControllerDemoRocket.ino" python3 -m platformio ci --lib="." ${boards} --keep-build-dir
PLATFORMIO_CI_SRC="examples/LedControllerDemoCounting/LedControllerDemoCounting.ino" python3 -m platformio ci --lib="." ${boards} --keep-build-dir
PLATFORMIO_CI_SRC="examples/LedControllerDemoHwSPI/LedControllerDemoHwSPI.ino" python3 -m platformio ci --lib="." ${boards} --keep-build-dir
PLATFORMIO_CI_SRC="examples/LCDemo7Segment/LCDemo7Segment.ino" python3 -m platformio ci --lib="." ${boards} --keep-build-dir
python3 -m platformio test -e native
