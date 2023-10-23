To be able to generate the code you moust have installed the VxWorks built tools on your computer

source ~/vxworks/wrsdk-vxworks7-qemu/sdkenv.sh
make

As a result you will get a file called: example.vxe

Start an ftp server to be able to load the programm into Qemu

    python3 -m pyftpdlib -p 21 -u target -P vxTarget -d ~/vxworks/wrsdk-vxworks7-qemu/examples/sinelabore/vxworks

The output should look like similar to this:

    [I 2023-10-23 11:21:11] concurrency model: async
    [I 2023-10-23 11:21:11] masquerade (NAT) address: None
    [I 2023-10-23 11:21:11] passive ports: None
    [I 2023-10-23 11:21:11] >>> starting FTP server on 0.0.0.0:21, pid=3024 <<<

Start the qemu in another terminal:

    cd ~/vxworks/wrsdk-vxworks7-qemu
    ./run-qemu.sh

After a moment you should see the output:

      Instantiating /ram0 as rawFs, device = 0x1
      Instantiating /ram0 as rawFs,  device = 0x1
      Formatting /ram0 for HRFS v1.2
      Formatting...OK.
      Target Name: vxTarget 
      Instantiating /tmp as rawFs, device = 0x10001
      Instantiating /tmp as rawFs,  device = 0x10001
      Formatting /tmp for HRFS v1.2
      Formatting...OK.
 
      _________            _________
      \........\          /......../
        \........\        /......../
        \........\      /......../
          \........\    /......../
          \........\   \......./
            \........\   \...../              VxWorks SMP 64-bit
            \........\   \.../
              \........\   \./     Release version: 22.09
              \........\   -      Build date: Dec 12 2022 14:41:41
                \........\
                \......./         Copyright Wind River Systems, Inc.
                  \...../   -                 1984-2022
                  \.../   /.\
                    \./   /...\
                    -   -------

                        Board: x86_64 Processor (ACPI_BOOT_OP) SMP/SMT
                    CPU Count: 1
                OS Memory Size: ~446MB
              ED&R Policy Mode: Deployed
          Debug Agent: Started (always)
              Stop Mode Agent: Not started


      Adding 24485 symbols for standalone.
      ->

Then type: cmd and start example.vxe

    [vxWorks *]#example.vxe
    Launching process 'example.vxe' ...
    Process 'example.vxe' (process Id = 0xffff800000312010) launched.
    Enter STOP
    In STOP
    task 1 started!
    task 2 started!
    In STOP
    Exit STOP
    Start timer!
    Enter RUN
    Enter Slow
    init finished!
    In RUN
    Exit Slow
    Enter Fast
    In RUN
    ...
    Exit Slow
    Exit RUN
    Stop timer!
    Enter STOP
    In STOP
    Exit STOP
    Entering Final
