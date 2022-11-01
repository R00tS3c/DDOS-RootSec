var path = require('path');
var fs = require('fs');
var keypath = require('underscore-keypath');
var detectIndent = require('detect-indent');

var isIntString = function(string) {
	return string == parseInt(string);
}

var typeOf = function(object) {
	return object instanceof Array ? 'array' : typeof object;
}

module.exports = function(file) {

	_this = this;

	this._object = undefined;

	this._file = undefined;

	this._indent = undefined;

	this._stat = {};

	this._read_queue = [];

	_this._wait_for_read = false;

	this.file = function(file) {
		_this._file = path.resolve(file);
		return _this;
	}

	this._readJson = function(callback) {
		if (_this._useCache()) {
			callback();
			return;
		}
		_this._wait_for_read = true;
		fs.readFile(_this._file, function(err, content) {
			_this._object = JSON.parse(content);
			callback();
			_this._exec_read_queue();
			_this._wait_for_read = false;
		});
	}

	this.content = function() {
		return fs.readFileSync(_this._file, 'utf8');
	}

	this.indent = function() {
		if (_this._indent === undefined) {
			try {
				_this._indent = detectIndent(_this.content()).indent;
			}
			catch (e) {
				_this._indent = 2
			}
		}
		return _this._indent
	}

	this._readJsonSync = function(log) {
		if (_this._useCache()) {
			return;
		}
		_this._object = JSON.parse(_this.content());
		_this._exec_read_queue();
		_this._wait_for_read = false;
	}

	this._useCache = function() {
		var stat = fs.statSync(_this._file);
		if (_this._object && _this._stat.mtime !== undefined && stat.mtime.toString() === _this._stat.mtime.toString() && stat.size === _this._stat.size) {
			_this._exec_read_queue();
			return true;
		}

		_this._stat = stat;
		return false;
	}

	this._exec_read_queue = function() {
		for (var func in _this._read_queue) {
			_this._read_queue[func]();
			delete _this._read_queue[func];
		}
	}

	this.get = function(key_path, callback) {
		if (typeof key_path === 'function') {
			callback = key_path;
			key_path = undefined;
		}
		_this._readJsonSync();
		var object;

		if (key_path === undefined) {
			object = _this._object;
		}
		else {
			object = keypath(_this._object).valueForKeyPath(key_path);
		}

		if (callback === undefined) {
			return object;
		}
		callback(object);
		return this;
	}

	this.set = function(key_path, value) {

		if (_this._file === undefined) {
			if (_this._object === undefined) {
				_this._object = {};
			}
			_this._setValueForKeyPath(key_path, value);
			return _this;
		}

		try {
			_this._readJsonSync();
			_this._setValueForKeyPath(key_path, value);
		}
		catch(e) {
			if (_this._object === undefined) {
				_this._object = {};
			}
			if (e.code === 'ENOENT' && e.path) {
				_this._setValueForKeyPath(key_path, value);
			}
		}
		return _this;
	}

	this._setValueForKeyPath = function(key_path, value) {
		var key_array = key_path.replace('..', '').split('.');
		var tip_key = key_array.pop();

		if (key_array.includes('__proto__') || key_array.includes('constructor') || key_array.includes('prototype')) {
			return;
		}

		var parent_path = key_array.join('.');

		var parent_object = keypath(_this._object).valueForKeyPath(parent_path);

		if (tip_key === '') {
			var parent_type = typeOf(parent_object);
			if (parent_type !== 'array' && parent_type !== 'undefined') {
				throw new Error("Could not set '"+value+"' to "+key_path+" : Item at key path "+parent_path+" is of type "+parent_type+" (must be of type Array)");
			}
			tip_key = parent_object === undefined ? 0 : parent_object.length;
			key_path += tip_key;
		}

		if (parent_object === undefined) {

			this._setValueForKeyPath(parent_path, isIntString(tip_key) ? [] : {});
		}
		keypath(_this._object).setValueForKeyPath(key_path, value);
	}

	this.save = function(indent, callback) {

		if (_this._file === undefined) {
			throw new Error("File not specified. Set file in constructor or by calling .file('my/path/myfile.json')")
		}

		if (indent && indent !== 'auto') {
			this._indent = parseInt(indent);
		}

		var dump = function () {
			return JSON.stringify(_this._object, null, _this.indent()) + '\n';
		}

		if (typeof callback !== 'function') {
			fs.writeFileSync(_this._file, dump());
			return this;
		}

		_this._read_queue.push(function(read_callback) {
			if (typeof read_callback !== 'function') {
				read_callback = function() {};
			}
			fs.writeFile(_this._file, dump(), function() {
				read_callback();
				callback();
			});
		});

		if ( ! _this._wait_for_read) {
			_this._exec_read_queue();
		}
		return this;
	}

	this.delete = function(key_path) {
		_this._readJsonSync();
		_this._deleteValueForKeyPath(key_path);
		return this;
	}

	this._deleteValueForKeyPath = function(key_path) {
		var key_array = key_path.split('.');
		if (key_array.length > 1) {
			var top = key_array.pop(key_array);
			if (parseInt(top) == top) {
				top = parseInt(top);
			}
			var base = key_array.join('.');
			var part = keypath(_this._object).valueForKeyPath(base);
			delete part[top];
			keypath(_this._object).setValueForKeyPath(base, part);
		}
		else {
			delete _this._object[key_path];
		}
	}

	if (file !== undefined) {
		this.file(file);
	}

}
