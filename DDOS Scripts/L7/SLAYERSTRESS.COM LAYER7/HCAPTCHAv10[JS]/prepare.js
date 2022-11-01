const UserAgent = require('user-agents');

function getUA(args) {
    return args.headers['user-agent'] == undefined ? new UserAgent().toString() : args.headers['user-agent'];
}

function start(args, proxies) {
    if (args.humanization == 'false') {
        proxies.forEach(ip => require('../attackers/attacker.js')(args, ip, getUA(args), 'false'));
    } else {
        return require('../engine/index.js')(args, proxies);
    }
}

module.exports = start;