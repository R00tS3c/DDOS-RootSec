function start(args, proxy, ua, secondcookies){

    args.target = args.target.replace(/%RAND%/g, Math.random().toString(36).substring(7));
    if(!(args.postdata == false)) args.postdata = args.postdata.replace(/%RAND%/g, Math.random().toString(36).substring(7));

    if(args.mode == 'http') require('./http.js')(args, proxy, ua, secondcookies);
    if(args.mode == 'httptest') require('./httptest.js')(args, proxy, ua, secondcookies);
    if(args.mode == 'socket') require('./socket.js')(args, proxy, ua, secondcookies);
	if(args.mode == 'sockettest') require('./sockettest.js')(args, proxy, ua, secondcookies);
    if(args.mode == 'tlsfl') require('./tlsfl.js')(args, proxy, ua, secondcookies);
    if(args.mode == 'none') return;
}

module.exports = start;