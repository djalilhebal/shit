/**
 * "Proof-of-concept": A 5-line "DarjaTTS" -- 2018-12-20
 * Handles *my* spelling, except X stands for Ch (Charade), 4 for Gh, 5 for Kh.
 * Its pronunciation is shitty/robotic/monotonic but intelligible enough~~ xD
 * The wavs/ folder contains my low-quality/8-bit audio recordings of syllables
 * e.g. '_.wav' (short silence), 'a.wav', 'n.wav', 'na.wav', 'ni.wav', etc.
 */
const writeFile = require('fs').writeFileSync;
const exec = require('child_process').execSync;

const text = 'Ana raya7 za3ma ne9ra f 9santina';
const rSyllables = /([bcdfghjklmnpqrstvwxyz345679][aoiue])|(.)/g;
const syllables  = text.toLowerCase().replace(/ /g, '_').match(rSyllables);
writeFile('parts.txt', syllables.map(x => `file 'wavs/${x}.wav'`).join('\n'));
exec('ffmpeg -f concat -i parts.txt -y hadra.wav && ffplay hadra.wav');
