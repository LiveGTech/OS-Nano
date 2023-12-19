# LiveG OS Nano
The edition of the LiveG OS family that runs on low-power, embedded devices.

Licensed by the [LiveG Open-Source Licence](LICENCE.md).

## Prerequisites
Before building LiveG OS Nano, you'll need to install the PlatformIO SDK and a few required libraries for simulation. These can all be installed by running the following:

```bash
./installdev.sh
```

## Building
To build LiveG OS Nano and upload the firmware to hardware, run the following:

```bash
./build.sh --upload
```

### Simulating
To simulate LiveG OS Nano, run the following to build a WebAssembly (.wasm) file:

```bash
./build.sh --sim
```

Then start a web server to host the simulator:

```bash
python3 -m http.server
```

You can then visit https://localhost:8000/simulator to run the simulator.