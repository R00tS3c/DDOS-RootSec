const { Worker } = require('worker_threads');
const fs = require("fs");
const proxies = [...new Set(fs.readFileSync(process.argv[4]).toString().match(/\S+/g))];

function runService(workerData) {
    return new Promise((resolve, reject) => {
        const worker = new Worker('./httpmurder.js', { workerData });
        worker.on('message', resolve);
        worker.on('error', reject);
        worker.on('exit', (code) => {
            if (code !== 0) {
                reject(new Error("Worker stopped with " + code));
            }
        })
    })
}

if (process.argv.length != 5) process.exit(-1);
for (let index = 0; index < process.argv[3]; index++) {
    console.log("service run");
    runService([process.argv[2], 1231313, proxies]).then(mess => {
        console.log(mess);
    })

}