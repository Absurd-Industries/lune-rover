#pragma once

static const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>LUN-E PILOT</title>
<style>
*{margin:0;padding:0;box-sizing:border-box;-webkit-touch-callout:none;-webkit-user-select:none;user-select:none}
:root{--o:#FF6A00;--bg:#0A0A0A;--fg:#E0E0E0;--dim:#666;--bdr:#333;
  --f:'SF Mono',SFMono-Regular,'IBM Plex Mono',Consolas,monospace}
html,body{width:100%;height:100%;overflow:hidden;background:var(--bg);color:var(--fg);font-family:var(--f)}

#feedWrap{display:flex;align-items:center;justify-content:center;width:100%;height:100%;position:absolute;
  top:0;left:0;z-index:0;background:#000}
#feed{display:block;max-width:100%;max-height:100%}
#feed.full{position:absolute;top:0;left:0;width:100%;height:100%;max-width:none;max-height:none;object-fit:cover}
canvas#cv{display:none;max-width:100%;max-height:100%}
canvas#cv.full{position:absolute;top:0;left:0;width:100%;height:100%;max-width:none;max-height:none;object-fit:cover}
.camoff #feed,.camoff canvas#cv{display:none!important}
#noStream{display:none;font-size:11px;color:var(--dim);letter-spacing:0.15em}
.camoff #noStream{display:block}

#top{position:absolute;top:0;left:0;right:0;z-index:20;display:flex;justify-content:space-between;
  align-items:center;padding:10px 14px;background:linear-gradient(rgba(10,10,10,0.9),transparent)}
.brand{font-size:14px;font-weight:700;letter-spacing:0.2em;color:var(--fg)}
.brand b{color:var(--o)}
#top .info{font-size:9px;letter-spacing:0.08em;color:var(--dim);text-align:right}
#top .info span{display:block;line-height:1.5}
#top .info .live{color:var(--o)}

#bar{position:absolute;top:44px;left:10px;right:10px;z-index:20;display:flex;flex-wrap:wrap;gap:4px;align-items:center}
.pill{font-size:8px;letter-spacing:0.1em;padding:4px 10px;border:1px solid var(--bdr);
  background:rgba(20,20,20,0.85);color:var(--dim);cursor:pointer;border-radius:3px}
.pill:hover{border-color:var(--o);color:var(--fg)}
.pill.on{border-color:var(--o);color:#fff;background:var(--o)}
.sep{width:1px;height:16px;background:var(--bdr);margin:0 2px}

#telem{position:absolute;bottom:60px;left:180px;z-index:15;font-size:9px;color:var(--dim);
  pointer-events:none;display:flex;gap:16px}
#telem .tg{display:flex;flex-direction:column;align-items:center}
#telem .tl{font-size:7px;letter-spacing:0.1em;margin-bottom:2px}
#telem .tv{color:var(--o);font-size:14px;font-weight:700}

@media(min-width:700px){
  #telem{bottom:auto;top:80px;left:auto;right:14px;flex-direction:column;gap:10px}
  #telem .tg{flex-direction:row;gap:8px}
  #telem .tl{margin-bottom:0;min-width:30px;text-align:right}
  #telem .tv{font-size:12px}
}

#joyWrap{position:absolute;bottom:24px;left:20px;z-index:30;touch-action:none}
#joy{width:130px;height:130px;border-radius:50%;border:2px solid var(--bdr);
  background:rgba(20,20,20,0.7);position:relative;touch-action:none}
#joyKnob{width:44px;height:44px;border-radius:50%;border:2px solid var(--o);
  background:rgba(255,106,0,0.15);position:absolute;top:50%;left:50%;
  transform:translate(-50%,-50%);pointer-events:none;
  box-shadow:0 0 12px rgba(255,106,0,0.2)}
#joyLabel{text-align:center;font-size:7px;letter-spacing:0.15em;color:var(--dim);margin-top:4px}

@media(min-width:700px){
  #joyWrap{bottom:40px;left:40px}
  #joy{width:160px;height:160px}
  #joyKnob{width:52px;height:52px}
}

