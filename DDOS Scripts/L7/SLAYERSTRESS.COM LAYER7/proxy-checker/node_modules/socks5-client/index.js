/*jshint node:true*/
'use strict';

var Socket = require('./lib/Socket');

exports.Socket = Socket;

exports.createConnection = function(options) {
	var host, hostname = options.hostname, port = options.port;

	if (options.host && (!hostname || !port)) {
		host = options.host.split(':');
	}

	if (!port && host) {
		port = parseInt(host[1], 10) || 0;
	}

	if (!hostname && host) {
		hostname = host[0];
	}

	return new Socket(options).connect(port, hostname);
};
