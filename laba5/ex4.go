package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strings"
)

type RegionDirectory struct {
	regions map[string]string // Хранение регионов и их центров
}

func NewRegionDirectory() *RegionDirectory {
	return &RegionDirectory{
		regions: make(map[string]string),
	}
}

func (rd *RegionDirectory) change(region, newCenter string) {
	if center, exists := rd.regions[region]; exists {
		fmt.Printf("Region %s has changed its administrative center from %s to %s\n", region, center, newCenter)
	} else {
		fmt.Printf("New region %s with administrative center %s\n", region, newCenter)
	}
	rd.regions[region] = newCenter
}

func (rd *RegionDirectory) rename(oldRegion, newRegion string) {
	if oldRegion == newRegion ||
		!rd.exists(oldRegion) ||
		rd.exists(newRegion) {
		fmt.Println("Incorrect")
		return
	}

	center := rd.regions[oldRegion]
	delete(rd.regions, oldRegion)
	rd.regions[newRegion] = center
	fmt.Printf("%s has been renamed to %s\n", oldRegion, newRegion)
}

func (rd *RegionDirectory) about(region string) {
	if center, exists := rd.regions[region]; exists {
		fmt.Printf("%s has administrative center %s\n", region, center)
	} else {
		fmt.Println("Incorrect")
	}
}

func (rd *RegionDirectory) all() {
	regions := make([]string, 0, len(rd.regions))
	for region := range rd.regions {
		regions = append(regions, region)
	}
	sort.Strings(regions)

	for _, region := range regions {
		fmt.Printf("%s - %s\n", region, rd.regions[region])
	}
}

func (rd *RegionDirectory) exists(region string) bool {
	_, ok := rd.regions[region]
	return ok
}

func main() {
	rd := NewRegionDirectory()
	scanner := bufio.NewScanner(os.Stdin)
	
	fmt.Print("Enter number of commands: ")
	scanner.Scan()
	var n int
	fmt.Sscanf(scanner.Text(), "%d", &n)

	for i := 0; i < n && scanner.Scan(); i++ {
		line := scanner.Text()
		parts := strings.Fields(line)
		if len(parts) == 0 {
			continue
		}

		cmd := strings.ToUpper(parts[0])
		args := parts[1:]

		switch cmd {
		case "CHANGE":
			if len(args) != 2 {
				fmt.Println("Incorrect")
				continue
			}
			rd.change(args[0], args[1])

		case "RENAME":
			if len(args) != 2 {
				fmt.Println("Incorrect")
				continue
			}
			rd.rename(args[0], args[1])

		case "ABOUT":
			if len(args) != 1 {
				fmt.Println("Incorrect")
				continue
			}
			rd.about(args[0])

		case "ALL":
			rd.all()

		default:
			fmt.Println("Incorrect")
		}
	}
}
