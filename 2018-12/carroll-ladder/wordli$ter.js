/**
 * @file WORDLI$TER: Mark ($) the words that appear in Nicki Minaj's lyrics
 * Note: '#lyrics' were downloaded from AZLyrics (as in, "it contains errors")
 * TODO: Remove 'aloof' words to reduce the file size and improve loading time
 */
const fs = require('fs');
const {LyricsDatabase, parseDir} = require('./@djalilhebal/lyrics-manager/');
const print = console.log;

(function main() {
  const lyricsDB = new LyricsDatabase();
  const nickiWords = new Set();
  const result = [];

  print('Parsing lyrics...');
  parseDir(lyricsDB, './songs-extra/#lyrics/*/*');
  lyricsDB
    .getMap()
    .get('Nicki Minaj')
    .forEach((album) => {
      album.forEach((song) => {
        song.lyrics.split(/\s+/).forEach((word) => {
          const cleaned = word.toLowerCase().replace(/[^a-z-]/g, '');
          nickiWords.add(cleaned);
          nickiWords.add(cleaned.replace(/in'?$/, 'ing')); // blazin' > blazing
        });
      });
    });

  print('Marking the worli$t...'); // The slowest phase
  fs.readFileSync('60k-wordlist.txt', 'utf8')
    .split('\n')
    .forEach((word) => {
      let w = word.replace(/[^a-z-]/g, '');
      if (w && !result.includes(w) && !result.includes(w+'$')) {
        if (nickiWords.has(w)) w += '$';
        result.push(w);
      }
    });

  print('Writing wordli$t.txt...');
  fs.writeFileSync('wordli$t.txt', result.join('\n'));

  print('Done.');
})();
