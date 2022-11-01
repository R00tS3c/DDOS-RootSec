/*jshint node:true*/
'use strict';

var net = require('net');
var EventEmitter = require('events').EventEmitter;
var inherits = require('util').inherits;

var Address6 = require('ip-address').Address6;

module.exports = exports = Socks5ClientSocket;

function Socks5ClientSocket(options) {
	var self = this;

	EventEmitter.call(self);

	self.socket = new net.Socket();
	self.socksHost = options.socksHost || 'localhost';
	self.socksPort = options.socksPort || 1080;
	self.socksUsername = options.socksUsername;
	self.socksPassword = options.socksPassword;

	self.socket.on('error', function(err) {
		self.emit('error', err);
	});

	self.on('error', function(err) {
		if (!self.socket.destroyed) {
			self.socket.destroy();
		}
	});
	self.on('end', self.end)
}

inherits(Socks5ClientSocket, EventEmitter);

Socks5ClientSocket.prototype.ref = function () {
	return this.socket.ref();
};

Socks5ClientSocket.prototype.unref = function () {
	return this.socket.unref();
};

Socks5ClientSocket.prototype.setTimeout = function(msecs, callback) {
	return this.socket.setTimeout(msecs, callback);
};

Socks5ClientSocket.prototype.setNoDelay = function(noDelay) {
	return this.socket.setNoDelay(noDelay);
};

Socks5ClientSocket.prototype.setKeepAlive = function(setting, msecs) {
	return this.socket.setKeepAlive(setting, msecs);
};

Socks5ClientSocket.prototype.address = function() {
	return this.socket.address();
};

Socks5ClientSocket.prototype.cork = function() {
	return this.socket.cork();
};

Socks5ClientSocket.prototype.uncork = function() {
	return this.socket.uncork();
};

Socks5ClientSocket.prototype.pause = function() {
	return this.socket.pause();
};

Socks5ClientSocket.prototype.resume = function() {
	return this.socket.resume();
};
Socks5ClientSocket.prototype.pipe = function(e) {
	return this.socket.pipe(e);
};

Socks5ClientSocket.prototype.end = function(data, encoding) {
	var ret = this.socket.end(data, encoding);

	this.writable = this.socket.writable; // copy writable state from underlying socket
	
	return ret;
};

Socks5ClientSocket.prototype.destroy = function(exception) {
	return this.socket.destroy(exception);
};

Socks5ClientSocket.prototype.destroySoon = function() {
	var ret = this.socket.destroySoon();

	this.writable = false; // node's http library asserts writable to be false after destroySoon

	return ret;
};

Socks5ClientSocket.prototype.setEncoding = function(encoding) {
	return this.socket.setEncoding(encoding);
};

Socks5ClientSocket.prototype.write = function(data, encoding, cb) {
	return this.socket.write(data, encoding, cb);
};

Socks5ClientSocket.prototype.read = function(size) {
	return this.socket.read(size);
};

Socks5ClientSocket.prototype.connect = function(port, host) {
	var self = this;

	if (typeof port == 'string' && /^\d+$/.test(port)) {
		port = parseInt(port, 10);
	}

	if (!Number.isInteger(port) || port < 1 || port > 65535) {
		throw new TypeError('Invalid port: ' + port);
	}

	if (!host || typeof host !== 'string') {
		throw new TypeError('Invalid host: ' + host);
	}

	self.socket.connect(self.socksPort, self.socksHost, function() {
		authenticateWithSocks(self, function() {
			connectSocksToHost(self, host, port, function() {
				self.onProxied();
			});
		});
	});

	return self;
};

Socks5ClientSocket.prototype.onProxied = function() {
	var self = this;

	self.socket.on('close', function(hadErr) {
		self.emit('close', hadErr);
	});

	self.socket.on('end', function() {
		self.emit('end');
	});

	self.socket.on('data', function(data) {
		self.emit('data', data);
	});

	self.socket._httpMessage = self._httpMessage;
	self.socket.parser = self.parser;
	self.socket.ondata = self.ondata;
	self.writable = true;
	self.readable = true;
	self.emit('connect');
};

