package main

type Attack struct {
	Url string
	Host string
	AttackMethod string
	PostData interface{}
	RequestsPerIP int
	Cookie interface{}
	LoadedProxies string
	Jokeragent interface{}
}

type System struct {
	Banner string
	HTTP2Timeout int
	Attack *Attack
}
