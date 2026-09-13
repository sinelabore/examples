/*
 * Drives the Oven by hand.
 *
 *   + / -   one wheel tick up / down   -> Wheel::doInc / Wheel::doDec
 *   c / o   close / open the door      -> DoorContact::contact
 *   d       debug trace on / off
 *   q       quit
 *
 * Nothing here carries a value from one part to another - the model's own 'connect'
 * statements do that, in Oven::init(). This file only schedules and watches.
 *
 * Reading an output: a Port keeps no value, it hands what is sent to whoever is
 * connected and forgets it. addReceiver() adds one more listener beside the ones
 * connect() made, so watching a port here does not disturb the wiring. Each cycle
 * starts by clearing what was seen, so the table shows what was sent in *this*
 * cycle and a '.' where nothing was.
 *
 * One cycle is one tick of the countdown, not one second.
 */
#include "system.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

using namespace MicrowaveOfen;

namespace {

/**
 * Terminal in raw mode while this object lives, so keys arrive unbuffered and unechoed.
 *
 * On POSIX, stdin is also marked non-blocking: VMIN/VTIME only govern a terminal, and
 * without it a scan would stall in read() whenever input came from a pipe rather than
 * a keyboard. On Windows the same contract is PeekConsoleInput / PeekNamedPipe: never
 * wait, restore the console mode on the way out, leave Ctrl+C alone.
 */
#ifdef _WIN32

struct RawTerminal {
    HANDLE handle = INVALID_HANDLE_VALUE;
    DWORD savedMode = 0;
    bool restore = false;

    RawTerminal() {
        handle = GetStdHandle(STD_INPUT_HANDLE);
        if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
            return;
        if (!GetConsoleMode(handle, &savedMode))
            return;  // pipe or file: nothing to put in raw mode
        DWORD raw = savedMode;
        raw &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
        restore = SetConsoleMode(handle, raw) != 0;
    }

    ~RawTerminal() {
        if (restore)
            SetConsoleMode(handle, savedMode);
    }
};

bool readKey(char& key) {
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in == INVALID_HANDLE_VALUE || in == nullptr)
        return false;

    DWORD mode = 0;
    if (GetConsoleMode(in, &mode)) {
        INPUT_RECORD rec{};
        DWORD n = 0;
        while (PeekConsoleInput(in, &rec, 1, &n) && n == 1) {
            if (!ReadConsoleInput(in, &rec, 1, &n) || n != 1)
                return false;
            if (rec.EventType != KEY_EVENT)
                continue;
            const KEY_EVENT_RECORD& kev = rec.Event.KeyEvent;
            if (!kev.bKeyDown)
                continue;
            const char ch = static_cast<char>(kev.uChar.AsciiChar);
            if (ch == '\0')
                continue;  // modifiers, arrows, function keys
            key = ch;
            return true;
        }
        return false;
    }

    DWORD avail = 0;
    if (!PeekNamedPipe(in, nullptr, 0, nullptr, &avail, nullptr) || avail == 0)
        return false;
    DWORD got = 0;
    return ReadFile(in, &key, 1, &got, nullptr) && got == 1;
}

#else

struct RawTerminal {
    termios saved{};
    int savedFlags = 0;
    RawTerminal() {
        tcgetattr(STDIN_FILENO, &saved);
        termios raw = saved;
        raw.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);
        raw.c_cc[VMIN] = 0;   // read() returns at once, empty-handed if nothing was typed
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);

        savedFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, savedFlags | O_NONBLOCK);
    }
    ~RawTerminal() {
        fcntl(STDIN_FILENO, F_SETFL, savedFlags);
        tcsetattr(STDIN_FILENO, TCSANOW, &saved);
    }
};

bool readKey(char& key) {
    return ::read(STDIN_FILENO, &key, 1) == 1;
}

#endif

const char* nameOf(Commands command) {
    switch (command) {
        case Commands::ON:    return "ON";
        case Commands::OFF:   return "OFF";
        case Commands::PAUSE: return "PAUSE";
        default:              return ".";
    }
}

const char* nameOf(WheelCmd tick) {
    switch (tick) {
        case WheelCmd::INCREMENT: return "INCREMENT";
        case WheelCmd::DECREMENT: return "DECREMENT";
        default:                  return ".";
    }
}

