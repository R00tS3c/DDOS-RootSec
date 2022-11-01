require('events').EventEmitter.defaultMaxListeners = 0;
const puppeteer = require('puppeteer-extra')
const RecaptchaPlugin = require('puppeteer-extra-plugin-recaptcha');
const StealthPlugin = require('puppeteer-extra-plugin-stealth');


function solving(message) {
    return new Promise((resolve, reject) => {
        console.log(`[STRESSID.CLUB] Launch browser...`)
        console.log(`[STRESSID.CLUB] Target: ${message.url}`)
        puppeteer.use(StealthPlugin())
        puppeteer.use(RecaptchaPlugin({
            provider: {
                id: '2captcha',
                token: '484f6867edd89ad23af2d4cdfae8cbf5'
            },
            visualFeedback: true
        }))
        puppeteer.launch({
            //product: 'firefox',
            headless: true,
            ignoreHTTPSErrors: true,
            ignoreDefaultArgs: [
                "--disable-extensions",
                "--enable-automation"
            ],
            args: [
                `--proxy-server=http://${message.proxy}`,
                '--disable-features=IsolateOrigins,site-per-process,SitePerProcess',
                '--flag-switches-begin --disable-site-isolation-trials --flag-switches-end',
                `--window-size=1920,1080`,
                "--window-position=000,000",
                "--disable-dev-shm-usage",
                '--no-sandbox',
                '--disable-setuid-sandbox',
                '--disable-dev-shm-usage',
                '--disable-accelerated-2d-canvas',
                '--no-first-run',
                '--no-zygote',
                '--disable-gpu',
                '--hide-scrollbars',
                '--mute-audio',
                '--disable-gl-drawing-for-tests',
                '--disable-canvas-aa',
                '--disable-2d-canvas-clip-aa',
                '--disable-web-security',
                '--ignore-certificate-errors',
                '--ignore-certificate-errors-spki-list',
                '--disable-features=IsolateOrigins,site-per-process',
            ]
        }).then(async (browser) => {

            try {
                const page = await browser.newPage()
                console.log(`[STRESSID.CLUB] Function: Create a browser.`)
                await page.setUserAgent(message.user_agent);
                await page.setJavaScriptEnabled(true);
                await page.setDefaultNavigationTimeout(0);
                await page.evaluateOnNewDocument(() => {
                    Object.defineProperty(navigator, 'webdriver', {
                        get: () => false
                    });
                })
                await page.evaluateOnNewDocument(() => {
                    Object.defineProperty(navigator, 'platform', {
                        get: () => 'Win32'
                    });
                })
                try {
                    const headeri = await page.on('request', req => {
                        const headers = req.headers();
                        headers['Host'] = `${message.fixhost}`;
                        headers['Upgrade-Insecure-Requests'] = '1';
                        headers['Sec-Fetch-Site'] = 'same-origin';
                        headers['Sec-Fetch-User'] = '?1';
                        headers['Sec-Fetch-Mode'] = 'navigate';
                        headers['Sec-Fetch-Dest'] = 'document';
                        headers['Sec-Ch-Ua-Platform'] = '"Windows"';
                        headers['Sec-Ch-Ua-Mobile'] = '?0';
                        headers['Accept'] = 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9';
                        headers['Accept-Language'] = 'ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7';
                        headers['Accept-Encoding'] = 'gzip, deflate, br';
                        headers['Sec-GPC'] = '1';
                        headers['Upgrade-Insecure-Requests'] = '1';
                        headers['Cache-Control'] = 'no-cache';
                        headers['Pragma'] = 'no-cache';
                        //console.log(req.headers());
                    });
                    var admv = await page.goto(message.url)
                } catch (error) {
                    await browser.close();
                }
                try {
                    const cloudFlareWrapper = await page.$('#challenge-running');
                    if (cloudFlareWrapper) {
                        await page.waitForTimeout(25000, {
                            waitUntil: 'networkidle0'
                        })
                        const maybecaptcha = await page.$('#cf-hcaptcha-container');
                        if (maybecaptcha) {
                            try {
                                await page.waitForSelector('#cf-hcaptcha-container');
                                await page.solveRecaptchas();

                            } catch (e) {}
                        }
                    }
                    await page.waitForTimeout(20000, {
                        waitUntil: 'networkidle0'
                    })
                    const cloudFlareWrapper2 = await page.$('#challenge-running');
                    if (cloudFlareWrapper2) {
                        await page.waitForTimeout(25000, {
                            waitUntil: 'networkidle0'
                        })
                        const maybecaptcha2 = await page.$('#cf-hcaptcha-container');
                        if (maybecaptcha2) {
                            try {
                                await page.waitForSelector('#cf-hcaptcha-container');
                                await page.solveRecaptchas();
                            } catch (e) {

                            }
                        }
                        await page.waitForTimeout(20000, {
                            waitUntil: 'networkidle0'
                        })
                        const cloudFlareWrapper3 = await page.$('#challenge-running');
                        if (cloudFlareWrapper3) {
                            await browser.close();
                            reject();
                            return;
                        }
                    }
                    const pagestitle = (await page.title()).toString().trim();
                    const cookies = await page.cookies()
                    if (cookies) {
                        resolve(cookies);
                        resolve();
                        console.log(`[STRESSID.CLUB] Title: ${pagestitle}`)
                        await browser.close();
                        return;
                    }
                } catch (ee) {
                    reject(ee)
                    await browser.close();
                }

            } catch (e) {
                // handle initialization error
            }

        })
    })
}

module.exports = {
    solving: solving
}