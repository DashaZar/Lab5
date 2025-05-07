package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Cell struct {
	item     string
	quantity int
}

var warehouse map[string]*Cell

func initializeWarehouse() {
	warehouse = make(map[string]*Cell)
	for _, zone := range []rune{'A', 'B', 'C'} {
		for shelf := 1; shelf <= 20; shelf++ {
			for section := 1; section <= 5; section++ {
				for rack := 1; rack <= 2; rack++ {
					address := fmt.Sprintf("%c%d%d%d", zone, shelf, section, rack)
					warehouse[address] = &Cell{"", 0}
				}
			}
		}
	}
}

func processAddCommand(tokens []string) {
	if len(tokens) != 4 {
		fmt.Println("Ошибка: Неверный формат. Используйте: ADD <товар> <кол-во> <адрес>")
		return
	}

	item := tokens[1]
	quantity, err := strconv.Atoi(tokens[2])
	if err != nil || quantity <= 0 {
		fmt.Println("Ошибка: Количество должно быть положительным числом")
		return
	}

	address := tokens[3]
	cell, exists := warehouse[address]
	if !exists {
		fmt.Printf("Ошибка: Ячейка %s не существует\n", address)
		return
	}

	if cell.quantity == 0 {
		if quantity > 10 {
			fmt.Println("Ошибка: В ячейку нельзя добавить более 10 единиц")
			return
		}
		cell.item = item
		cell.quantity = quantity
		fmt.Printf("Добавлено %d ед. товара \"%s\" в %s\n", quantity, item, address)
	} else {
		if cell.item != item {
			fmt.Printf("Ошибка: В ячейке находится другой товар (\"%s\")\n", cell.item)
			return
		}
		if cell.quantity+quantity > 10 {
			fmt.Printf("Ошибка: Превышен лимит. Доступно места: %d\n", 10-cell.quantity)
			return
		}
		cell.quantity += quantity
		fmt.Printf("Обновлено: %s x%d в %s\n", item, cell.quantity, address)
	}
}

func processRemoveCommand(tokens []string) {
	if len(tokens) != 4 {
		fmt.Println("Ошибка: Неверный формат. Используйте: REMOVE <товар> <кол-во> <адрес>")
		return
	}

	item := tokens[1]
	quantity, err := strconv.Atoi(tokens[2])
	if err != nil || quantity <= 0 {
		fmt.Println("Ошибка: Количество должно быть положительным числом")
		return
	}

	address := tokens[3]
	cell, exists := warehouse[address]
	if !exists {
		fmt.Printf("Ошибка: Ячейка %s не существует\n", address)
		return
	}

	if cell.quantity == 0 {
		fmt.Printf("Ошибка: Ячейка %s пуста\n", address)
		return
	}

	if cell.item != item {
		fmt.Printf("Ошибка: В ячейке находится товар \"%s\"\n", cell.item)
		return
	}

	if cell.quantity < quantity {
		fmt.Printf("Ошибка: Недостаточно товара. Доступно: %d\n", cell.quantity)
		return
	}

	cell.quantity -= quantity
	if cell.quantity == 0 {
		cell.item = ""
	}
	fmt.Printf("Удалено %d ед. товара \"%s\" из %s. Остаток: %d\n", quantity, item, address, cell.quantity)
}

func processInfoCommand() {
	total := 0
	zones := make(map[rune]int)
	var filled, empty []string

	for addr, cell := range warehouse {
		if cell.quantity > 0 {
			total += cell.quantity
			zones[rune(addr[0])] += cell.quantity
			filled = append(filled, fmt.Sprintf("%s: \"%s\" x%d", addr, cell.item, cell.quantity))
		} else {
			empty = append(empty, addr)
		}
	}

	fmt.Printf("Общая загруженность: %.2f%%\n", float64(total)/6000*100)
	for _, z := range []rune{'A', 'B', 'C'} {
		fmt.Printf("Зона %c: %.2f%%\n", z, float64(zones[z])/2000*100)
	}

	fmt.Printf("\nЗаполненные ячейки (%d):\n", len(filled))
	for _, f := range filled {
		fmt.Println(" ", f)
	}

	fmt.Printf("\nПустые ячейки (%d):\n", len(empty))
	for _, e := range empty {
		fmt.Println(" ", e)
	}
}

func main() {
	initializeWarehouse()
	fmt.Println("=== Система управления складом ===")
	fmt.Println("Формат команд:")
	fmt.Println("  ADD <товар> <кол-во> <адрес>")
	fmt.Println("  REMOVE <товар> <кол-во> <адрес>")
	fmt.Println("  INFO")

	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("> ")
		scanner.Scan()
		input := strings.Fields(scanner.Text())
		if len(input) == 0 {
			continue
		}

		switch input[0] {
		case "ADD":
			processAddCommand(input)
		case "REMOVE":
			processRemoveCommand(input)
		case "INFO":
			processInfoCommand()
		default:
			fmt.Println("Неизвестная команда")
		}
	}
}
