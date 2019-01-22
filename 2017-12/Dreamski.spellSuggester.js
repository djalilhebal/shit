var Dreamski = Dreamski || {};

Dreamski.spellSuggester = function (input = '', max = 5) {
  const replaceAt = (str, c, i) => str.slice(0, i) + c + str.slice(i + 1);

  const DICT = ['cat', 'drama', 'dream', 'hope', 'hell', 'tycoon', 'typhoon',
               'suppress', 'suspect', 'malice', 'wonderland'];

  const LETTERS = 'abcdefghijklmnopqrstuvwxyz'.split('');
  const VOWELS = 'aeiouy'.split('');

  // ================

  function swap(word) {
    // swap('hoep') => ['ohep', 'heop', 'hope']
    let candidates = [];
    for (let i = 0; i < word.length - 1; i++) {
      let tmp = word.split('');
      const t = tmp[i];
      tmp[i] = tmp[i + 1];
      tmp[i + 1] = t;
      tmp = tmp.join('');
      candidates.push(tmp);
    }
    return candidates;
  }

  function repeat(word) {
    // repeat('hel') => ['hhel', 'heel', 'hell']
    let candidates = [];
    for (let i = 0; i < word.length; i++) {
      candidates.push(replaceAt(word, word[i] + word[i], i));
    }
    return candidates;
  }

  function unrepeat(word) {
    // unrepeat('ccat') => ['cat']
    let candidates = [];
    word.replace(/([a-z])\1/g, (s, p, i) => {
      candidates.push(replaceAt(word, '', i));
    });
    return candidates;
  }

  function alterVowels(word) {
    // alterVowels('toll') => ['tall', 'tell', 'till', 'toll', 'tull', 'tyll']
    let candidates = [];
    word.replace(/([a|e|i|o|u|y])/g, (s, p, i) => {
      VOWELS.forEach((v) => {
        candidates.push(replaceAt(word, v, i));
      });
    });
    return candidates;
  }

  function substitute(word) {
    // substitute('kame') => ['aame', 'bame", 'came', 'dame', ..., 'kamz']
    let candidates = [];
    for (let i = 0; i < word.length; i++) {
      LETTERS.forEach(L => candidates.push(replaceAt(word, L, i)));
    }
    return candidates;
  }

  function insert(word) {
    // insert('alice') => ['aalice', 'balice', ..., 'malice', ..., "alicez"]
    let candidates = [];
    for (let i = 0; i < word.length; i++) {
      LETTERS.forEach(L => candidates.push(replaceAt(word, L + word[i], i)));
    }
    // to add 'letters' at the end of the word
    LETTERS.forEach(L => candidates.push(word + L));
    return candidates;
  }

  function remove(word) {
    // substitute('arte') => ['rte', 'ate', 'are', 'art']
    let candidates = [];
    for (let i = 0; i < word.length; i++) {
      candidates.push(replaceAt(word, '', i));
    }
    return candidates;
  }

  function filter(candidates) {
    // only keep words that exist in the DICT, with no duplication
    const finalCandidates = [];
    candidates.forEach((candidate) => {
      if (!finalCandidates.includes(candidate) && DICT.includes(candidate)) {
        finalCandidates.push(candidate);
      }
    });
    return finalCandidates;
  }

  function oneEdit(word) {
    return [].concat(
            swap(word),
            unrepeat(word),
            repeat(word),
            alterVowels(word),
            substitute(word),
            insert(word),
            remove(word),
        );
  }

  function swapEdit() {
    let candidates = [];
    _oneEdits.forEach(_oneEdit => candidates = candidates.concat(swap(_oneEdit)));
    return candidates;
  }

  function twoEdits() {
    let candidates = [];
    _oneEdits.forEach(_oneEdit => candidates = candidates.concat(oneEdit(_oneEdit)));
    return candidates;
  }

  const cleanedInput = (input || '').toLowerCase().trim();

  if (!cleanedInput) return [];

  const _oneEdits = [cleanedInput].concat(oneEdit(cleanedInput));
  const _allEdits = [].concat(
            _oneEdits,
            swapEdit(cleanedInput),
            twoEdits(cleanedInput),
            );

  return filter(_allEdits).slice(0, max);
};
