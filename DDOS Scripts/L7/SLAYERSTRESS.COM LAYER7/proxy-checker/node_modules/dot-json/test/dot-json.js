var expect = require('chai').expect;
var DotJson = require('../lib/dot-json.js');
var fs = require('fs');

var dir = 'test-files';

try {
	fs.statSync(dir);
}
catch(e) {
	fs.mkdirSync(dir);
}

fs.writeFileSync(dir+'/read-test.json', JSON.stringify(
	{
		a_string: "this is a string",
		"dotted.key": "a string",
		a_number: 42,
		an_object: {
			key_1: "string-one",
			key_2: "string-two",
			key_3: "string-three"
		},
		an_array: [
			"array-item-one",
			"array-item-two",
			"array-item-three"
		]
	}
,null, '  '));

describe("dot-json", function() {

	describe("#file()", function() {

		it("should be able to give a relative file", function() {
			var dot_json = new DotJson("./non-existing-file.json");
			expect(dot_json._file).equal(process.cwd()+"/non-existing-file.json");
		});

		it("should be able to give an absolute file", function() {
			var dot_json = new DotJson(process.cwd()+"/non-existing-file.json");
			expect(dot_json._file).equal(process.cwd()+"/non-existing-file.json");
		});

	});

	describe("#_readJson()", function() {

		it("should error when a non existing file is read", function() {
			var dot_json = new DotJson("non-existing-file.json");
			expect(dot_json._readJson).to.throw();
		});

		it("should be able to read json", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			var object = {
				a_string: "this is a string",
				"dotted.key": "a string",
				a_number: 42,
				an_object: {
					key_1: "string-one",
					key_2: "string-two",
					key_3: "string-three"
				},
				an_array: [
					"array-item-one",
					"array-item-two",
					"array-item-three"
				]
			};
			dot_json._readJson(function() {
				expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(object));
				done();
			});
		});

		it("should cache a json files content", function(done) {
			fs.writeFileSync(dir+'/cache-test.json', JSON.stringify(
				{
					cache_string: "this is a cache string"
				}
			,null, '  '));
			var dot_json = new DotJson("test-files/cache-test.json");
			expect(dot_json._file).equal(process.cwd()+"/test-files/cache-test.json");
			dot_json._readJson(function() {
				dot_json._object.cache_string = "this string is to simulate caching NfrQf4RWq08vlYgV";
				dot_json._readJson(function() {
					expect(dot_json._object.cache_string).equal("this string is to simulate caching NfrQf4RWq08vlYgV");
					done();
				}, true);
			}, true);
		});

		it("should update cached a json file if changed", function(done) {
			fs.writeFileSync(dir+'/cache-update-test.json', JSON.stringify(
				{
					cache_string: "this is a cache string",
					date: "initial string"
				}
			,null, '  '));

			var dot_json = new DotJson("test-files/cache-update-test.json");
			dot_json._readJson(function() {
				var new_object = JSON.parse(JSON.stringify(dot_json._object));
				new_object.date = new Date().toString();
				// Mimic the modification of a json file
				fs.writeFile("test-files/cache-update-test.json", JSON.stringify(new_object, null, '  '), function() {
					dot_json._readJson(function() {
						expect(new_object.date).equal(dot_json._object.date);
						done();
					});
				});
			});
		});

	});

	describe("#_readJsonSync()", function() {

		it("should error when a non existing file is read", function() {
			var dot_json = new DotJson("non-existing-file-sync-7n2nluaujiehedy8.json");
			expect(dot_json._readJsonSync).to.throw();
		});

		it("should be able to read json", function() {
			fs.writeFileSync(dir+'/read-test-sync.json', JSON.stringify(
				{
					a_string: "this is a string",
					"dotted.key": "a string",
					a_number: 42,
					an_object: {
						key_1: "string-one",
						key_2: "string-two",
						key_3: "string-three"
					},
					an_array: [
						"array-item-one",
						"array-item-two",
						"array-item-three"
					]
				}
			,null, '  '));

			var dot_json = new DotJson("test-files/read-test-sync.json");
			var object = {
				a_string: "this is a string",
				"dotted.key": "a string",
				a_number: 42,
				an_object: {
					key_1: "string-one",
					key_2: "string-two",
					key_3: "string-three"
				},
				an_array: [
					"array-item-one",
					"array-item-two",
					"array-item-three"
				]
			};
			dot_json._readJsonSync();
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(object));
		});

		it("should cache a json files content", function() {
			fs.writeFileSync(dir+'/cache-test-sync.json', JSON.stringify(
				{
					cache_string: "this is a cache string"
				}
			,null, '  '));
			var dot_json = new DotJson("test-files/cache-test-sync.json");
			expect(dot_json._file).equal(process.cwd()+"/test-files/cache-test-sync.json");
			dot_json._readJsonSync();
			dot_json._object.cache_string = "this string is to simulate caching 9db392-{2g9CQn^+";
			dot_json._readJsonSync();
			expect(dot_json._object.cache_string).equal("this string is to simulate caching 9db392-{2g9CQn^+");
		});

		it("should update cached a json file if changed", function() {
			fs.writeFileSync(dir+'/cache-update-test-sync.json', JSON.stringify(
				{
					cache_string: "this is a cache string",
					date: "initial string"
				}
			,null, '  '));

			var dot_json = new DotJson("test-files/cache-update-test-sync.json");
			dot_json._readJsonSync();
			var string_object = JSON.stringify(dot_json._object);
			var new_object = JSON.parse(string_object);
			new_object.date = new Date().toString();
			// Mimic the modification of a json file
			fs.writeFile("test-files/cache-test-sync.json", JSON.stringify(new_object, null, '  '), function() {
				dot_json._readJsonSync();
				expect(new_object.date).equal(dot_json._object.date);
			});
		});

	});

	describe("#get()", function() {

		it("should return the value from file on get method", function() {
			var dot_json = new DotJson("test-files/read-test.json");
			var value = dot_json.get("a_string");
			expect(value).equal("this is a string");
		});

		it("should return the value from file on get method with callback", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("a_string", function(value) {
				expect(value).equal("this is a string");
				done();
			});

		});

		it("should be able to get the whole object when only a callback function is given", function() {
			var dot_json = new DotJson("test-files/read-test.json");
			var object = dot_json.get();
			expect(JSON.stringify(object)).equal(JSON.stringify(
				{
					a_string: "this is a string",
					"dotted.key": "a string",
					a_number: 42,
					an_object: {
						key_1: "string-one",
						key_2: "string-two",
						key_3: "string-three"
					},
					an_array: [
						"array-item-one",
						"array-item-two",
						"array-item-three"
					]
				}
			));
		});

		it("should be able to get a json string value", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("a_string", function(value) {
				expect(value).equal("this is a string");
				done();
			});
		});

		it("should be able to get a json string value", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("dotted..key", function(value) {
				expect(value).equal("a string");
				done();
			});
		});

		it("should be able to get a json number value", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("a_number", function(value) {
				expect(value).equal(42);
				done();
			});
		});

		it("should be able to get a json object value", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("an_object", function(value_object) {
				var object = {
					key_1: "string-one",
					key_2: "string-two",
					key_3: "string-three"
				};
				expect(JSON.stringify(value_object)).equal(JSON.stringify(object));
				done();
			});
		});

		it("should be able to get a json array value", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("an_array", function(value_object) {
				var object = [
					"array-item-one",
					"array-item-two",
					"array-item-three"
				];
				expect(JSON.stringify(value_object)).equal(JSON.stringify(object));
				done();
			});
		});

		it("should be able to get a json value by key path", function(done) {
			var dot_json = new DotJson("test-files/read-test.json");
			dot_json.get("an_object.key_2", function(value) {
				expect(value).equal("string-two");
				done();
			});
		});

	});

	describe("#set()", function() {

		it("should throw error when file not set", function() {
			var dot_json = new DotJson();
			dot_json.set('my_path', "string");
			expect(dot_json.save).to.throw("File not specified. Set file in constructor or by calling .file('my/path/myfile.json')");
		});

		it("should be able to set a property on object", function() {
			var dot_json = new DotJson();
			dot_json.set('hello', "world");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify({hello: "world"}));
		});

		it("should be able to set a dotted property on object", function() {
			var dot_json = new DotJson();
			dot_json.set("foo..bar", "baz");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify({"foo.bar": "baz"}));
		});

		it("should be able to set a property on a prepared key path", function() {
			var dot_json = new DotJson();

			dot_json._object = {
				my: {
					keypath: "this must be replaced"
				}
			}

			dot_json.set('my.keypath', "is working");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						keypath: "is working"
					}
				}
			));
		});

		it("should be able to set a property on a key path", function() {
			var dot_json = new DotJson();
			dot_json.set('my.keypath', "is working");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						keypath: "is working"
					}
				}
			));
		});

		it("should be able to give a not existing file and set a property on a key path", function() {
			var dot_json = new DotJson("test-files/does-no-exist.json");
			dot_json.set('my.keypath', "is working");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						keypath: "is working"
					}
				}
			));
		});

		it("should be able to give an existing file and set a property on a key path", function() {
			fs.writeFileSync(dir+'/set-test.json', JSON.stringify(
				{
					prepared: "prepared string"
				}
			,null, '  '));
			var dot_json = new DotJson("test-files/set-test.json");
			dot_json.set('my.keypath', "is working");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					prepared: "prepared string",
					my: {
						keypath: "is working"
					}
				}
			));
		});

		it("should be able to set a property in an array on the 0 index", function() {
			var dot_json = new DotJson();
			dot_json.set('deep.array.0', "test");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					deep: {
						array: [
							"test"
						]
					}
				}
			));
		});

		it("should be able to replace a property in an array on a 1 index", function() {
			var dot_json = new DotJson();
			dot_json._object = {
				deep: {
					array: [
						"existing value one",
						"existing value two"
					]
				}
			}
			dot_json.set('deep.array.1', "test");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					deep: {
						array: [
							"existing value one",
							"test"
						]
					}
				}
			));
		});

		it("should be able to set a property in an array on a 1 index", function() {
			var dot_json = new DotJson();
			dot_json._object = {
				deep: {
					array: [
						"existing value"
					]
				}
			}
			dot_json.set('deep.array.1', "test");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					deep: {
						array: [
							"existing value",
							"test"
						]
					}
				}
			));
		});

		it("should be able to set a property in an existing array on a 1 index", function() {
			var dot_json = new DotJson();
			dot_json.set('deep.array.1', "test");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					deep: {
						array: [
							null,
							"test"
						]
					}
				}
			));
		});

		it("should be able to set a property on the first item in an array", function() {
			var dot_json = new DotJson();
			dot_json.set('ob1.array.', "this is number one");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					ob1: {
						array: [
							"this is number one"
						]
					}
				}
			));
		});

		it("should throw error when trying to add an array item to an object", function() {
			var dot_json = new DotJson();
			dot_json._object = {
				ob2: {
					array: [
						"first item"
					]
				}
			}
			expect(function() {
				dot_json.set('ob2.', "second");
			}).to.throw("Could not set 'second' to ob2. : Item at key path ob2 is of type object (must be of type Array)");
		});

		it("should be able to add a property to an existing array on the next index", function() {
			var dot_json = new DotJson();
			dot_json._object = {
				ob3: [
					4
				]
			}
			dot_json.set('ob3.', "second");
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					ob3: [
						4,
						"second"
					]
				}
			));

		});

		it("should be able to set an item far into an array", function() {
			var dot_json = new DotJson();
			dot_json._object = {
				ob4: [
					42
				]
			}
			dot_json.set('ob4.3.', "fourth");

			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					ob4: [
						42,
						null,
						null,
						[
							"fourth"
						]
					]
				}
			));
		});

		it("should be able to set an object", function() {
			var dot_json = new DotJson();
			dot_json.set('my.path', {object: "i am an object"});
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						path: {
							object: "i am an object"
						}
					}
				}
			));
		});

		it("should be able to set a int", function() {
			var dot_json = new DotJson();
			dot_json.set('my.int', 42);
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						int: 42
					}
				}
			));
		});

		it("should be able to set a int string", function() {
			var dot_json = new DotJson();
			dot_json.set('my.string_int', '42');
			expect(JSON.stringify(dot_json._object)).equal(JSON.stringify(
				{
					my: {
						string_int: '42'
					}
				}
			));
		});

		it("should be able to set file just before saving", function(done) {
			var file = "test-files/dot-file-test.json";
			var dot_json = new DotJson();
			dot_json.set('my.path', "my_string").file(file).save(2, function() {
				fs.readFile(file, function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							my: {
								path: "my_string"
							}
						}
					));
					done();
				});
			});
		});

		it("should be able to set a property in a non existing file", function(done) {
			var file = "test-files/write-new-file-test.json";
			var test = function() {
				var dot_json = new DotJson(file);
				fs.stat(file, function(err, stat) {
					expect(err).not.equal(null);
					dot_json.set('write_test_string', 'testing new file string').save(2, function() {
						fs.stat(file, function(err, stat) {
							expect(err).equal(null);
							fs.unlink(file, function() {
								done();
							});
						});
					});
				});
			}

			fs.stat(file, function(err) {
				if(err) {
					test();
					return;
				}
				fs.unlink(file, function() {
					test();
				});
			});
		});

		it("should be able to set a property and write to an existing file", function(done) {
			fs.writeFileSync(dir+'/set-get-existing-test.json', JSON.stringify(
				{
					set_get_string: "date string"
				}
			,null, '  '));

			var file = "test-files/set-get-existing-test.json";
			var dot_json = new DotJson(file);
			var date_string = new Date().toString();
			dot_json.set('set_get_string', date_string).save(2, function() {
				fs.readFile(file, function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							set_get_string: date_string
						}
					));
					done();
				});
			});
		});

		it("should be able to set a property and write to not existing file", function(done) {
			try {
				fs.unlinkSync(dir+'/set-get-new-file-test.json');
			}
			catch(e) {

			}

			var file = "test-files/set-get-new-file-test.json";
			var dot_json = new DotJson(file);
			var date_string = new Date().toString();
			dot_json.set('set_get_string', date_string).save(2, function() {
				fs.readFile(file, function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							set_get_string: date_string
						}
					));
					done();
				});
			});
		});

		it("should be able to set a property and write to file and read the updated json", function(done) {

			fs.writeFileSync(dir+'/set-get-updated-test.json', JSON.stringify(
				{
					set_get_string: "date string"
				}
			,null, '  '));

			var file = "test-files/set-get-updated-test.json";
			var dot_json = new DotJson(file);
			var date_string = new Date().toString();
			dot_json.set('set_get_string', date_string).save(2, function() {
				var dot_json_written = new DotJson(file);
				expect(dot_json_written._object).equal(undefined);
				dot_json_written.get('set_get_string', function(value) {
					expect(value).equal(date_string);
				 	done();
				});
			});
		});

		it("should be able to use get and set multiple times", function(done) {

			fs.writeFileSync(dir+'/get-set-test.json', JSON.stringify(
				{
					get_set_string: "date string"
				}
			,null, '  '));

			var dot_json = new DotJson("test-files/get-set-test.json");
			var date_string = new Date().toString();
			dot_json.get('get_set_string', function(v) {
				dot_json.set('get_set_string', date_string).save(2, function() {
					dot_json.get('get_set_string', function(value) {
						expect(value).equal(date_string);
						done();
					});
				});
			});
		});

		it("should be able to set and get a property by deep key path", function(done) {
			var dot_json = new DotJson("test-files/set-deep-test.json");
			var date_string = new Date().toString();
			dot_json.set('deep.string.item', date_string).save(2, function() {
				dot_json.get('deep.string.item', function(value) {
					expect(value).equal(date_string);
					done();
				});
			});
		});

		it("should be able to detect 3 space indents", function() {
			fs.writeFileSync(dir+'/auto-space-indent-test.json', JSON.stringify(
				{
					user: {}
				}
			,null, 3));

			var myfile = new DotJson(dir+'/auto-space-indent-test.json');
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com');
			myfile.save('auto')
			var content = fs.readFileSync(dir+'/auto-space-indent-test.json', 'utf8');
			expect(myfile.indent()).equal('   ')
			expect(content).equal('{\n   \"user\": {\n      \"name\": \"John Doe\",\n      \"email\": \"john@example.com\"\n   }\n}\n');
		});

		it("should be able to detect tab indents", function() {
			fs.writeFileSync(dir+'/auto-tab-indent-test.json', JSON.stringify(
				{
					user: {}
				}
			,null, '\t'));

			var myfile = new DotJson(dir+'/auto-tab-indent-test.json');
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com');
			myfile.save('auto')
			var content = fs.readFileSync(dir+'/auto-tab-indent-test.json', 'utf8');
			expect(myfile.indent()).equal('\t')
			expect(content).equal('{\n\t\"user\": {\n\t\t\"name\": \"John Doe\",\n\t\t\"email\": \"john@example.com\"\n\t}\n}\n');
		});

		it("should be use 2 spaces indents by default when creating new file", function() {
			var myfile = new DotJson(dir+'/new-2-space-indent-test.json');
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com');
			myfile.save('auto')
			var content = fs.readFileSync(dir+'/new-2-space-indent-test.json', 'utf8');
			expect(myfile.indent()).equal('  ')
			expect(content).equal('{\n  \"user\": {\n    \"name\": \"John Doe\",\n    \"email\": \"john@example.com\"\n  }\n}\n');
		});

	});

	describe("#delete()", function() {

		it("should be able to delete a property from a file by key path", function(done) {
			var file = "test-files/delete-test.json";
			var dot_json = new DotJson(file);
			fs.writeFile(file, JSON.stringify({delete_string: "this string is going to be deleted"}), function() {
				dot_json.delete('delete_string').save(2, function() {
					fs.readFile(file, function(err, content) {
						expect(JSON.stringify(JSON.parse(content))).equal('{}');
						done();
					});
				});
			});
		});

		it("should be able to delete a property from a file by deep key path", function(done) {
			var file = "test-files/delete-deep-test.json";
			var dot_json = new DotJson(file);

			var object = {
				object_item: {
					deeper_object_item: {
						very_deep_item_to_delete: "this is deep and will be deleted",
						neighbour_item: "this should stay"
					}
				}
			};
			var result_object = {
				object_item: {
					deeper_object_item: {
						neighbour_item: "this should stay"
					}
				}
			};
			fs.writeFile(file, JSON.stringify(object, null, '  '), function() {
				dot_json.delete('object_item.deeper_object_item.very_deep_item_to_delete').save(2, function() {
					fs.readFile(file, function(err, content) {
						expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(result_object));
						done();
					});
				});
			});
		});

	});

	describe("readme examples", function() {

		it("should be able to execute the write example", function(done) {
			try {
				fs.unlinkSync(dir+'/myfile-write.json');
			}
			catch(e) {

			}
			var myfile = new DotJson("test-files/myfile-write.json");
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com').save(2, function() {
			  	fs.readFile("test-files/myfile-write.json", function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							user: {
								name: "John Doe",
								email: "john@example.com"
							}
						}
					));
					done();
				});
			});
		});

		it("should be able to execute the read example", function(done) {
			fs.writeFileSync(dir+'/myfile-read.json', JSON.stringify(
				{
					user: {
						name: "John Doe",
						email: "john@example.com"
					}
				}
			,null, '  '));

			var myfile = new DotJson("test-files/myfile-read.json");
			myfile.get('user.name', function(value) {
				expect(value).equal("John Doe");
				done();
			});
		});

		it("should be able to execute the delete example", function(done) {
			fs.writeFileSync(dir+'/myfile-delete.json', JSON.stringify(
				{
					user: {
						name: "John Doe",
						email: "john@example.com"
					}
				}
			,null, '  '));

			var myfile = new DotJson("test-files/myfile-delete.json");
			myfile.delete('user.name').save(2, function() {
				fs.readFile("test-files/myfile-delete.json", function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							user: {
								email: "john@example.com"
							}
						}
					));
					done();
				});
			});
		});

		it("should be able to execute the full example", function(done) {
			try {
				fs.unlinkSync(dir+'/myfile.json');
			}
			catch(e) {

			}
			var myfile = new DotJson("test-files/myfile.json");
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com');
			myfile.save(2, function() {
			  	fs.readFile("test-files/myfile.json", function(err, content) {
					expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
						{
							user: {
								name: "John Doe",
								email: "john@example.com"
							}
						}
					));
					myfile.get('user.name', function(value) {
						expect(value).equal("John Doe");
						myfile.delete('user.name').save(2, function() {
							fs.readFile("test-files/myfile.json", function(err, content) {
								expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
									{
										user: {
											email: "john@example.com"
										}
									}
								));
								done();
							});
						});
					});
				});
			});
		});

		it("should be able to execute the write example synchronous", function(done) {
			try {
				fs.unlinkSync(dir+'/myfile-write-sync.json');
			}
			catch(e) {

			}
			var myfile = new DotJson("test-files/myfile-write-sync.json");
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com').save(2);
		  	fs.readFile("test-files/myfile-write-sync.json", function(err, content) {
				expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
					{
						user: {
							name: "John Doe",
							email: "john@example.com"
						}
					}
				));
				done();
			});
		});

		it("should be able to execute the read example synchronous", function() {
			fs.writeFileSync(dir+'/myfile-read-sync.json', JSON.stringify(
				{
					user: {
						name: "John Doe",
						email: "john@example.com"
					}
				}
			,null, '  '));

			var myfile = new DotJson("test-files/myfile-read-sync.json");
			var value = myfile.get('user.name');
			expect(value).equal("John Doe")
		});

		it("should be able to execute the delete example synchronous", function(done) {
			fs.writeFileSync(dir+'/myfile-delete-sync.json', JSON.stringify(
				{
					user: {
						name: "John Doe",
						email: "john@example.com"
					}
				}
			,null, '  '));

			var myfile = new DotJson("test-files/myfile-delete-sync.json");
			myfile.delete('user.name').save(2);
			fs.readFile("test-files/myfile-delete-sync.json", function(err, content) {
				expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
					{
						user: {
							email: "john@example.com"
						}
					}
				));
				done();
			});
		});

		it("should be able to execute the full example synchronous", function() {
			try {
				fs.unlinkSync(dir+'/myfile-sync.json');
			}
			catch(e) {

			}

			var myfile = new DotJson("test-files/myfile-sync.json");
			myfile.set('user.name', 'John Doe').set('user.email', 'john@example.com');
			myfile.save(2);
		  	var content = fs.readFileSync("test-files/myfile-sync.json");
			expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
				{
					user: {
						name: "John Doe",
						email: "john@example.com"
					}
				}
			));

			var value = myfile.get('user.name');
			expect(value).equal("John Doe");

			myfile.delete('user.name').save(2);
			var content = fs.readFileSync("test-files/myfile-sync.json");
			expect(JSON.stringify(JSON.parse(content))).equal(JSON.stringify(
				{
					user: {
						email: "john@example.com"
					}
				}
			));
		});

	});

});
