package main

import (
	"fmt"
	"sync"
	"time"
)

var printMu sync.Mutex

func report(label string, format string, args ...any) {
	printMu.Lock()
	defer printMu.Unlock()
	fmt.Printf("[%s] "+format+"\n", append([]any{label}, args...)...)
}

func count(n int, interval time.Duration, label string, started chan<- struct{}) float64 {
	start := time.Now()
	report(label, "before loop %.1f", interval.Seconds())
	close(started)

	for i := 0; i < n; i++ {
		time.Sleep(interval)
		report(label, "slept %.1f | i: %d < n: %d", interval.Seconds(), i, n)
	}

	elapsed := time.Since(start).Seconds()
	report(label, "after loop %.1f", elapsed)
	return elapsed
}

func main() {
	fmt.Println("begin")

	taskA := make(chan float64, 1)
	taskB := make(chan float64, 1)
	startedA := make(chan struct{})
	startedB := make(chan struct{})

	go func() { taskA <- count(2, time.Second, "task a", startedA) }()
	<-startedA
	go func() { taskB <- count(3, 600*time.Millisecond, "task b", startedB) }()
	<-startedB

	total := <-taskA + <-taskB

	fmt.Println("end")
	fmt.Printf("total: %.1f\n", total)
}