#ctrl{position:absolute;bottom:24px;right:14px;z-index:30;display:flex;flex-direction:column;
  gap:6px;align-items:flex-end}
.cbtn{width:48px;height:48px;border-radius:50%;border:2px solid var(--bdr);
  background:rgba(20,20,20,0.7);display:flex;align-items:center;justify-content:center;
  font-size:7px;font-weight:700;letter-spacing:0.08em;color:var(--dim);cursor:pointer;font-family:var(--f)}
.cbtn:hover{border-color:var(--o);color:var(--fg)}
.cbtn.on{border-color:var(--o);color:#fff;background:var(--o);
  box-shadow:0 0 16px rgba(255,106,0,0.3)}
#speedRow{display:flex;gap:4px}
#speedRow .spd{padding:3px 7px;font-size:7px;letter-spacing:0.08em;border:1px solid var(--bdr);
  background:rgba(20,20,20,0.7);color:var(--dim);cursor:pointer;font-family:var(--f);border-radius:3px}
#speedRow .spd:hover{border-color:var(--o)}
#speedRow .spd.on{border-color:var(--o);color:var(--o);font-weight:700}

@media(min-width:700px){
  #ctrl{bottom:40px;right:40px}
  .cbtn{width:56px;height:56px;font-size:8px}
}

#logWrap{position:absolute;bottom:0;left:0;right:0;z-index:15;padding:4px 14px;
  background:linear-gradient(transparent,rgba(10,10,10,0.95));font-size:8px;line-height:1.5;
  color:var(--dim);max-height:42px;overflow:hidden;pointer-events:none}
.logln{white-space:nowrap}.logln .ts{color:var(--o);opacity:0.4;margin-right:6px}
.logln.hi{color:var(--o)}

#autonav{position:absolute;top:80px;left:10px;z-index:15;font-size:9px;
  color:var(--o);opacity:0;transition:opacity 0.3s;pointer-events:none}
#autonav.on{opacity:1}
#autonav .state{font-weight:700;font-size:11px;letter-spacing:0.15em}
#autonav .reason{color:var(--dim);font-size:8px;margin-top:2px}
#zoneViz{display:grid;grid-template-columns:repeat(5,18px);gap:1px;margin-top:4px}
.zc{width:18px;height:14px;font-size:7px;text-align:center;line-height:14px;
  background:rgba(255,106,0,0.04);color:var(--dim);border:1px solid rgba(255,106,0,0.15)}
#cliffWarn{color:#f44;font-size:8px;font-weight:700;margin-top:3px;letter-spacing:0.1em;display:none}
#cliffWarn.on{display:block}

#mapWrap{position:absolute;bottom:50px;left:50%;transform:translateX(-50%);z-index:18;
  display:none;pointer-events:none}
#mapWrap.on{display:block}
#mapCv{border:1px solid var(--bdr);image-rendering:pixelated}
#mapLabel{text-align:center;font-size:7px;color:var(--dim);letter-spacing:0.1em;margin-top:2px}
@media(min-width:700px){#mapWrap{bottom:auto;top:50%;left:10px;transform:translateY(-50%)}}

#wasdHint{position:absolute;bottom:8px;left:50%;transform:translateX(-50%);z-index:16;
  font-size:8px;color:var(--dim);letter-spacing:0.12em;pointer-events:none;display:none;opacity:0.5}
@media(hover:hover){#wasdHint{display:block}}
#wsStatus{position:absolute;top:12px;left:50%;transform:translateX(-50%);z-index:20;
  font-size:7px;letter-spacing:0.12em;color:var(--dim);pointer-events:none}
#wsStatus.ok{color:var(--o)}
</style>
</head>
<body>
<div id="feedWrap">
  <img id="feed" crossorigin="anonymous">
  <canvas id="cv"></canvas>
  <div id="noStream">STREAM OFF</div>
</div>
<div id="top">
  <span class="brand">LUN<b>-E</b></span>
  <div class="info">
    <span id="conn">CONNECTING...</span>
  </div>
