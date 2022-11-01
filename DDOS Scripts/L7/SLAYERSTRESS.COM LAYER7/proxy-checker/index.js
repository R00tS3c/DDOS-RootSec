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

const usage = `node index.js --proxy proxy.txt --pool 10`;

if (process.argv[2] == null || parsedArgs['help'] == true) return console.log(usage);

var arguments = {
    //----Target-Options----
    target: process.argv[2],
    headers: parsedArgs['headers'] || 'false',
    postdata: parsedArgs['postdata'] || 'false',
    precheck: parsedArgs['precheck'] || 'false',

    //----Proxy-Options----
    proxy: parsedArgs['proxy'] || 'Enter Proxy File',
    proxylen: parseInt(parsedArgs['proxylen']) || 'false',
    pool: parseInt(parsedArgs['pool']) || 10,
    uptime: parseInt(parsedArgs['uptime']) || 10000,

    // Others
    debug: parsedArgs['debug'] || 'false',

};

arguments.postdata = arguments.postdata == 'false' ? false : arguments.postdata;
arguments.headers = arguments.headers == 'false' ? 'false' : parseHeaders(arguments.headers);
arguments.precheck = arguments.precheck == "false" ? "https://google.com/" : arguments.target;


function start(args){
    console.log(args);


    return require('./misc/proxies.js')(args);
}

start(arguments);