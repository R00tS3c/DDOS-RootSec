//@ts-check
require('events').EventEmitter.defaultMaxListeners = 0;
const playwright = require('playwright')
//captcha
const { chromium } = require('playwright-extra')
const RecaptchaPlugin = require('@extra/recaptcha')
const RecaptchaOptions = {
  visualFeedback: true,
  provider: {
    id: '2captcha',
    token: '0',
  },
}

function sleep(millis) {
    var t = (new Date()).getTime();
    var i = 0;
    while (((new Date()).getTime() - t) < millis) {
        i++;
    }
}

//var sleeper = getbpaw;

function solving(message){
    return new Promise((resolve,reject) => {
        console.log(`[STRESSID.CLUB] Launch browser...`)
		console.log(`[STRESSID.CLUB] Target: ${message.url}`)
		//chromium.use(RecaptchaPlugin(RecaptchaOptions))
        playwright.firefox.launch({
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
            const page = await browser.newPage({ userAgent: 'Mozilla/5.0 (Windows NT 6.1; rv:104.0) Gecko/20100101 Firefox/104.0' })
			console.log(`[STRESSID.CLUB] Function: Create a browser.`)
            await page.setDefaultNavigationTimeout(1000000);
            await page.evaluate(() => {
                Object.defineProperty(navigator, 'webdriver', {
                    get: () => false
                });
            })
            await page.evaluate(() => {
                Object.defineProperty(navigator, 'platform', {
                    get: () => 'Win32'
                });
            })
            try {
                var admv = await page.goto(message.url)
            } catch (error) {
                await browser.close();			
            }	
								
            try {
                const cloudFlareWrapper = await page.$('#challenge-running'); 
                if (cloudFlareWrapper) {
                    await page.waitForTimeout(25000, { waitUntil:'networkidle0' })
                    const maybecaptcha = await page.$('#cf-hcaptcha-container');
                    if (maybecaptcha) {
                        try {
                            await page.waitForSelector('#cf-hcaptcha-container'); 
                            await page.solveRecaptchas();					
							
                        } catch (e) {
                        }
                    }
                }
                await page.waitForTimeout(40000, { waitUntil: 'networkidle0' })
                const cloudFlareWrapper2 = await page.$('#challenge-running'); 
                if (cloudFlareWrapper2) {
                    await page.waitForTimeout(25000, { waitUntil:'networkidle0' })
                    const maybecaptcha2 = await page.$('#cf-hcaptcha-container');
                    if (maybecaptcha2) {
                        try {
                            await page.waitForSelector('#cf-hcaptcha-container');                          
                            await page.solveRecaptchas();
                        } catch (e) {
							
                        }
                    }
                    await page.waitForTimeout(40000, { waitUntil: 'networkidle0' })
                    const cloudFlareWrapper3 = await page.$('#challenge-running'); 
                    if (cloudFlareWrapper3) {
                        await browser.close();
                        reject();
                        return;
                    }
                }											
								
				var pagestitle = (await page.title()).toString().trim();
				const cookies = await page.context().cookies();			
				if (cookies) {
					console.log(`[STRESSID.CLUB] Title: ${pagestitle}`)
					resolve(cookies);
					await browser.close();
					return;
				}
				else {
					await browser.close();	
				}
            }
			catch (ee) { 
                reject(ee)
                await browser.close();
            };
        })
    })
}

module.exports = { solving:solving }