const char* stateOf(TimerDisplay& display) {
    if (display.stateMachine.isInZERO())      return "ZERO";
    if (display.stateMachine.isInPAUSE())     return "PAUSE";
    if (display.stateMachine.isInCOUNTDOWN()) return "COUNTDOWN";
    return "-";
}

}  // namespace

int main() {

    Oven oven;
    oven.setInstanceName("oven");
    oven.init();

    // system.h is generated with -d, so every action and accept reports itself. Off
    // until 'd' asks for it, or the trace buries the table.
    TraceSink debugSink = traceSink();
    traceSink() = nullptr;

    Commands doorOut = Commands::None;
    Commands timeOut = Commands::None;
    WheelCmd wheelOut = WheelCmd::None;

    // The three appliances are levels, not events: the Controller commands one only when
    // the timer tells it to, and a lamp stays lit until told otherwise. So these keep the
    // last command sent rather than being cleared at the top of each cycle.
    Commands lampCmd = Commands::None;
    Commands motorCmd = Commands::None;
    Commands magnetronCmd = Commands::None;

    oven.doorContact->doorSignal.addReceiver([&doorOut](const CommandPort& sent) {
        doorOut = std::get<CommandPortDef>(sent).cmd.cmd;
    });
    oven.timerDisplay->timePort.addReceiver([&timeOut](const CommandPort& sent) {
        timeOut = std::get<CommandPortDef>(sent).cmd.cmd;
    });
    oven.wheel->wheel.addReceiver([&wheelOut](const WheelPort& sent) {
        wheelOut = std::get<WheelPortDef>(sent).cmd.tick;
    });
    oven.controller->cmdPortToLamp.addReceiver([&lampCmd](const CommandPort& sent) {
        lampCmd = std::get<CommandPortDef>(sent).cmd.cmd;
    });
    oven.controller->cmdPortToMotor.addReceiver([&motorCmd](const CommandPort& sent) {
        motorCmd = std::get<CommandPortDef>(sent).cmd.cmd;
    });
    oven.controller->cmdPortToMagnetron.addReceiver([&magnetronCmd](const CommandPort& sent) {
        magnetronCmd = std::get<CommandPortDef>(sent).cmd.cmd;
    });

    RawTerminal rawTerminal;

    std::cout << "+/- wheel   c/o door   d trace   q quit\r\n"
              << "cycle | wheel.wheel  doorSignal  timePort | ticks  state     "
              << "|  lamp  motor  magnetron\r\n";

    std::cout << "Nominal Power of Device: " << oven.nominalPower << " Watt" << std::endl;
    
    for (int cycle = 0;; cycle++) {

        // nothing has been sent yet in this cycle
        doorOut = Commands::None;
        timeOut = Commands::None;
        wheelOut = WheelCmd::None;

        char key = 0;
        while (readKey(key)) {
            switch (key) {
                case '+': oven.wheel->doInc(*oven.wheel); break;
                case '-': oven.wheel->doDec(*oven.wheel); break;
                case 'c': oven.doorContact->contact = true; break;
                case 'o': oven.doorContact->contact = false; break;
                case 'd': {
                    bool tracing = static_cast<bool>(traceSink());
                    traceSink() = tracing ? TraceSink{} : debugSink;
                    break;
                }
                case 'q': return 0;
                default: break;
            }
        }

        // One scan. process() walks the containment tree and steps each part's state
        // machine, and each machine runs its states' 'do' actions - DoorContact publishes
        // the contact, TimerDisplay reads the wheel and counts down. Nothing else to call.    
        oven.process();

        std::cout << std::setw(5) << cycle << " | "
                  << std::setw(11) << nameOf(wheelOut) << "  "
                  << std::setw(10) << nameOf(doorOut) << "  "
                  << std::setw(8) << nameOf(timeOut) << " | "
                  << std::setw(5) << oven.timerDisplay->adjustedTicks << "  "
                  << std::left << std::setw(10) << stateOf(*oven.timerDisplay) << std::right
                  << "| " << std::setw(5) << nameOf(lampCmd)
                  << std::setw(7) << nameOf(motorCmd)
                  << std::setw(11) << nameOf(magnetronCmd)
                  << "\r\n" << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
