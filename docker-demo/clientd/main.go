package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	i := 0
	fmt.Fprintln(os.Stdout, "Simulate some work")
	for {
		i++
		time.Sleep(10 * time.Second)
		fmt.Fprintf(os.Stderr, "counter ticks: %d\n", i)
	}
}
