/**
 * TODOs:
 * - Use a WebWorker to make the UI non-blocking
 * - Use something like: fetch('./wordli$t.txt')
 * - Google Closure Compiler says this code is almost ~90% typed. Make it 100%!
 */

class Hip {
  constructor(/** function(?, ?):number */ fn) {
    this._arr = [];
    this._comparator = fn;
  }

  /** void */ push(/** ? */ obj) {
    //TODO: Add the new element as if you're doing an 'insertion sort'
    //TODO: Use Array.prototype.splice
    this._arr.push(obj);
    this._arr.sort(this._comparator);
  }

  /** ? */ pop() {
    return this._arr.pop();
  }

  /** boolean */ isEmpty() {
    return this._arr.length === 0;
  }

}

class PinkLadder {
  constructor(/** Array<string> */ words) {
    this.nickiSet = this.getNickiSet(words);
    //this.rareness = this.getRareness(words);
    this.map = this.getMap(words);
  }
  
  /** boolean */ acceptsWord(/** string */ word) {
    return this.map.has(word);
  }
  
  /** Array<string> */ getLadder(/** string */ x, /** string */ y) {
    const visited = new Set();
    const isNotVisited = (word) => !visited.has(word);
    const unvisitedNeighborsOf = (z) => this.map.get(z).filter(isNotVisited);

    const frontier = new Hip( (a, b) => b[0] - a[0] );
    frontier.push([0, x, [x]]);
    while (!frontier.isEmpty()) {
      const [oldCost, word, path] = frontier.pop();
      if (word === y) return path;
      for (const adjacent of unvisitedNeighborsOf(word)) {
        visited.add(adjacent);
        // Clone the `path` then appended the new word (`adjacent`)
        const appendedPath = Array.from(path); appendedPath.push(adjacent);
        let cost = oldCost + PinkLadder.distance(adjacent, y);
        if (this.isNickiWord(adjacent)) cost--;
        //cost += rareness.get(adjacent);
        frontier.push([cost, adjacent, appendedPath]);
      }
    }

    return [];
  }

  // The Hamming distance between a and b, or -1 if they aren't of equal length
  static /** number */ distance(/** string */ a, /** string */ b) {
    if (a.length !== b.length) return -1;

    let result = 0;
    for (let i = 0; i < a.length; i++) {
      if (a[i] !== b[i])
        result++;
    }
    
    return result;
  }

  /* Preprocesses words (removes '$') and returns a set of words used by Nicki */
  /** Set<string> */ getNickiSet(/** Array<string> */ words) {
    const nickiSet = new Set();
    words.forEach((word, i) => {
      if (word.endsWith('$')) {
        const _word = word.replace('$', '');
        nickiSet.add(_word);
        words[i] = _word;
      }
    });
    return nickiSet;
  }

  /** boolean */ isNickiWord(/** string */ w) {
    return this.nickiSet.has(w);
  }

  /** Map<string, number> */ getRareness(/** Array<string> */ words) {
    // Supposing `words` are sorted, most frequently used to least used...
    const rareness = new Map(); // "rareness" doesn't sound right
    words.forEach((word, i) => rareness.set(word, i/words.length));
    return rareness;
  }

  /** Map<string, Array<string>> */ getMap(/** Array<string> */ words) {
    const dict = new Map();
    words.forEach((key) => {
      words.forEach((word) => {
        if (PinkLadder.distance(key, word) === 1) {
          if (dict.has(key))
            dict.get(key).push(word);
          else
            dict.set(key, [word]);
        }
      });
    });
    
    return dict;
  }

}

(function /** void */ main() {
  const print = (msg) => document.querySelector('#output').innerHTML = msg;
  
  print('Preparing the wordli$t...');
  // `WORDLI$T` is declared globally
  const words = WORDLI$T.split('\n');
  const pink = new PinkLadder(words);
  print('Ready!');
  
  // Add event listener
  document.querySelector('button').onclick = function go() {
    const x = document.querySelector('#x').value;
    const y = document.querySelector('#y').value;

    // validate input...
    if (x === y)
      return print('The two words must be different');

    if (x.length !== y.length)
      return print('The two words must be of equal length');

    if (!pink.acceptsWord(x))
      return print(`"${x}" is not in the wordli$t`);

    if (!pink.acceptsWord(y))
      return print(`"${y}" is not in the wordli$t`);
    
    const result = pink.getLadder(x, y);
    if (result.length) {
      print(result.map(w => pink.isNickiWord(w) ? `<b>${w}</b>` : w).join(' > '));
    } else {
      print('No chain found :(');
    }
  };
})();