</div>
<div id="wsStatus">WS ---</div>
<div id="bar">
  <button class="pill on" data-m="raw">RAW</button>
  <button class="pill" data-m="night">NVIS</button>
  <button class="pill" data-m="edges">EDGE</button>
  <button class="pill" data-m="therm">THRM</button>
  <button class="pill" data-m="motion">MOTN</button>
  <button class="pill" data-m="enhance">ENHANCE</button>
  <div class="sep"></div>
  <button class="pill" id="camBtn">CAM OFF</button>
  <button class="pill" id="expandBtn">FULL</button>
  <button class="pill" id="mapBtn">MAP</button>
</div>
<div id="telem">
  <div class="tg"><span class="tl">THR</span><span class="tv" id="tT">0</span></div>
  <div class="tg"><span class="tl">TRN</span><span class="tv" id="tS">0</span></div>
  <div class="tg"><span class="tl">PKT</span><span class="tv" id="tP">0</span></div>
  <div class="tg"><span class="tl">CH</span><span class="tv" id="tCh">--</span></div>
</div>
<div id="autonav">
  <div class="state" id="aState">IDLE</div>
  <div class="reason" id="aReason">--</div>
  <div id="cliffWarn">CLIFF!</div>
  <div id="zoneViz"></div>
</div>
<div id="mapWrap">
  <canvas id="mapCv" width="96" height="96"></canvas>
  <div id="mapLabel">MAP</div>
</div>
<div id="joyWrap">
  <div id="joy"><div id="joyKnob"></div></div>
  <div id="joyLabel">PILOT</div>
</div>
<div id="ctrl">
  <div id="speedRow">
    <button class="spd" data-s="25">SLO</button>
    <button class="spd on" data-s="55">MED</button>
    <button class="spd" data-s="100">MAX</button>
  </div>
  <div class="cbtn" id="autoBtn" style="opacity:0.3;cursor:default">AUTO</div>
  <div class="cbtn" id="snapBtn">SNAP</div>
</div>
<div id="logWrap"></div>
<div id="wasdHint">WASD / ARROWS</div>
<script>
var feed=document.getElementById('feed'),cv=document.getElementById('cv'),
    ctx=cv.getContext('2d',{willReadFrequently:true}),
    feedWrap=document.getElementById('feedWrap'),
    wsEl=document.getElementById('wsStatus');
var streamUrl=location.protocol+'//'+location.hostname+':81/stream';
var streamOn=true,expanded=false;
feed.src=streamUrl;

var mode='raw',prevGray=null;

function showFeed(){
  if(mode==='raw'){feed.style.display='block';cv.style.display='none';}
  else{feed.style.display='none';cv.style.display='block';}
}

// === WEBSOCKET DRIVE ===
var jT=0,jS=0,maxSpd=55,autoMode=false,touching=false;
var ws=null,wsOk=false;
function wsConnect(){
  ws=new WebSocket('ws://'+location.hostname+'/ws');
  ws.onopen=function(){wsOk=true;wsEl.textContent='WS OK';wsEl.className='ok';addLog('WS CONNECTED',true);};
  ws.onclose=function(){wsOk=false;wsEl.textContent='WS ---';wsEl.className='';setTimeout(wsConnect,1000);};
  ws.onerror=function(){wsOk=false;};
}
wsConnect();
function sendDrive(){
  if(wsOk&&ws.readyState===1)ws.send(jT+','+jS);
  else fetch('/drive?t='+jT+'&s='+jS).catch(function(){});
}
var cmdTimer=null;
function startCmd(){if(cmdTimer)return;sendDrive();cmdTimer=setInterval(sendDrive,25);}
function stopCmd(){if(cmdTimer){clearInterval(cmdTimer);cmdTimer=null;}jT=0;jS=0;sendDrive();}

