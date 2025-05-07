package main

import (
	"bufio"
	"container/heap"
	"fmt"
	"math/rand"
	"os"
	"sort"
	"strings"
	"time"
)

// Ticket представляет структуру билета
type Ticket struct {
	number   string
	duration int
}

// QueueSystem управляет системой очередей
type QueueSystem struct {
	windowsCount int
	tickets      []Ticket
	windows      [][]Ticket
}

// NewQueueSystem создает новую систему с указанным количеством окон
func NewQueueSystem(windowsCount int) *QueueSystem {
	return &QueueSystem{
		windowsCount: windowsCount,
		windows:      make([][]Ticket, windowsCount),
	}
}

// generateTicketNumber генерирует случайный номер билета
func (qs *QueueSystem) generateTicketNumber() string {
	rand.New(rand.NewSource(time.Now().UnixNano()))
	return fmt.Sprintf("T%03d", rand.Intn(1000))
}

// Enqueue добавляет новый билет в систему
func (qs *QueueSystem) Enqueue(duration int) {
	ticket := Ticket{
		number:   qs.generateTicketNumber(),
		duration: duration,
	}
	qs.tickets = append(qs.tickets, ticket)
	fmt.Printf(">>> %s\n", ticket.number)
}

// Distribute распределяет билеты по окнам
func (qs *QueueSystem) Distribute() {
	// Сортировка билетов по убыванию длительности
	sort.Slice(qs.tickets, func(i, j int) bool {
		return qs.tickets[i].duration > qs.tickets[j].duration
	})

	// Инициализация минимальной кучи
	pq := make(PriorityQueue, qs.windowsCount)
	for i := 0; i < qs.windowsCount; i++ {
		pq[i] = &Item{
			value:    i,
			priority: 0,
			index:    i,
		}
	}
	heap.Init(&pq)

	// Распределение билетов
	for _, ticket := range qs.tickets {
		item := heap.Pop(&pq).(*Item)
		qs.windows[item.value] = append(qs.windows[item.value], ticket)
		item.priority += ticket.duration
		heap.Push(&pq, item)
	}

	// Вычисление времени обработки для окон
	windowTimes := make([]int, qs.windowsCount)
	maxTime := 0
	for i := 0; i < qs.windowsCount; i++ {
		total := 0
		for _, t := range qs.windows[i] {
			total += t.duration
		}
		windowTimes[i] = total
		if total > maxTime {
			maxTime = total
		}
	}

	// Вывод результатов
	for i, wt := range windowTimes {
		if len(qs.windows[i]) > 0 {
			var numbers []string
			for _, t := range qs.windows[i] {
				numbers = append(numbers, t.number)
			}
			fmt.Printf(">>> Окно %d (%d минут): %s\n", i+1, wt, strings.Join(numbers, ", "))
		}
	}
	fmt.Printf(">>> Максимальное время обработки: %d минут\n", maxTime)
}

// Реализация приоритетной очереди
type Item struct {
	value    int // Номер окна
	priority int // Текущее время обработки
	index    int // Индекс в куче
}

type PriorityQueue []*Item

func (pq PriorityQueue) Len() int { return len(pq) }

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].priority < pq[j].priority
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
	pq[i].index = i
	pq[j].index = j
}

func (pq *PriorityQueue) Push(x interface{}) {
	item := x.(*Item)
	item.index = len(*pq)
	*pq = append(*pq, item)
}

func (pq *PriorityQueue) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	old[n-1] = nil
	item.index = -1
	*pq = old[0 : n-1]
	return item
}

func main() {
	fmt.Println(">>> Введите кол-во окон")
	var windows int
	fmt.Print("<<< ")
	_, err := fmt.Scan(&windows)
	if err != nil {
		fmt.Println(">>> Ошибка: введите число")
		return
	}

	system := NewQueueSystem(windows)

	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("<<< ")
		scanner.Scan()
		input := strings.TrimSpace(scanner.Text())
		if input == "" {
			continue
		}

		command := strings.ToUpper(input)
		switch {
		case strings.HasPrefix(command, "ENQUEUE"):
			var duration int
			_, err := fmt.Sscanf(input, "ENQUEUE %d", &duration)
			if err != nil {
				fmt.Println(">>> Ошибка: введите число для длительности")
				continue
			}
			system.Enqueue(duration)

		case command == "DISTRIBUTE":
			system.Distribute()
			return

		default:
			fmt.Println(">>> Неизвестная команда. Используйте ENQUEUE или DISTRIBUTE.")
		}
	}
}
