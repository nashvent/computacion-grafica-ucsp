// Fridrich Method
// https://www.npmjs.com/package/rubiks-cube-solver
const solver = require('rubiks-cube-solver');
var args = process.argv.slice(2);
console.log(solver(args[0]));