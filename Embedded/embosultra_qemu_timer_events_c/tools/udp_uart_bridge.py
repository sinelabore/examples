#!/usr/bin/env python3
"""Bridge embOSView's Ethernet interface to the target's embOSView UART in QEMU.

embOSView (Ethernet) sends UDP datagrams to 127.0.0.1:50021. QEMU exposes the
target's USART3 as a TCP port (-serial tcp:...:4444,server=on,wait=off). This
passes the bytes of each datagram to the serial port, and what the target sends
back to the last UDP sender, one datagram per burst (a burst ends after GAP
seconds of silence). embOSView's UART protocol goes through unchanged.

The serial side is connected again whenever it closes, e.g. while QEMU starts
or restarts; Ctrl-C ends the bridge.

    udp_uart_bridge.py [--udp 50021] [--tcp 4444] [-v]
"""
import argparse
import selectors
import socket
import time

GAP = 0.005


def connect(port):
    """The serial port's TCP connection, retried until QEMU accepts it."""
    while True:
        try:
            s = socket.create_connection(("127.0.0.1", port), timeout=2)
            s.setblocking(False)
            return s
        except OSError:
            time.sleep(0.5)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--udp", type=int, default=50021, help="embOSView's UDP port")
    ap.add_argument("--tcp", type=int, default=4444, help="QEMU's TCP port for the target's USART3")
    ap.add_argument("-v", "--verbose", action="store_true", help="print every packet in hex")
    a = ap.parse_args()

    udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp.bind(("127.0.0.1", a.udp))
    print(f"bridge: embOSView UDP 127.0.0.1:{a.udp} <-> QEMU serial 127.0.0.1:{a.tcp}", flush=True)

    sel = selectors.DefaultSelector()
    sel.register(udp, selectors.EVENT_READ, "udp")
    tcp = connect(a.tcp)
    sel.register(tcp, selectors.EVENT_READ, "tcp")
    peer, pending, last = None, b"", 0.0
    while True:
        timeout = GAP if pending else None
        for key, _ in sel.select(timeout):
            if key.data == "udp":
                data, peer = udp.recvfrom(65536)
                if a.verbose:
                    print("view>", data.hex(" "), flush=True)
                try:
                    tcp.sendall(data)
                except OSError:
                    pass                # the serial side is gone; reconnected below
            else:
                try:
                    data = tcp.recv(65536)
                except BlockingIOError:
                    continue
                except OSError:
                    data = b""
                if not data:            # QEMU closed the port, or is not up yet
                    sel.unregister(tcp)
                    tcp.close()
                    time.sleep(0.5)
                    tcp = connect(a.tcp)
                    sel.register(tcp, selectors.EVENT_READ, "tcp")
                    continue
                pending += data
                last = time.monotonic()
        if pending and time.monotonic() - last >= GAP:
            if a.verbose:
                print("<targ", pending.hex(" "), flush=True)
            if peer:
                udp.sendto(pending, peer)
            pending = b""


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