// === JOYSTICK ===
var joy=document.getElementById('joy'),knob=document.getElementById('joyKnob'),jRect=null;
function jCalc(tx,ty){
  if(!jRect)jRect=joy.getBoundingClientRect();
  var cx=jRect.left+jRect.width/2,cy=jRect.top+jRect.height/2;
  var dx=tx-cx,dy=ty-cy;
  var dist=Math.sqrt(dx*dx+dy*dy),mr=jRect.width/2-22;
  if(dist>mr){dx=dx/dist*mr;dy=dy/dist*mr;}
  knob.style.transform='translate(calc(-50% + '+dx+'px),calc(-50% + '+dy+'px))';
  jS=Math.round(dx/mr*maxSpd);
  jT=Math.round(-dy/mr*maxSpd);
}
function jEnd(){touching=false;knob.style.transform='translate(-50%,-50%)';jT=0;jS=0;
  if(!autoMode)stopCmd();jRect=null;}

joy.ontouchstart=function(e){e.preventDefault();touching=true;
  jRect=joy.getBoundingClientRect();jCalc(e.touches[0].clientX,e.touches[0].clientY);
  if(!autoMode)startCmd();};
joy.ontouchmove=function(e){if(!touching)return;e.preventDefault();
  jCalc(e.touches[0].clientX,e.touches[0].clientY);};
joy.ontouchend=function(e){e.preventDefault();jEnd();};
joy.ontouchcancel=function(){jEnd();};
joy.onmousedown=function(e){e.preventDefault();touching=true;jRect=joy.getBoundingClientRect();
  jCalc(e.clientX,e.clientY);if(!autoMode)startCmd();};
document.onmousemove=function(e){if(touching)jCalc(e.clientX,e.clientY);};
document.onmouseup=function(){if(touching)jEnd();};

// === KEYBOARD ===
var keys={};
document.onkeydown=function(e){
  var k=e.key.toLowerCase();
  if(k==='arrowup')k='w';if(k==='arrowdown')k='s';if(k==='arrowleft')k='a';if(k==='arrowright')k='d';
  if('wasd'.indexOf(k)<0)return;
  e.preventDefault();if(keys[k])return;keys[k]=true;kDrive();};
document.onkeyup=function(e){
  var k=e.key.toLowerCase();
  if(k==='arrowup')k='w';if(k==='arrowdown')k='s';if(k==='arrowleft')k='a';if(k==='arrowright')k='d';
  if('wasd'.indexOf(k)<0)return;
  e.preventDefault();keys[k]=false;kDrive();};
function kDrive(){
  var t=0,s=0;
  if(keys.w)t+=maxSpd;if(keys.s)t-=maxSpd;
  if(keys.a)s-=maxSpd;if(keys.d)s+=maxSpd;
  jT=t;jS=s;
  if(t||s){if(!cmdTimer)startCmd();}else stopCmd();
}

// === SPEED ===
document.getElementById('speedRow').onclick=function(e){if(!e.target.dataset.s)return;
  maxSpd=parseInt(e.target.dataset.s);
  document.querySelectorAll('.spd').forEach(function(b){b.classList.toggle('on',b===e.target);});};

// === MODES ===
document.getElementById('bar').onclick=function(e){if(!e.target.dataset.m)return;
  mode=e.target.dataset.m;prevGray=null;
  if(streamOn)showFeed();
  document.querySelectorAll('[data-m]').forEach(function(b){b.classList.toggle('on',b.dataset.m===mode);});};

// === CAM TOGGLE ===
var camBtn=document.getElementById('camBtn');
camBtn.onclick=function(){
  streamOn=!streamOn;
  if(streamOn){feed.src=streamUrl;feedWrap.classList.remove('camoff');
    camBtn.textContent='CAM OFF';camBtn.classList.remove('on');showFeed();addLog('STREAM ON',true);}
  else{feed.src='';feedWrap.classList.add('camoff');
    camBtn.textContent='CAM ON';camBtn.classList.add('on');addLog('STREAM OFF');}};

// === EXPAND TOGGLE ===
var expandBtn=document.getElementById('expandBtn');
expandBtn.onclick=function(){
  expanded=!expanded;
  feed.classList.toggle('full',expanded);cv.classList.toggle('full',expanded);
  expandBtn.classList.toggle('on',expanded);expandBtn.textContent=expanded?'FIT':'FULL';};

