/**
 * USBKiss.js -- USB-triggered Kill Switch (2018-12-23)
 * Once the observed USB device gets removed, act like a good boy:
 * OPEN, COLORFUL, FAMOUS, and maybe SERIOUS.
 *
 * Tested using Windows 10 x64, Node v8.11.2, Chrome v71, VeraCrypt v1.23
 * Inspired by hephaest0s/usbkill (Read about it on wikipedia.org/wiki/USBKill)
 */

const execSync = require('child_process').execSync;
// I rather use `usb-detection` than handle\monitor\parse a command like this:
// `wmic logicaldisk where drivetype=2 get DeviceId, VolumeSerialNumber`
const usbDetect = require('usb-detection');
// To catch cmd errors (e.g. `taskkill`ing non-existent `chrome.exe` processes)
const cmd = (x) => { try { execSync(x) } catch (e) { console.log('Ugh', x) } };

function goodboyfy() {
  const SERIOUS = false; // more like PANIC!! \(>o<)/
  
  // OPEN. To be safe, first lock the screen
  cmd('rundll32.exe user32.dll, LockWorkStation');
  // COLORFUL. Unmount all 'colorless' volumes
  cmd('"C:\\\\Program Files\\VeraCrypt\\VeraCrypt.exe" /dismount /force /q');
  // FAMOUS. To close incognito tabs
  cmd('taskkill /F /T /IM chrome.exe');
  // SERIOUS? Reset Chrome (history, cookies, passwords, etc.) then shut down
  if (SERIOUS) {
    cmd('rmdir /S /Q "%LocalAppData%\\Google\\Chrome"');
    cmd('shutdown /p /f');
  }
}

(function main() {
  let kissing = null;
  usbDetect.startMonitoring();
  console.log('Waiting for a USB device...');
  usbDetect.on('add', (dev) => {
    if (!kissing) {
      console.log('Kissing', dev);
      kissing = dev;
      usbDetect.on(`remove:${dev.vendorId}:${dev.productId}`, (removed) => {
        if (removed.deviceAddress === kissing.deviceAddress) { // To be sure.
          console.log('Acting...');
          goodboyfy();
          usbDetect.stopMonitoring();
        }
      });
    }
  });
})();
