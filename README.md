# rpiweatherd-qtclient
A lightweight Qt-based client for [rpiweatherd](http://github.com/ronen25/rpiweatherd).

![Screenshot](/res/rpiweatherd-qtclient-screen.png "Screenshot")

***

### Features
- Scalable display of the current temperature and humidity
- [HUMIDEX](https://www.ccohs.ca/oshanswers/phys_agents/humidex.html) calculation
- [Heat Index](http://www.srh.noaa.gov/ama/?n=heatindex) calculation
- A sky-like dynamic background effect (see screenshot and notes)
- Highly scalable display

### System Requirements
1. An already-installed [rpiweatherd](http://github.com/ronen25/rpiweatherd) server.
2. Ubuntu 14.04 or newer*/Windows 7
3. Qt 5.5 or newer (might work with older versions).

>\* Versions of Ubuntu older then 15.10 come with an old version of Qt. You will need to manually install
>Qt 5.5 if you are running these versions.
><br />Other Linux distributions will also probably work, but were not tested.

### Installation
Pre-built binaries for 64-bit Linux and 64-bit Windows are available from the releases page.<br />
If you do not have a 64-bit system, you will need to compile the program from source.

### Compile From Source
1) Clone repository:
```
    $ git clone https://github.com/ronen25/rpiweatherd-qtclient
```

2) Change directory and compile:
```
    $ cd rpiweatherd-qtclient
    $ qmake ./rpiweatherd-qtclient.pro
    $ make
```

3) Run the program:
```
    $ ./rpiweatherd-qtclient
```

> (Make sure you're compiling with a Qt5 <code>qmake</code>! Most linux distributions default
> to a Qt4-based qmake).

***

### License
Distributed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html) license.
