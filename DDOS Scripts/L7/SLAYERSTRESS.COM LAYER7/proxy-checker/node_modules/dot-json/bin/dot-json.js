#!/usr/bin/env node

'use strict';

var DotJson = require('../')
	,fs = require('fs')
	,path = require('path')
	,docopt = require('docopt').docopt
	,package_ = require('../package.json')
	;

var usage = [
	'<file> <key-path>',
	'<file> <key-path> <value> [--indent=<n|auto>]',
	'<file> <key-path> --delete',
];

var options = [
	'--indent=<n|auto>    Indent with <n> of white space characters [default: auto]',
	'-d --delete          Delete the key-path',
	'-h --help            Show this message with options',
	'-v --version         Print the version number',
];

var name = path.basename(__filename, '.js');
var args = docopt('Usage:\n  '+name+' '+usage.join('\n  '+name+' ')+'\n\nOptions:\n  '+options.join('\n  '), {
	version: package_.version
});

try {
	var dot_json = new DotJson(args['<file>']);

	if (args['<value>']) {
		var value = args['<value>'];
		var indent = args['--indent'];

		switch(value) {
			case 'true':
				value = true;
				break;
			case 'false':
				value = false;
				break;
			case 'undefined':
				value = undefined;
				break;
			case 'null':
				value = null;
				break;
		}
		if (value == parseInt(value)) {
			value = parseInt(value);
		}
		else if(value == parseFloat(value)) {
			value = parseFloat(value);
		}
		try {
			dot_json.set(args['<key-path>'], value);
		}
		catch(e) {
			console.error(e.message);
		}
		dot_json.save(indent);
	}
	else if (args['--delete']) {
		dot_json.delete(args['<key-path>']);
		dot_json.save();
	}
	else {
		dot_json.get(args['<key-path>'], function(object){
			if (object === undefined) {
				return;
			}
			if (typeof object === 'object') {
				console.log(JSON.stringify(object, null, '  '));
				return;
			}
			console.log(object);
		});
	}
}
catch (e) {
	if(e.name === 'SyntaxError') {
		console.error("There is a syntax error in "+args['<file>']+": "+e.message);
	}
	else if(e.code === 'ENOENT' && e.path) {
		console.error("File not found: "+e.path);
	}
	else {
		console.error(e);
	}
	process.exit(1);
}