// === SNAP ===
document.getElementById('snapBtn').onclick=function(){
  if(!feed.naturalWidth){addLog('NO FRAME');return;}
  var sc=document.createElement('canvas');sc.width=feed.naturalWidth;sc.height=feed.naturalHeight;
  try{sc.getContext('2d').drawImage(feed,0,0);
    var a=document.createElement('a');a.download='lune-'+Date.now()+'.png';a.href=sc.toDataURL();a.click();
    addLog('SNAP SAVED',true);
  }catch(e){addLog('SNAP FAILED: '+e.message);}};

// === LOG ===
var logEl=document.getElementById('logWrap');
function addLog(m,hi){var d=document.createElement('div');d.className='logln'+(hi?' hi':'');
  d.innerHTML='<span class="ts">'+new Date().toTimeString().slice(0,8)+'</span>'+m;
  logEl.appendChild(d);while(logEl.children.length>3)logEl.removeChild(logEl.firstChild);}
addLog('SYSTEMS NOMINAL',true);

// === STATUS ===
function poll(){fetch('/status').then(function(r){return r.json();}).then(function(d){
  document.getElementById('tT').textContent=d.thr;
  document.getElementById('tS').textContent=d.trn;
  document.getElementById('tP').textContent=d.pkt;
  document.getElementById('tCh').textContent=d.ch;
  document.getElementById('conn').textContent=d.wifi?d.ip:'AP MODE';
}).catch(function(){document.getElementById('conn').textContent='NO LINK';});}
setInterval(poll,2000);

// === MAP TOGGLE ===
var mapBtn=document.getElementById('mapBtn'),mapWrap=document.getElementById('mapWrap'),showMap=false;
mapBtn.onclick=function(){showMap=!showMap;mapWrap.classList.toggle('on',showMap);mapBtn.classList.toggle('on',showMap);};

// === AUTO NAV — SMART ===
var ZR=3,ZC=5,aState='IDLE',turnDir=1,stuckN=0,cliffTicks=0;
var zoneViz=document.getElementById('zoneViz');
for(var i=0;i<ZR*ZC;i++){var zd=document.createElement('div');zd.className='zc';zoneViz.appendChild(zd);}
var autoNav=document.getElementById('autonav'),autoBtnEl=document.getElementById('autoBtn');
var cliffEl=document.getElementById('cliffWarn');
var navCv=document.createElement('canvas'),navX=navCv.getContext('2d',{willReadFrequently:true});
var NW=80,NH=60;navCv.width=NW;navCv.height=NH;

// Occupancy grid
var MS=64,map=new Int8Array(MS*MS),mapX=MS/2,mapY=MS/2,heading=0;
var mapCv=document.getElementById('mapCv'),mapCtx=mapCv.getContext('2d');

// Floor reference (learned on auto start)
var floorRef=null,floorFrames=0;

// Tuning
var CLIFF_THRESH=35,FLOOR_THRESH=45,WALL_BIAS=12;

function resetMap(){map.fill(0);mapX=MS/2;mapY=MS/2;heading=0;floorRef=null;floorFrames=0;}

autoBtnEl.onclick=function(){addLog('AUTO DISABLED — COMING SOON');};

function drawMap(){
  var s=mapCv.width/MS;
  mapCtx.fillStyle='#111';mapCtx.fillRect(0,0,mapCv.width,mapCv.height);
  for(var my=0;my<MS;my++)for(var mx=0;mx<MS;mx++){
    var v=map[my*MS+mx];if(v===0)continue;
    mapCtx.fillStyle=v>0?'rgba(0,200,80,0.5)':'rgba(255,60,60,0.7)';
    mapCtx.fillRect(mx*s,my*s,s,s);}
  var px=mapX*s,py=mapY*s;
  mapCtx.fillStyle='var(--o)';mapCtx.beginPath();mapCtx.arc(px,py,2.5,0,6.28);mapCtx.fill();
  var hx=px+Math.sin(heading)*5,hy=py-Math.cos(heading)*5;
  mapCtx.strokeStyle='var(--o)';mapCtx.lineWidth=1.5;
  mapCtx.beginPath();mapCtx.moveTo(px,py);mapCtx.lineTo(hx,hy);mapCtx.stroke();
}

