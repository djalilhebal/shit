/**
 * 2018-11 -- I googled my problem (my friend Abdou's, actually),
 * I found my exact question on Stack Overflow + a working solution in Python,
 * I simply wrote a shitty version in JavaScript...
 * 
 * https://stackoverflow.com/questions/50008296/facebook-json-badly-encoded
 * https://stackoverflow.com/a/50011987/9453525
 */
const {readFileSync, writeFileSync} = require('fs');
const original = readFileSync("Abdou's conversation.json");
const result = JSON.parse(original);
result
  .messages
  .forEach(m => m.content = Buffer.from(m.content, 'latin1').toString('utf8'));
writeFileSync('corrected.json', JSON.stringify(result), 'utf8');
