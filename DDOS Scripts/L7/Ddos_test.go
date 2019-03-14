package ddos_test

import (
	"fmt"
	"net/http"
	"strconv"
	"testing"
	"time"

	ddos "github.com/Konstantin8105/DDoS"
	freeport "github.com/Konstantin8105/FreePort"
)

func TestNewDDoS(t *testing.T) {
	d, err := ddos.New("http://127.0.0.1", 1)
	if err != nil {
		t.Error("Cannot create a new ddos structure. Error = ", err)
	}
	if d == nil {
		t.Error("Cannot create a new ddos structure")
	}
}

func TestDDoS(t *testing.T) {
	port, err := freeport.Get()
	if err != nil {
		t.Errorf("Cannot found free tcp port. Error = ", err)
	}
	createServer(port, t)

	url := "http://127.0.0.1:" + strconv.Itoa(port)
	d, err := ddos.New(url, 100)
	if err != nil {
		t.Error("Cannot create a new ddos structure")
	}
	d.Run()
	time.Sleep(time.Second)
	d.Stop()
	success, amount := d.Result()
	if success == 0 || amount == 0 {
		t.Errorf("Negative result of DDoS attack.\nSuccess requests = %v.\nAmount requests = %v", success, amount)
	}
}

// Create a simple go server
func createServer(port int, t *testing.T) {
	go func() {
		http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
			fmt.Fprintf(w, "Hi there, I love %s!", r.URL.Path[1:])
		})
		if err := http.ListenAndServe(":"+strconv.Itoa(port), nil); err != nil {
			t.Fatalf("Server is down. %v", err)
		}
	}()
}

func TestWorkers(t *testing.T) {
	_, err := ddos.New("127.0.0.1", 0)
	if err == nil {
		t.Error("Cannot create a new ddos structure")
	}
}

func TestUrl(t *testing.T) {
	_, err := ddos.New("some_strange_host", 1)
	if err == nil {
		t.Error("Cannot create a new ddos structure")
	}
}

func ExampleNew() {
	workers := 100
	d, err := ddos.New("http://127.0.0.1:80", workers)
	if err != nil {
		panic(err)
	}
	d.Run()
	time.Sleep(time.Second)
	d.Stop()
	fmt.Println("DDoS attack server: http://127.0.0.1:80")
	// Output: DDoS attack server: http://127.0.0.1:80
}