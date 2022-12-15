// https://www.npmjs.com/package/kociemba
const kociemba = require('kociemba');
let args = process.argv.slice(2);
let state = args[0].toUpperCase();
console.log(kociemba.solve(state));