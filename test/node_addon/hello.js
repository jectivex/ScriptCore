
require('./build/Release/scriptcore-node-addon');
// using "bindings": "~1.2.1"
// require('bindings')('script-engine-node-addon');

console.log(`
Running test code for ScriptCore node.js addon

${ScriptCoreDemoTest}
property names: ${JSON.stringify(Object.getOwnPropertyNames(ScriptCoreDemoTest))}
hello ${ScriptCoreDemoTest.hello()}.

`);
