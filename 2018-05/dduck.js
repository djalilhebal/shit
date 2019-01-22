/*
dduck.js -- a dull The Duck Comics downloader (2018-05-08)

Remember Hatem's Heaven? Well, this should be named Hatem's Hell :")
It sorta sucks:
- Experimental stuff: Promises, async/await, save/load
- it should save each comic in a separate folder, but it just saves everything in the cwd
- it should depend only on nodejs packages but it still relies on the CLI utility 'aria2c'
*/

const execSync = require('child_process').execSync;
const requestp = require('request-promise');
const cheerio = require('cheerio');
const fs = require('fs');

const print = console.log;
const exit = process.exit;

const file = 'info.completed.json';
const info = {
  title: 'Road_to_heaven',
  index: [],
  downloaded: [],
  failed: [],
};

function load() {
  try {
    const readed = fs.readFileSync(file, 'utf8');
    const parsed = JSON.parse(readed);
    return parsed;
  } catch (e) {
    return [];
  }
}

function save(arr) {
  try {
    const jsoned = JSON.stringify(arr);
    fs.writeFileSync(file, jsoned);
    return true;
  } catch (e) {
    return false;
  }
}

async function saveImg(id) {
  const link = `http://www.theduckwebcomics.com/${info.title}/${id}/`;
  const body = await requestp(link);
  const $ = cheerio.load(body);
  const src = $('meta')[2].attribs.content; // the img's src
  const ext = src.slice(src.lastIndexOf('.')); // extension (.jpg, .png, idk)
  const path = `${info.title}_${id}${ext}`; // title_id.ext
  // for some reason, this method produces corrupt files (?)
  // const data = await requestp(src);
  // fs.writeFileSync(path, data);
  // I'm too lazy to try to fix it so...
  execSync(`aria2c "${src}" -o ${path}`);
}

async function getIndex() {
  const link = `http://www.theduckwebcomics.com/${info.title}/`;
  const body = await requestp(link);
  const $ = cheerio.load(body);
  const options = $('#dropdownholder select option');
  const entries = options.map(option => options[option].attribs.value);
  return entries;
}

async function main() {
  if (process.argv.length !== 3) {
    print('dduck -- A dull The Duck Comics downloader');
    print('USAGE: node dduck <title>');
    print('EXAMPLE: node dduck Road_to_heaven');
    exit();
  }

  info.title = process.argv[2];

  print(`Comic: ${info.title}`);

  print('Getting index...');
  try {
    info.index = await getIndex();
  } catch (e) {
    print(e);
    print('ERR: Check your internet or the title of the comic or idk');
    exit();
  }

  info.completed = load();
  const queue = info.index.filter(id => !info.completed.includes(id));

  print('Downloading...');
  for (let i = 0; i < queue.length; i++) {
    const id = queue[i];
    const perc = i * 100 / queue.length;
    print(`${perc.toFixed(1)}%`);
    try {
      await saveImg(id);
      info.completed.push(id);
    } catch (e) {
      print(e);
      info.failed.push(id);
    }
  }

  print('Finishing...');
  save(info.completed);

  if (info.failed.length > 0) {
    print(`${info.failed.length} failed`);
    print(info.failed);
  }
}

main();
