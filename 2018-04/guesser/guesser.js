/*

  2017-11 cracker.c
  Test on Windows 8.1 Pro 32-bit (and compiled with gcc-4.9.2),
  I think that after a certain number of wrong attempts (~510), the guessme's
  output ("err") gets cached, so even if we come across the correct guess,
  the result will still be "err".

  2018-04-04 guesser.js
  translated cracker.c to JavaScript. The nodejs works like fine though...
  
*/

const exec = require("child_process").execSync;

function generateStringFromNumber(n) {
	// as if converting from base 10 to base X
	const CHARACTERS = "_abcdefghijklmnopqrstuvwxyz";
	const BASE = CHARACTERS.length;
	let result = "";
	do {
		result += CHARACTERS[n % BASE];
		n = Math.floor(n/BASE);
	} while (n > 0);
	return result;
}

function isCorrect(guess) {
	const cmd = "node guessme " + guess;
	const out = exec(cmd).toString().trim(); // "err" or "hey"
	return out === "hey";
}

function main(){
	let n = 0;
	let guess = "";
	
	console.log("Guessing...\n");
	while(1) {
		guess = generateStringFromNumber(n++);
		console.log(n + " : " + guess);
		if (isCorrect(guess)) {
			console.log("FOUND IT!!\n\n<< %s >>", guess);
			break;
		} 
	}
	
	return 0;
}

main();
