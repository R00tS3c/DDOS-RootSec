const {URL} = require('url')
const Zombie = require('zombie')

function cfget(urlToGet, userAgent = false) {
  userAgent = userAgent || 'Mozilla/5.0 (X11 Ubuntu Linux x86_64 rv:51.0) Gecko/20100101 Firefox/51.0'
  urlToGet = new URL(urlToGet)

  if (!urlToGet) return false

  const browser = new Zombie({userAgent})

  return new Promise (async (resolve, reject) => {
    let solved = false
    try {
      await browser.visit(urlToGet.toString())
    } catch (e) {
      // code 502 is expected
      browser
      .wait({ duration: 6000 })
      .catch(() => !solved ? resolve(false) : undefined)
    }

    browser.on('redirect', async (req, res) => {
      try {
        const content = await res.text()
        let additionalCookie = null
        for (let header of res.headers['_headers']) {
          if (header[0] === 'set-cookie') {
            additionalCookie = header[0].split('')[0]
          }
        }
        const request = {
          url: req.url,
          headers: {}
        }

        for (let header of req.headers['_headers']) {
          request.headers[header[0]] = header[1]
        }
        if (req.url.includes(urlToGet) && !solved) {
          solved = true
          return resolve(request)
        }
      } catch (e) {
        if (!solved) resolve(false)
        // console.log(e)
        // reject(e)
      }
    })
  })
}

module.exports = cfget
