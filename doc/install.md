# Installation instructions

If you want to see how to use the library look [here](dc/dc4/md_doc_usage.html)

## Arduino Ide

The library is in the Arduino library manager.

1. Open your [Arduino](http://arduino.cc) IDE.
2. Click on Tools, Manage Libraries..
3. Type `LedController` in the search bar.
4. Select the version you want and click install.
5. That was it you are ready to use the library.

## PlatformIO

The [library](https://platformio.org/lib/show/7144/LedController) is also available for the [platform.io](https://platformio.org/platformio) library manager.
**Since they get their information from the Arduino Library Manager and the git repository, their version might not be up to date.**

### CLI

Just type `pio lib install "LedController"` or `python -m platformio lib install "LedController"` to install the library.

### Visual Studio Code extention

Either open a command line and use the CLI instructions or do the following steps:

1. Open [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [platform.io extention](https://platformio.org/platformio-ide) and wait till it is installed and configured
3. Open the PlatformIO home (by clicking on the little house symbol)
4. Click on the libraries tab.
5. Type `LedController` in the search bar and click on the result named `LedController`
6. Select the Version you want to install and click on install.
