> [!WARNING]
> To run this project, CMake, Python needs to be installed <br>

## Setup
```bash
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
install.bat / install.sh
```
In main project folder, create a ESP shell env in terminal
```bash
export.bat / export.sh
```
Set target
```bash
idf.py set-target <target>
```
Build
```bash
idf.py build
```
Run project
```bash
idf.py -p PORT flash
```
Show data
```bash
idf.py monitor
```

## Dependencies
- [esp-idf](https://github.com/espressif/esp-idf.git)
- [hd44780](https://github.com/esp-idf-lib/hd44780)
