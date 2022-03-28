module.exports = function Cloudflare() {
    const privacypass = require('./privacypass'),
        cloudscraper = require('cloudscraper'),
        request = require('request');
    var privacyPassSupport = true;
    var bypassHeader = false;

    function useNewToken() {
        bypassHeader = privacypass(l7.target);
        console.log('[cloudflare-bypass ~ privacypass]: generated new token ::', bypassHeader);
    }

    if (l7.firewall[1] == 'captcha') {
        privacyPassSupport = l7.firewall[2];
        useNewToken();
    }

    function bypass(proxy, uagent, callback, force) {
        var cookie = "";
        if (l7.firewall[1] == 'captcha' || force && privacyPassSupport) {
            request.get({
                url: l7.target,
                gzip: true,
                proxy: proxy,
                gzip: true,
                headers: {
                    'Connection': 'Keep-Alive',
                    'Cache-Control': 'max-age=0',
                    'Upgrade-Insecure-Requests': 1,
                    'User-Agent': uagent,
                    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                    'Accept-Encoding': 'gzip, deflate, br',
                    'Accept-Language': 'en-US;q=0.9'
                }
            }, (err, res) => {
                if (!res) {
                    return false;
                }
                if (res.headers['cf-chl-bypass'] && res.headers['cf-chl-bypass'] == '1') {
                    if (!res.headers['set-cookie']) {
                        return false;
                    }
                } else {
                    if (l7.firewall[1] == 'captcha') {
                        console.warn('[cloudflare-bypass]: The target is not supporting privacypass, now closing rip...');
                        process.exit(34);
                        return false;
                    } else {
                        privacyPassSupport = false;
                    }
                }
    
                cookie = res.headers['set-cookie'].shift().split(';').shift();
                if (l7.firewall[1] == 'captcha' && privacyPassSupport || force && privacyPassSupport) {
                    cloudscraper.get({
                        url: l7.target,
                        gzip: true,
                        proxy: proxy,
                        headers: {
                            'Connection': 'Keep-Alive',
                            'Cache-Control': 'max-age=0',
                            'Upgrade-Insecure-Requests': 1,
                            'User-Agent': uagent,
                            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                            'Accept-Encoding': 'gzip, deflate, br',
                            'Accept-Language': 'en-US;q=0.9',
                            'challenge-bypass-token': bypassHeader,
                            "Cookie": cookie
                        }
                    }, (err, res) => {
                        if (err || !res) return false;
                        if (res.headers['set-cookie']) {
                            cookie += '; ' + res.headers['set-cookie'].shift().split(';').shift();
                            cloudscraper.get({
                                url: l7.target,
                                proxy: proxy,
                                headers: {
                                    'Connection': 'Keep-Alive',
                                    'Cache-Control': 'max-age=0',
                                    'Upgrade-Insecure-Requests': 1,
                                    'User-Agent': uagent,
                                    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                                    'Accept-Encoding': 'gzip, deflate, br',
                                    'Accept-Language': 'en-US;q=0.9',
                                    "Cookie": cookie
                                }
                            }, (err, res, body) => {
                                if (err || !res || res && res.statusCode == 403) {
                                    console.warn('[cloudflare-bypass ~ privacypass]: Failed to bypass with privacypass, generating new token:');
                                    useNewToken();
                                    return;
                                }
                                callback(cookie);
                            });
                        } else {
                            console.log(res.statusCode, res.headers);
                            if (res.headers['cf-chl-bypass-resp']) {
                                let respHeader = res.headers['cf-chl-bypass-resp'];
                                switch (respHeader) {
                                    case '6':
                                        console.warn("[privacy-pass]: internal server connection error occurred");
                                        useNewToken();
                                        break;
                                    case '5':
                                        console.warn(`[privacy-pass]: token verification failed for ${l7.target}`);
                                        break;
                                    case '7':
                                        console.warn(`[privacy-pass]: server indicated a bad client request`);
                                        break;
                                    case '8':
                                        console.warn(`[privacy-pass]: server sent unrecognised response code (${header.value})`);
                                        break;
                                }
                                return bypass(proxy, uagent, callback, true);
                            }
                        }
                    });
                } else {
                    cloudscraper.get({
                        url: l7.target,
                        proxy: proxy,
                        headers: {
                            'Connection': 'Keep-Alive',
                            'Cache-Control': 'max-age=0',
                            'Upgrade-Insecure-Requests': 1,
                            'User-Agent': uagent,
                            'Accept-Language': 'en-US;q=0.9'
                        }
                    }, (err, res) => {
                        if (err || !res || !res.request.headers.cookie) {
                            if (err) {
                                if (err.name == 'CaptchaError') {
                                    return bypass(proxy, uagent, callback, true);
                                }
                            }
                            return false;
                        }
                        callback(res.request.headers.cookie);
                    });
                }
            });
        } else if (l7.firewall[1] == 'uam' && privacyPassSupport == false) {
            cloudscraper.get({
                url: l7.target,
                proxy: proxy,
                headers: {
                    'Upgrade-Insecure-Requests': 1,
                    'User-Agent': uagent
                }
            }, (err, res, body) => {
                if (err) {
                    if (err.name == 'CaptchaError') {
                        return bypass(proxy, uagent, callback, true);
                    }
                    return false;
                }
                if (res && res.request.headers.cookie) {
                    callback(res.request.headers.cookie);
                } else if (res && body && res.headers.server == 'cloudflare') {
                    if (res && body && /Why do I have to complete a CAPTCHA/.test(body) && res.headers.server == 'cloudflare' && res.statusCode !== 200) {
                        return bypass(proxy, uagent, callback, true);
                    }
                } else {
    
                }
            });
        } else {
            cloudscraper.get({
                url: l7.target,
                gzip: true,
                proxy: proxy,
                headers: {
                    'Connection': 'Keep-Alive',
                    'Cache-Control': 'max-age=0',
                    'Upgrade-Insecure-Requests': 1,
                    'User-Agent': uagent,
                    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                    'Accept-Encoding': 'gzip, deflate, br',
                    'Accept-Language': 'en-US;q=0.9'
                }
            }, (err, res, body) => {
                if (err || !res || !body || !res.headers['set-cookie']) {
                    if (res && body && /Why do I have to complete a CAPTCHA/.test(body) && res.headers.server == 'cloudflare' && res.statusCode !== 200) {
                        return bypass(proxy, uagent, callback, true);
                    }
                    return false;
                }
                cookie = res.headers['set-cookie'].shift().split(';').shift();
                callback(cookie);
            });
        }
    }

    return bypass;
}