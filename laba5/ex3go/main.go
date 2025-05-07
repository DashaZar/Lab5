package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Command int

const (
	CmdCreateTram Command = iota
	CmdTramsInStop
	CmdStopsInTram
	CmdTrams
	CmdExit
	CmdUnknown
)

func parseCommand(input string) Command {
	switch strings.ToUpper(input) {
	case "CREATE_TRAM":
		return CmdCreateTram
	case "TRAMS_IN_STOP":
		return CmdTramsInStop
	case "STOPS_IN_TRAM":
		return CmdStopsInTram
	case "TRAMS":
		return CmdTrams
	case "EXIT":
		return CmdExit
	default:
		return CmdUnknown
	}
}

func printHelp() {
	fmt.Println(`Available commands:
CREATE_TRAM <name> <stop1> <stop2> ...
TRAMS_IN_STOP <stop>
STOPS_IN_TRAM <tram>
TRAMS
EXIT`)
}

func main() {
	manager := NewTramManager()
	reader := bufio.NewReader(os.Stdin)

	fmt.Println("Tram Management System (type 'exit' to quit)")

	for {
		fmt.Print("> ")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		if input == "" {
			continue
		}

		parts := strings.Fields(input)
		if len(parts) == 0 {
			continue
		}

		cmd := parseCommand(parts[0])
		if cmd == CmdExit {
			break
		}

		switch cmd {
		case CmdCreateTram:
			if len(parts) < 3 {
				fmt.Println("ERROR: Missing arguments")
				continue
			}
			err := manager.CreateTram(parts[1], parts[2:])
			if err != nil {
				fmt.Println(err)
			}

		case CmdTramsInStop:
			if len(parts) < 2 {
				fmt.Println("ERROR: Missing stop name")
				continue
			}
			trams := manager.GetTramsInStop(parts[1])
			if len(trams) == 0 {
				fmt.Printf("No trams in stop %s\n", parts[1])
			} else {
				fmt.Printf("Trams in stop %s: %s\n", parts[1], strings.Join(trams, " "))
			}

		case CmdStopsInTram:
			if len(parts) < 2 {
				fmt.Println("ERROR: Missing tram name")
				continue
			}
			stops := manager.GetStopsInTram(parts[1])
			if stops == nil {
				fmt.Printf("Tram %s not found\n", parts[1])
			} else {
				fmt.Printf("Stops for tram %s:\n", parts[1])
				for stop, trams := range stops {
					if len(trams) == 0 {
						fmt.Printf(" - %s: no other trams\n", stop)
					} else {
						fmt.Printf(" - %s: %s\n", stop, strings.Join(trams, " "))
					}
				}
			}

		case CmdTrams:
			trams := manager.GetAllTrams()
			if len(trams) == 0 {
				fmt.Println("No trams registered")
			} else {
				fmt.Println("All trams:")
				for tram, stops := range trams {
					fmt.Printf(" - %s: %s\n", tram, strings.Join(stops, " "))
				}
			}

		case CmdUnknown:
			fmt.Println("ERROR: Unknown command")
			printHelp()
		}
	}
}
