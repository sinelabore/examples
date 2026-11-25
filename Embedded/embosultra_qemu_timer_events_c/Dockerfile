# Build and run environment: the Arm GNU Toolchain the embOS-Ultra libraries
# for Cortex-M and GCC were built with (11.2-2022.02, named in
# Release_embOS_Ultra_CortexM_GCC.html of the embOS download), and QEMU.
FROM debian:trixie-slim
RUN apt-get update \
 && apt-get install -y --no-install-recommends qemu-system-arm make curl xz-utils ca-certificates \
 && rm -rf /var/lib/apt/lists/*
RUN arch=$(uname -m) \
 && curl -fsSL "https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-${arch}-arm-none-eabi.tar.xz" \
    | tar -xJ -C /opt \
 && ln -s /opt/gcc-arm-11.2-2022.02-${arch}-arm-none-eabi /opt/gcc-arm
ENV PATH=/opt/gcc-arm/bin:$PATH