setInterval(function(){
  if(!autoMode||!streamOn||!feed.naturalWidth)return;
  try{navX.drawImage(feed,0,0,NW,NH);}catch(e){return;}
  var img=navX.getImageData(0,0,NW,NH),d=img.data;
  var gray=new Uint8Array(NW*NH);
  for(var i=0;i<d.length;i+=4)gray[i>>2]=(d[i]*77+d[i+1]*150+d[i+2]*29)>>8;

  // Floor color sampling (from color data, bottom center 20x10)
  var fR=0,fG=0,fB=0,fN=0;
  for(var fy=48;fy<58;fy++)for(var fx=30;fx<50;fx++){
    var fi=fy*NW+fx;fR+=d[fi*4];fG+=d[fi*4+1];fB+=d[fi*4+2];fN++;}
  fR/=fN;fG/=fN;fB/=fN;

  // Learn floor on first 5 frames
  if(aState==='LEARN'){
    if(!floorRef)floorRef={r:fR,g:fG,b:fB};
    else{floorRef.r=(floorRef.r+fR)/2;floorRef.g=(floorRef.g+fG)/2;floorRef.b=(floorRef.b+fB)/2;}
    floorFrames++;
    if(floorFrames>=5){aState='SCAN';addLog('FLOOR LEARNED R'+Math.round(floorRef.r)+' G'+Math.round(floorRef.g)+' B'+Math.round(floorRef.b));}
    document.getElementById('aState').textContent='LEARN';
    document.getElementById('aReason').textContent='SAMPLING FLOOR...';
    jT=0;jS=0;drawMap();return;
  }

  // Floor color deviation
  var floorDev=floorRef?Math.sqrt(Math.pow(fR-floorRef.r,2)+Math.pow(fG-floorRef.g,2)+Math.pow(fB-floorRef.b,2)):0;
  var floorChanged=floorDev>FLOOR_THRESH;

  // Cliff detection: compare mid-floor rows vs near-floor rows
  var midAvg=0,nearAvg=0,mn=0;
  for(var cy=36;cy<46;cy++)for(var cx=20;cx<60;cx++){midAvg+=gray[cy*NW+cx];mn++;}
  midAvg/=mn;mn=0;
  for(var cy2=50;cy2<58;cy2++)for(var cx2=20;cx2<60;cx2++){nearAvg+=gray[cy2*NW+cx2];mn++;}
  nearAvg/=mn;
  var cliffRisk=Math.abs(nearAvg-midAvg)>CLIFF_THRESH||floorChanged;
  cliffEl.classList.toggle('on',cliffRisk);

  // Zone brightness + per-zone edge density
  var zw=NW/ZC,zh=NH/ZR,zones=new Float32Array(ZR*ZC),zEdge=new Float32Array(ZR*ZC);
  for(var zy=0;zy<ZR;zy++)for(var zx=0;zx<ZC;zx++){var sb=0,se=0,n=0;
    for(var py=(zy*zh)|0;py<((zy+1)*zh)|0;py++)
      for(var px=(zx*zw)|0;px<((zx+1)*zw)|0;px++){
        sb+=gray[py*NW+px];n++;
        if(py>0&&py<NH-1&&px>0&&px<NW-1){
          var ei2=py*NW+px;
          if(Math.abs(gray[ei2+1]-gray[ei2-1])+Math.abs(gray[ei2+NW]-gray[ei2-NW])>25)se++;}}
    zones[zy*ZC+zx]=n?sb/n:0;zEdge[zy*ZC+zx]=n?se/n*100:0;}
  for(var zi=0;zi<ZR*ZC;zi++){var v=zones[zi]/255;var cell=zoneViz.children[zi];
    if(cell){cell.textContent=Math.round(v*100);
      cell.style.borderColor='rgba(255,106,0,'+(v*0.5+0.1).toFixed(2)+')';}}

  // Clearance scores: blend brightness with edge density (textured = floor = clear)
  var mid=Math.floor(ZC/2),sL=0,sC=0,sR=0;
  for(var c=0;c<ZC;c++){
    var score=zones[2*ZC+c]*0.5+zEdge[2*ZC+c]*50;
    if(c<mid)sL+=score;else if(c===mid)sC=score;else sR+=score;}
  sL/=mid;sR/=mid;
  var clearAhead=sC>60&&!cliffRisk,blocked=sC<30||cliffRisk;

  // Motion detection
  var motD=0;
  if(prevGray&&prevGray.length===gray.length){var tot=0;
    for(var mi=0;mi<gray.length;mi++)if(Math.abs(gray[mi]-prevGray[mi])>20)tot++;
    motD=tot/gray.length*100;}
  prevGray=gray;

  // Wall detection (consistent high edge density on one side)
  var wallL=zEdge[2*ZC]>15||zEdge[1*ZC]>15;
  var wallR=zEdge[2*ZC+ZC-1]>15||zEdge[1*ZC+ZC-1]>15;

  // Dead reckoning
  heading+=jS*0.003;
  var dist=jT*0.006;
  mapX+=dist*Math.sin(heading);mapY-=dist*Math.cos(heading);
  mapX=Math.max(1,Math.min(MS-2,mapX));mapY=Math.max(1,Math.min(MS-2,mapY));
  var mi2=Math.round(mapY)*MS+Math.round(mapX);
  if(mi2>=0&&mi2<MS*MS)map[mi2]=1;
  if(blocked){
    var ox=mapX+Math.sin(heading)*2,oy=mapY-Math.cos(heading)*2;
    var oi=Math.round(oy)*MS+Math.round(ox);
    if(oi>=0&&oi<MS*MS)map[oi]=-1;}

  // Exploration bias: check what's ahead in unknown vs explored
  var exploreL=0,exploreR=0;
  for(var ed=1;ed<6;ed++){
    var lx=Math.round(mapX+Math.sin(heading-0.5)*ed),ly=Math.round(mapY-Math.cos(heading-0.5)*ed);
    var rx=Math.round(mapX+Math.sin(heading+0.5)*ed),ry=Math.round(mapY-Math.cos(heading+0.5)*ed);
    if(lx>=0&&lx<MS&&ly>=0&&ly<MS&&map[ly*MS+lx]===0)exploreL++;
    if(rx>=0&&rx<MS&&ry>=0&&ry<MS&&map[ry*MS+rx]===0)exploreR++;
  }

  // FSM
  var stEl=document.getElementById('aState'),rsEl=document.getElementById('aReason');
  var t=0,s=0;
  if(motD<0.5&&aState==='FWD')stuckN++;else if(aState==='FWD')stuckN=0;

  if(aState==='CLIFF'){
    t=-35;s=0;cliffTicks--;rsEl.textContent='EDGE DETECTED — REVERSING';
    if(cliffTicks<=0){aState='SCAN';turnDir=exploreL>exploreR?-1:1;}}
  else if(cliffRisk&&aState!=='LEARN'){
    aState='CLIFF';cliffTicks=4;t=0;s=0;addLog('CLIFF DETECTED!',true);rsEl.textContent='STOPPING!';}
  else if(aState==='SCAN'){t=0;s=25*turnDir;rsEl.textContent='SCANNING';
    if(clearAhead){aState='FWD';stuckN=0;}}
  else if(aState==='FWD'){
    if(blocked){aState='AVOID';turnDir=sL>sR?-1:1;
      if(exploreL>exploreR+2)turnDir=-1;else if(exploreR>exploreL+2)turnDir=1;
      rsEl.textContent='OBSTACLE!';}
    else if(stuckN>6){aState='REV';rsEl.textContent='STUCK';}
    else{t=30;
      if(wallL&&!wallR)s=WALL_BIAS;
      else if(wallR&&!wallL)s=-WALL_BIAS;
      else if(sL>sR+15)s=-8;else if(sR>sL+15)s=8;
      rsEl.textContent=wallL?'WALL-L':wallR?'WALL-R':'CLEAR';}}
  else if(aState==='AVOID'){t=-15;s=40*turnDir;rsEl.textContent='AVOIDING';
    if(clearAhead){aState='FWD';stuckN=0;}}
  else if(aState==='REV'){t=-30;s=25*turnDir;rsEl.textContent='REVERSING';stuckN--;
    if(stuckN<=0||clearAhead){aState='SCAN';stuckN=0;turnDir=exploreL>exploreR?-1:1;}}
  else aState='SCAN';
  stEl.textContent=aState;jT=t;jS=s;
  addLog('NAV '+aState+(cliffRisk?' [CLIFF]':''));
  drawMap();
},500);