function authenticateWithSocks(client, cb) {
	var authMethods, buffer;

	client.socket.once('data', function(data) {
		var error, request, buffer, i, l;

		if (2 !== data.length) {
			error = 'Unexpected number of bytes received.';
		} else if (0x05 !== data[0]) {
			error = 'Unexpected SOCKS version number: ' + data[0] + '.';
		} else if (0xFF === data[1]) {
			error = 'No acceptable authentication methods were offered.';
		} else if (authMethods.indexOf(data[1]) === -1) {
			error = 'Unexpected SOCKS authentication method: ' + data[1] + '.';
		}

		if (error) {
			client.emit('error', new Error('SOCKS authentication failed. ' + error));
			return;
		}

		// Begin username and password authentication.
		if (0x02 === data[1]) {
			client.socket.once('data', function(data) {
				var error;

				if (2 !== data.length) {
					error = 'Unexpected number of bytes received.';
				} else if (0x01 !== data[0]) {
					error = 'Unexpected authentication method code: ' + data[0] + '.';
				} else if (0x00 !== data[1]) {
					error = 'Username and password authentication failure: ' + data[1] + '.';
				}

				if (error) {
					client.emit('error', new Error('SOCKS authentication failed. ' + error));
				} else {
					cb();
				}
			});

			request = [0x01];
			parseString(client.socksUsername, request);
			parseString(client.socksPassword, request);
			client.write(new Buffer(request));

		// No authentication to negotiate.
		} else {
			cb();
		}
	});

	// Add the "no authentication" method.
	authMethods = [0x00];
	if (client.socksUsername) {
		authMethods.push(0x02);
	}

	buffer = new Buffer(2 + authMethods.length);
	buffer[0] = 0x05; // SOCKS version.
	buffer[1] = authMethods.length; // Number of authentication methods.

	// Copy the authentication method codes into the request buffer.
	authMethods.forEach(function(authMethod, i) {
		buffer[2 + i] = authMethod;
	});

	client.write(buffer);
}

function connectSocksToHost(client, host, port, cb) {
	var request, buffer;

	client.socket.once('data', function(data) {
		var error;

		if (data[0] !== 0x05) {
			error = 'Unexpected SOCKS version number: ' + data[0] + '.';
		} else if (data[1] !== 0x00) {
			error = getErrorMessage(data[1]) + '.';
		} else if (data[2] !== 0x00) {
			error = 'The reserved byte must be 0x00.';
		}

		if (error) {
			client.emit('error', new Error('SOCKS connection failed. ' + error));
			return;
		}

		cb();
	});

	request = [];
	request.push(0x05); // SOCKS version.
	request.push(0x01); // Command code: establish a TCP/IP stream connection.
	request.push(0x00); // Reserved - must be 0x00.

	switch (net.isIP(host)) {

	// Add a hostname to the request.
	case 0:
		request.push(0x03);
		parseString(host, request);
		break;

	// Add an IPv4 address to the request.
	case 4:
		request.push(0x01);
		parseIPv4(host, request);
		break;
	case 6:
		request.push(0x04);
		if (parseIPv6(host, request) === false) {
			client.emit('error', new Error('IPv6 host parsing failed. Invalid address.'));
			return;
		}
		break;
	}

	// Add a placeholder for the port bytes.
	request.length += 2;

	buffer = new Buffer(request);
	buffer.writeUInt16BE(port, buffer.length - 2);

	client.write(buffer);
}

function parseString(string, request) {
	var buffer = new Buffer(string), i, l = buffer.length;

	// Declare the length of the following string.
	request.push(l);

	// Copy the hostname buffer into the request buffer.
	for (i = 0; i < l; i++) {
		request.push(buffer[i]);
	}
}

function parseIPv4(host, request) {
	var i, ip, groups = host.split('.');

	for (i = 0; i < 4; i++) {
		ip = parseInt(groups[i], 10);
		request.push(ip);
	}
}

function parseIPv6(host, request) {
	var i, b1, b2, part1, part2, address, groups;

	// `#canonicalForm` returns `null` if the address is invalid.
	address = new Address6(host).canonicalForm();
	if (!address) {
		return false;
	}

	groups = address.split(':');

	for (i = 0; i < groups.length; i++) {
		part1 = groups[i].substr(0,2);
		part2 = groups[i].substr(2,2);

		b1 = parseInt(part1, 16);
		b2 = parseInt(part2, 16);

		request.push(b1);
		request.push(b2);
	}

	return true;
}

function getErrorMessage(code) {
	switch (code) {
	case 1:
		return 'General SOCKS server failure';
	case 2:
		return 'Connection not allowed by ruleset';
	case 3:
		return 'Network unreachable';
	case 4:
		return 'Host unreachable';
	case 5:
		return 'Connection refused';
	case 6:
		return 'TTL expired';
	case 7:
		return 'Command not supported';
	case 8:
		return 'Address type not supported';
	default:
		return 'Unknown status code ' + code;
	}
}
