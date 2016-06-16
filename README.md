<p align="center">
    <img src="media/tritium-logo.png" alt="Tritium Logo">
</p>

# tritium [![Build Status](https://travis-ci.org/syb0rg/tritium.svg?branch=master)](https://travis-ci.org/syb0rg/tritium)

A free, premium quality speech synthesis engine written completely in C.  tritium was developed with the mindset of being portable, small, and fast.

---

### Dependencies

This is a list of the required libraries Khronos needs in order to compile properly.

- CMake (version 2.8.7 minimum): a portable Makefile generator.

- PortAudio: a portable audio I/O library.

If CMake is installed and run, it will handle the downloading of all of the other dependencies (if they are not already installed on the system).

---

### Configuration & Installation

Download and extract the compressed file.  Switch into the `build/` directory (it should be empty). If it does not exist then create one. This will be where the final build will be housed through the building process.

Run `cmake ..` and the configuation should begin.  This will create a Makefile tailored for running in your specific environment.  Any dependencies that you do not have will also be noted and flagged for downloading.

Run `make`.  Any flagged dependencies will now begin downloading into their own special folder, where they will have to be configured and built so that tritium can link to their libraries.  Once everything has finished downloading and everything is linked together, the build should be complete.  To complete installation onto your system, make sure you have the privaleges and run `sudo make install`.
