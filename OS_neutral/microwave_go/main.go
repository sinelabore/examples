package main

import (
	"fmt"
	"log"

	term "github.com/nsf/termbox-go"
)

func debugPrint(msg string) {
	log.Print(msg)
}

func main() {
	err := term.Init()
	if err != nil {
		panic(err)
	}
	defer term.Close()

	fmt.Println(" ===== Microwave oven simulation")
	fmt.Println(" ===== Use the following keys:")
	fmt.Println(" ===== o,O: Open the door")
	fmt.Println(" ===== c,C: Close the door")
	fmt.Println(" ===== + increase cooking time")
	fmt.Println(" ===== - decrese cooking time")
	fmt.Println(" ===== q,Q: quit")
	fmt.Println(" ===== ")
	fmt.Println(" ===== Example sequence: +++++c   o   c +++ c")
	fmt.Println(" ===== ")

	ch := make(chan OvenEvent, 10)
	quit := make(chan bool)

	sm := NewReactor()
	sm.Init()

	go sm.React(ch, quit)

	/*
		events := []OvenEvent{EvInc, EvInc, EvInc, EvInc, EvInc, EvInc, EvInc, EvDoorClosed}
		for _, event := range events {
			ch <- event
		}

		time.Sleep(2 * time.Second)
		ch <- EvDoorOpen
		time.Sleep(1 * time.Second)
		ch <- EvDoorClosed

		time.Sleep(10 * time.Second)
		quit <- true
		fmt.Println("\nDone!")
	*/

	for {
		switch ev := term.PollEvent(); ev.Type {
		case term.EventKey:
			switch ev.Key {
			default:
				//term.Sync()
				if ev.Ch == 'q' || ev.Ch == 'Q' {
					return
				} else if ev.Ch == '+' {
					ch <- EvInc
				} else if ev.Ch == '-' {
					ch <- EvDec
				} else if ev.Ch == 'c' || ev.Ch == 'C' {
					ch <- EvDoorClosed
				} else if ev.Ch == 'o' || ev.Ch == 'O' {
					ch <- EvDoorOpen
				}
			}
		}
	}
}