// === VISUAL MODES ===
setInterval(function(){
  if(mode==='raw'||!streamOn||!feed.naturalWidth)return;
  var w=feed.naturalWidth,h=feed.naturalHeight;
  if(cv.width!==w)cv.width=w;if(cv.height!==h)cv.height=h;
  try{ctx.drawImage(feed,0,0);}catch(e){return;}
  var img=ctx.getImageData(0,0,w,h),d=img.data,len=d.length;
  var gray=new Uint8Array(w*h);
  for(var i=0;i<len;i+=4)gray[i>>2]=(d[i]*77+d[i+1]*150+d[i+2]*29)>>8;
  if(mode==='night'){for(var ni=0;ni<len;ni+=4){var b=Math.min(255,gray[ni>>2]*2.5);
    d[ni]=0;d[ni+1]=b|0;d[ni+2]=b*0.15|0;}}
  else if(mode==='edges'){
    for(var p=0;p<len;p+=4){d[p]=0;d[p+1]=0;d[p+2]=0;}
    for(var sy=1;sy<h-1;sy++)for(var sx=1;sx<w-1;sx++){var si=sy*w+sx;
    var gx=-gray[si-w-1]-2*gray[si-1]-gray[si+w-1]+gray[si-w+1]+2*gray[si+1]+gray[si+w+1];
    var gy=-gray[si-w-1]-2*gray[si-w]-gray[si-w+1]+gray[si+w-1]+2*gray[si+w]+gray[si+w+1];
    var sv=Math.min(255,Math.sqrt(gx*gx+gy*gy));var sp=si*4;d[sp]=sv*0.4|0;d[sp+1]=sv*0.3|0;d[sp+2]=sv|0;}}
  else if(mode==='therm'){for(var ti=0;ti<len;ti+=4){var tn=gray[ti>>2]/255;
    if(tn<0.25){d[ti]=0;d[ti+1]=0;d[ti+2]=tn*4*200|0;}
    else if(tn<0.5){d[ti]=0;d[ti+1]=(tn-0.25)*4*255|0;d[ti+2]=(0.5-tn)*4*200|0;}
    else if(tn<0.75){d[ti]=(tn-0.5)*4*255|0;d[ti+1]=200;d[ti+2]=0;}
    else{d[ti]=255;d[ti+1]=(1-tn)*4*200|0;d[ti+2]=0;}}}
  else if(mode==='motion'){
    var mg=new Uint8Array(w*h);
    for(var mi2=0;mi2<len;mi2+=4)mg[mi2>>2]=(d[mi2]*77+d[mi2+1]*150+d[mi2+2]*29)>>8;
    for(var p2=0;p2<len;p2+=4){d[p2]=0;d[p2+1]=0;d[p2+2]=0;}
    if(prevGray&&prevGray.length===mg.length){
      for(var moi=0;moi<mg.length;moi++){
        if(Math.abs(mg[moi]-prevGray[moi])>20){var mp=moi*4;d[mp]=255;d[mp+1]=106;d[mp+2]=0;}}}
    prevGray=mg;}
  else if(mode==='enhance'){for(var ei=0;ei<len;ei+=4){d[ei]=Math.min(255,d[ei]*1.4+20)|0;
    d[ei+1]=Math.min(255,d[ei+1]*1.3+10)|0;d[ei+2]=Math.min(255,d[ei+2]*1.2)|0;}}
  ctx.putImageData(img,0,0);
},83);
</script>
</body>
</html>
)rawliteral";
