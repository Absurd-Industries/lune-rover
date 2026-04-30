#pragma once

static const char PAGE_WIFI[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>LUN-E SETUP</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#0A0A0A;color:#E8E8E8;font-family:'SF Mono',Consolas,monospace;
  display:flex;align-items:center;justify-content:center;min-height:100vh;padding:20px}
.box{width:100%;max-width:360px}
h1{font-size:14px;letter-spacing:0.2em;color:#FF6A00;margin-bottom:4px;font-weight:900}
h2{font-size:10px;letter-spacing:0.12em;margin-bottom:16px;color:#666}
label{display:block;font-size:9px;letter-spacing:0.12em;color:#666;
  text-transform:uppercase;margin-bottom:4px;margin-top:14px}
input{width:100%;background:#111;border:1px solid #333;color:#E8E8E8;
  padding:10px;font:11px/1 inherit;letter-spacing:0.05em}
input:focus{outline:none;border-color:#FF6A00}
button{margin-top:18px;width:100%;background:#FF6A00;color:#0A0A0A;border:none;
  padding:12px;font:11px/1 inherit;letter-spacing:0.15em;text-transform:uppercase;
  cursor:pointer;font-weight:700}
button:hover{background:#FF8C00}
.net{padding:8px 0;border-bottom:1px solid #1A1A1A;font-size:10px;cursor:pointer;color:#666}
.net:hover{color:#FF6A00}
.info{font-size:9px;color:#666;margin-top:18px;line-height:1.8}
.info b{color:#FF6A00}
#nets{margin-bottom:10px}
</style>
</head><body><div class="box">
<h1>LUN-E</h1>
<h2>NETWORK CONFIGURATION</h2>
<div id="nets">Scanning...</div>
<form method="POST" action="/wifi-save">
<label>SSID</label>
<input name="ssid" id="ssid" required autocomplete="off">
<label>PASSWORD</label>
<input name="pass" type="password" required>
<button type="submit">CONNECT TO NETWORK</button>
</form>
<div class="info">
After connecting, find the rover at <b>explorer.local</b><br>
ESP-NOW channel will sync to your router.
</div>
</div>
<script>
function doScan(){
  document.getElementById('nets').textContent='Scanning...';
  fetch('/scan').then(r=>r.json()).then(nets=>{
    const el=document.getElementById('nets');el.innerHTML='';
    nets.forEach(n=>{
      const d=document.createElement('div');d.className='net';
      d.textContent=n.ssid+' ('+n.rssi+'dBm CH'+n.ch+')';
      d.onclick=()=>document.getElementById('ssid').value=n.ssid;
      el.appendChild(d);});
    if(!nets.length)el.innerHTML='No networks. <span class="net" onclick="doScan()" style="display:inline;text-decoration:underline;cursor:pointer">Retry</span>';
    else{const r=document.createElement('div');r.className='net';r.style.color='#FF6A00';r.textContent='[RESCAN]';r.onclick=doScan;el.appendChild(r);}
  }).catch(()=>{document.getElementById('nets').innerHTML='Scan failed. <span class="net" onclick="doScan()" style="display:inline;text-decoration:underline;cursor:pointer">Retry</span>';});
}
setTimeout(doScan,500);
</script>
</body></html>
)rawliteral";

static const char PAGE_WIFI_SAVED[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>SAVED</title>
<style>
body{background:#0A0A0A;color:#E8E8E8;font-family:'SF Mono',Consolas,monospace;
  display:flex;align-items:center;justify-content:center;min-height:100vh}
.box{text-align:center;font-size:12px;letter-spacing:0.12em;line-height:2.2}
.o{color:#FF6A00;font-weight:700}
</style>
</head><body><div class="box">
<span class="o">CREDENTIALS SAVED</span><br>
REBOOTING SYSTEMS...<br><br>
FIND ROVER AT <span class="o">explorer.local</span>
</div></body></html>
)rawliteral";
