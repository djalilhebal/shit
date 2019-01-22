let writeFileSync, readFileSync, execSync; // These are imported by `init()`
const $input = document.querySelector('#input');
let $output  = document.querySelector('#output'); // it will be replaced!
const $run   = document.querySelector('#run');

const keywords = // incomplete
 `segment ends proc endp macro endm end
  assume stack offset SEG PTR far near dup
  db byte word dword para page`.replace(/\s+/g, ' ').trim().split(' ');
const operators = // These are the ones I know and/or use, there exist others..
 `mov int lea hlt call ret retn retf iret nop
  add sub mul div inc dec imul idiv aaa aas aam aad
  loop test cmp jmp jz jnz jc js je jne jg jl jge jle JCXZ JCXNZ
  and or xor not shr shl
  push pop`.replace(/\s+/g, ' ').trim().split(' ');
const registers =  // General-purpose~ + segment~ + other registers
 `AX AH AL DX DH DL CX CH CL BX BH BL BP SI DI SP
  CS DS SS ES
  Flags IP`.replace(/\s+/g, ' ').trim().split(' ');

const rOperator = RegExp('\\b(' + operators.join('|') + ')\\b','ig');
const rKeyword  = RegExp('\\b(' + keywords.join('|') + ')\\b','ig');
const rRegister = RegExp('\\b(' + registers.join('|') + ')\\b','ig');
const rNumber   = /\b(\d+(\d*[A-F]*)*\.?(\d*[A-F]*)*[BHOQTY]?)\b/ig;
const rString   = /((['"]).+?(\2))/g;
const rComment  = /&[a-z]{2,3};|(;.*)/g; /// TODO: Shouldn't match escaped HTML

function /** string */ highlightLine(/** string */ line, /** number */ index) {
  const content = line // First escape HTML entities then do the highlighting..
    .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    .replace(rString,   '<span class="string">$&</span>')
    .replace(rComment,  '<span class="comment">$&</span>')
    .replace(rOperator, '<span class="operator">$&</span>')
    .replace(rKeyword,  '<span class="keyword">$&</span>')
    .replace(rRegister, '<span class="register">$&</span>')
    .replace(rNumber,   '<span class="number">$&</span>');
  return `<span id="line${index+1}">${content}</span><br>`;
}

const fixScroll = () => $output.scrollTop = $input.scrollTop;
const removeClass = $x => $x.removeAttribute('class');

function draw() {
  const $pre = document.createElement('pre');
  $pre.setAttribute('id', 'output');
  $pre.innerHTML = $input.value.split('\n').map( highlightLine ).join('');
  // Attempting to fix nested highlights
  /// TODO Fix: `"string;" ; should be a comment`
  $pre.querySelectorAll('.string [class], .comment [class]').forEach(removeClass);
  $output.replaceWith($pre);
  $output = $pre;
  fixScroll();
}

function run() {
  // - Save the code ($input.value) in 'PROGRAM.ASM'
  // - Using DOSBox, call GO.BAT to assemble, link, and execute it.
  // - Parse MASM's output (errors and warnings) and show them in the editor.
  writeFileSync('PROGRAM.ASM', $input.value);
  execSync('DOSBox.exe GO.BAT -noconsole');
  const log = readFileSync('PROGRAM.LOG', 'utf-8');
  const messages = parseMASMOutput(log);
  messages.forEach(showMessage);
}

function showMessage(/** Object */ msg) {
  // Just set the class and title, CSS will take it from there...
  const $line = document.querySelector('#line' + msg.line);
  $line.setAttribute('class', msg.type);
  $line.setAttribute('title', msg.message);
}

function /** Object[] */ parseMASMOutput(/** string */ log) {
  // yo.asm(18): error A2009: Symbol not defined: KH
  // X.ASM(28): warning A4031: Operand types must match
  // (filename).asm(lineNum): (type:error|warning) (code): (message)
  const rLine = /^(\w+)\.asm\((\d+)\): (error|warning) (.+?): (.+)$/i;
  return log.split('\r\n').filter( line => rLine.test(line)).map((line) => {
    const [, filename, lineNum, type, code, message] = line.match(rLine);
    return { type, message, line: Number(lineNum) };
  });
}

function init() {
  $input.onscroll = fixScroll;
  draw();
  if (typeof nw === 'object' && typeof nw.require === 'function') {
    // We're in nwjs. We can 'Run!'
    readFileSync = require('fs').readFileSync;
    writeFileSync = require('fs').writeFileSync;
    execSync = require('child_process').execSync;
    $run.onclick = run;
    $run.hidden = false;
  }
}
