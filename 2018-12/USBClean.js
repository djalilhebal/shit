/**
 * USBClean.js -- 2018-12-28
 * Removes the annoying shortcut virus from the computer and flash drives.
 * For simplicity, compile it with `pkg` and place it on the desktop.
 * Assumption: Only one "malevolent" WScript entry exists in the 'Run' key
 * Tested on Windows 10 x64 and Windows 7 x32
 */

const execSync = require('child_process').execSync;
const print = console.log;
const r = String.raw; // inspired by Python.
const KEY = r`HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`;

function /** string */ cmd(/** string */ command) {
  try {
    return execSync(command).toString();
  } catch(e) {
    print(`ERROR: ${command}`);
    return '';
  }
}

function /** string */ getRegistryValue() {
  /* From Hope's infected computer:
  `    Format Factory    REG_SZ    Cmd.exe /c start WScript.exe /e:VBScript.Encode [path to file~]`
  */
  let result = '';
  const output = cmd(`reg query ${KEY} /f wscript.exe`);
  output.trim().split('\r\n').forEach( line => {
    const rLine = /^    (.+)    (.+)    (.+)$/;
    const [matched, value, type, data] = line.match(rLine) || [];
    if (matched) result = value;
  });
  return result;
}

function /** Array<string> */ getUSBDrives() {
  const rX = /^([A-Z]):/; // RegExp that matches a driver's letter (X), ex: 'F'
  const output  = cmd('WMIC logicaldisk where drivetype=2 get DeviceId');
  const letters = output.split('\r\n')
      .filter(line => rX.test(line))
      .map(line => line.match(rX)[1]);
  return letters;
}

(function main() {
  print('Working...');
  print('Checking & Killing the virus...');
  const wscriptRunning = /^wscript\.exe/m.test(cmd('tasklist'));
  if (wscriptRunning) cmd('taskkill /IM wscript.exe /T /F');
  
  print('Removing its auto-run registry entry...');
  const value = getRegistryValue();
  if (value) cmd(`reg delete ${KEY} /v "${value}" /f`);

  print('Removing shortcuts and showing hidden files...');
  getUSBDrives().forEach( (X) => {
    print(`Cleaning ${X}...`);
    cmd(r`erase ${X}:\*.lnk /Q /F`); // Remove shortcuts
    cmd(r`attrib -H -R -S -A ${X}:\* /S /D`); // Show hidden files
    // Fix the mess I made: Restore this folder and file's original attributes
    cmd(r`attrib +S +H "${X}:\System Volume Information"`);
    cmd(r`attrib +A "${X}:\System Volume Information\IndexerVolumeGuid"`);
  });

  print('\n\n  DONE, IG');
  print('____________');
  
})();
