# AmigaOS cross-development on Linux

This folder contains instructions on setting up C cross-development in Linux for AmigaOS and sample source-code + m68k binaries.

You will need (open-source version of) LHA to uncompress the Amiga-specific archives (*.lha).

## AmigaOS - C cross-development requirements

The following packages are required to set up a cross-compiler environment for AmigaOS on Linux.

We are using VBCC for cross-development/-compilation for AmigaOS, performed on Linux.
http://sun.hasenbraten.de/vbcc/

For a step-by-step tutorial on how to set this all up, check out this YouTube-video by Wei-ju Wu:
[Cross Development for the Amiga with VBCC](https://www.youtube.com/watch?v=vFV0oEyY92I&ab_channel=Wei-juWu)

`wget` resource list:
```
### Cross-compiler
wget http://server.owl.de/~frank/tags/vbcc0_9e.tar.gz

### Compile targets (m68k/amigaos)
wget http://server.owl.de/~frank/vbcc/2014-12-30/vbcc_target_m68k_amigaos.lha
wget http://server.owl.de/~frank/vbcc/2015-06-04/vbcc_target_m68k-amigaos-v4.lha
wget http://server.owl.de/~frank/vbcc/2015-11-22/vbcc_target_m68k-amigaos_P2.lha

### VASM
wget http://sun.hasenbraten.de/vasm/release/vasm.tar.gz

### VLINK
wget http://sun.hasenbraten.de/vlink/release/vlink.tar.gz

### NDK 3.9
wget http://www.haage-partner.de/download/AmigaOS/NDK39.lha
```

## amigacc

Wrapper-script for compiling AmigaOS binaries with VBCC.

## Environment variables

Add this snippet to the end of your `.bashrc` file.
```
# AmigaOS - C cross-development env
export VBCC=~/amiga_sdk/vbcc
export PATH=$VBCC/bin/:$PATH
export NDK_INC=~/amiga_sdk/NDK_3.9/Include/include_h
```
