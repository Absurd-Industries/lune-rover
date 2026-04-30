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
:root{--o:#FF6A00;--bg:#0A0A0A;--dim:#666;--f:'SF Mono',SFMono-Regular,'IBM Plex Mono',Consolas,monospace}
html,body{width:100%;height:100%;overflow:hidden;background:var(--bg);color:#E8E8E8;font-family:var(--f)}
#feed{position:absolute;top:0;left:0;width:100%;height:100%;object-fit:cover;z-index:0}
canvas#cv{position:absolute;top:0;left:0;width:100%;height:100%;object-fit:cover;pointer-events:none;z-index:0;display:none}
#scan{position:absolute;top:0;left:0;width:100%;height:100%;pointer-events:none;
  background:repeating-linear-gradient(0deg,transparent,transparent 2px,rgba(255,106,0,0.02) 2px,rgba(255,106,0,0.02) 4px);z-index:1}
#grid{position:absolute;top:0;left:0;width:100%;height:100%;pointer-events:none;opacity:0.03;z-index:1;
  background-image:repeating-linear-gradient(0deg,transparent,transparent 39px,var(--o) 39px,var(--o) 40px),
  repeating-linear-gradient(90deg,transparent,transparent 39px,var(--o) 39px,var(--o) 40px)}

#top{position:absolute;top:0;left:0;right:0;z-index:20;display:flex;justify-content:space-between;
  align-items:center;padding:10px 14px;background:linear-gradient(var(--bg),transparent)}
#top .brand{font-size:12px;font-weight:700;letter-spacing:0.2em;color:var(--o)}
#top .info{font-size:9px;letter-spacing:0.08em;color:var(--dim);text-align:right}
#top .info span{display:block;line-height:1.5}
#top .info .live{color:var(--o)}

#modes{position:absolute;top:46px;left:10px;z-index:20;display:flex;flex-wrap:wrap;gap:4px}
.pill{font-size:8px;letter-spacing:0.1em;padding:4px 8px;border:1px solid rgba(255,106,0,0.2);
  background:rgba(10,10,10,0.8);color:var(--dim);cursor:pointer;
  clip-path:polygon(4px 0,100% 0,calc(100% - 4px) 100%,0 100%)}
.pill.on{border-color:var(--o);color:var(--o);background:rgba(255,106,0,0.1)}

#telem{position:absolute;top:90px;left:10px;z-index:15;font-size:8px;color:var(--dim);
  line-height:1.6;pointer-events:none;opacity:0.6}
#telem span{display:block}
#telem .val{color:#E8E8E8}

#joyWrap{position:absolute;bottom:24px;left:20px;z-index:30;touch-action:none}
#joy{width:140px;height:140px;border-radius:50%;border:2px solid rgba(255,106,0,0.3);
  background:rgba(10,10,10,0.5);position:relative;touch-action:none}
#joyKnob{width:48px;height:48px;border-radius:50%;border:2px solid var(--o);
  background:rgba(255,106,0,0.15);position:absolute;top:50%;left:50%;
  transform:translate(-50%,-50%);pointer-events:none}
#joyLabel{text-align:center;font-size:8px;letter-spacing:0.15em;color:var(--dim);margin-top:6px}

#ctrl{position:absolute;bottom:24px;right:14px;z-index:30;display:flex;flex-direction:column;
  gap:8px;align-items:flex-end}
.cbtn{width:56px;height:56px;border-radius:50%;border:2px solid rgba(255,106,0,0.3);
  background:rgba(10,10,10,0.6);display:flex;align-items:center;justify-content:center;
  font-size:8px;font-weight:700;letter-spacing:0.1em;color:var(--dim);cursor:pointer;font-family:var(--f)}
.cbtn:active{background:rgba(255,106,0,0.2)}
.cbtn.on{border-color:var(--o);color:var(--o);background:rgba(255,106,0,0.15);
  box-shadow:0 0 20px rgba(255,106,0,0.2)}
#speedRow{display:flex;gap:4px}
#speedRow .spd{padding:4px 8px;font-size:8px;letter-spacing:0.08em;border:1px solid rgba(255,106,0,0.2);
  background:rgba(10,10,10,0.8);color:var(--dim);cursor:pointer;font-family:var(--f);
  clip-path:polygon(3px 0,100% 0,calc(100% - 3px) 100%,0 100%)}
#speedRow .spd.on{border-color:var(--o);color:var(--o)}

#logWrap{position:absolute;bottom:0;left:0;right:0;z-index:15;padding:6px 14px;
  background:linear-gradient(transparent,var(--bg));font-size:8px;line-height:1.6;
  color:var(--dim);max-height:50px;overflow:hidden;pointer-events:none}
.logln{white-space:nowrap}.logln .ts{color:rgba(255,106,0,0.4);margin-right:6px}
.logln.hi{color:var(--o)}

#autonav{position:absolute;top:46px;right:10px;z-index:15;text-align:right;font-size:9px;
  color:var(--o);opacity:0;transition:opacity 0.3s;pointer-events:none}
#autonav.on{opacity:1}
#autonav .state{font-weight:700;font-size:11px;letter-spacing:0.15em}
#autonav .reason{color:var(--dim);font-size:8px;margin-top:2px}
#zoneViz{display:grid;grid-template-columns:repeat(5,18px);gap:1px;margin-top:6px;justify-content:flex-end}
.zc{width:18px;height:14px;font-size:7px;text-align:center;line-height:14px;
  background:rgba(255,106,0,0.05);color:var(--dim);border:1px solid rgba(255,106,0,0.1)}

#wasdHint{position:absolute;bottom:80px;left:50%;transform:translateX(-50%);z-index:15;
  font-size:8px;color:var(--dim);letter-spacing:0.1em;opacity:0.4;pointer-events:none;display:none}
@media(hover:hover){#wasdHint{display:block}}
</style>
</head>
<body>
<img id="feed">
<canvas id="cv"></canvas>
<div id="scan"></div>
<div id="grid"></div>
<div id="top">
  <span class="brand">LUN-E</span>
  <div class="info">
    <span class="live" id="fps">STREAM</span>
    <span id="conn">CONNECTING...</span>
  </div>
</div>
<div id="modes">
  <button class="pill on" data-m="raw">RAW</button>
  <button class="pill" data-m="night">NVIS</button>
  <button class="pill" data-m="edges">EDGE</button>
  <button class="pill" data-m="therm">THRM</button>
  <button class="pill" data-m="motion">MOTN</button>
  <button class="pill" data-m="enhance">ENHANCE</button>
</div>
<div id="telem">
  <span>THR <span class="val" id="tT">0</span> TRN <span class="val" id="tS">0</span></span>
  <span>PKT <span class="val" id="tP">0</span></span>
  <span id="tCh">CH --</span>
</div>
<div id="autonav">
  <div class="state" id="aState">IDLE</div>
  <div class="reason" id="aReason">--</div>
  <div id="zoneViz"></div>
</div>
<div id="joyWrap">
  <div id="joy"><div id="joyKnob"></div></div>
  <div id="joyLabel">PILOT CONTROL</div>
</div>
<div id="ctrl">
  <div id="speedRow">
    <button class="spd" data-s="25">SLO</button>
    <button class="spd on" data-s="55">MED</button>
    <button class="spd" data-s="100">MAX</button>
  </div>
  <div class="cbtn" id="autoBtn">AUTO</div>
  <div class="cbtn" id="snapBtn">SNAP</div>
</div>
<div id="logWrap"></div>
<div id="wasdHint">WASD TO DRIVE</div>
<script>
var feed=document.getElementById('feed'),cv=document.getElementById('cv'),
    ctx=cv.getContext('2d',{willReadFrequently:true});
var streamUrl=location.protocol+'//'+location.hostname+':81/stream';
feed.src=streamUrl;

var mode='raw',prevGray=null;

// === DRIVE ===
var jT=0,jS=0,maxSpd=55,autoMode=false,lastSentT=99,lastSentS=99,touching=false;
function send(t,s){
  if(t===lastSentT&&s===lastSentS)return;
  lastSentT=t;lastSentS=s;
  fetch('/drive?t='+t+'&s='+s).catch(function(){});
}
var cmdTimer=null;
function startCmd(){if(cmdTimer)return;send(jT,jS);cmdTimer=setInterval(function(){send(jT,jS);},100);}
function stopCmd(){if(cmdTimer){clearInterval(cmdTimer);cmdTimer=null;}send(0,0);lastSentT=99;lastSentS=99;}

// === JOYSTICK ===
var joy=document.getElementById('joy'),knob=document.getElementById('joyKnob'),jRect=null;
function jCalc(tx,ty){
  if(!jRect)jRect=joy.getBoundingClientRect();
  var cx=jRect.left+jRect.width/2,cy=jRect.top+jRect.height/2;
  var dx=tx-cx,dy=ty-cy;
  var dist=Math.sqrt(dx*dx+dy*dy),mr=jRect.width/2-24;
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

// === KEYBOARD (WASD + arrows) ===
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
document.getElementById('modes').onclick=function(e){if(!e.target.dataset.m)return;
  mode=e.target.dataset.m;prevGray=null;
  if(mode==='raw'){feed.style.display='';cv.style.display='none';}
  else{feed.style.display='none';cv.style.display='';}
  document.querySelectorAll('.pill').forEach(function(b){b.classList.toggle('on',b.dataset.m===mode);});};

// === SNAP ===
document.getElementById('snapBtn').onclick=function(){
  var sc=document.createElement('canvas');sc.width=feed.naturalWidth||320;sc.height=feed.naturalHeight||240;
  sc.getContext('2d').drawImage(feed,0,0);
  var a=document.createElement('a');a.download='lune-'+Date.now()+'.png';a.href=sc.toDataURL();a.click();};

// === LOG ===
var logEl=document.getElementById('logWrap');
function addLog(m,hi){var d=document.createElement('div');d.className='logln'+(hi?' hi':'');
  d.innerHTML='<span class="ts">'+new Date().toTimeString().slice(0,8)+'</span>'+m;
  logEl.appendChild(d);while(logEl.children.length>4)logEl.removeChild(logEl.firstChild);}
addLog('SYSTEMS NOMINAL',true);addLog('AWAITING PILOT INPUT');

// === STATUS ===
function poll(){fetch('/status').then(function(r){return r.json();}).then(function(d){
  document.getElementById('tT').textContent=d.thr;
  document.getElementById('tS').textContent=d.trn;
  document.getElementById('tP').textContent=d.pkt;
  document.getElementById('tCh').textContent='CH '+d.ch+(d.wifi?' // WIFI':'');
  document.getElementById('conn').textContent=d.wifi?d.ip:'AP MODE';
}).catch(function(){document.getElementById('conn').textContent='NO LINK';});}
setInterval(poll,1500);

// === AUTO NAV (lightweight — runs on timer, not every frame) ===
var ZR=3,ZC=5,aState='IDLE',turnDir=1,stuckN=0;
var zoneViz=document.getElementById('zoneViz');
for(var i=0;i<ZR*ZC;i++){var zd=document.createElement('div');zd.className='zc';zoneViz.appendChild(zd);}
var autoNav=document.getElementById('autonav'),autoBtnEl=document.getElementById('autoBtn');
var navCanvas=document.createElement('canvas'),navCtx=navCanvas.getContext('2d',{willReadFrequently:true});
var NAV_W=80,NAV_H=60;navCanvas.width=NAV_W;navCanvas.height=NAV_H;

autoBtnEl.onclick=function(){autoMode=!autoMode;autoBtnEl.classList.toggle('on',autoMode);
  autoNav.classList.toggle('on',autoMode);
  if(!autoMode){send(0,0);aState='IDLE';document.getElementById('aState').textContent='OFF';stopCmd();}
  else{addLog('AUTO ENGAGED',true);aState='SCAN';startCmd();}};

var navTimer=null;
function startNav(){if(navTimer)return;navTimer=setInterval(runNav,500);}
function stopNav(){if(navTimer){clearInterval(navTimer);navTimer=null;}}
startNav();

function runNav(){
  if(!autoMode)return;
  if(!feed.naturalWidth)return;
  navCtx.drawImage(feed,0,0,NAV_W,NAV_H);
  var img=navCtx.getImageData(0,0,NAV_W,NAV_H),d=img.data;
  var gray=new Uint8Array(NAV_W*NAV_H);
  for(var i=0;i<d.length;i+=4)gray[i>>2]=(d[i]*77+d[i+1]*150+d[i+2]*29)>>8;

  var zw=NAV_W/ZC,zh=NAV_H/ZR,zones=new Float32Array(ZR*ZC);
  for(var zy=0;zy<ZR;zy++)for(var zx=0;zx<ZC;zx++){var s=0,n=0;
    for(var py=(zy*zh)|0;py<((zy+1)*zh)|0;py++)
      for(var px=(zx*zw)|0;px<((zx+1)*zw)|0;px++){s+=gray[py*NAV_W+px];n++;}
    zones[zy*ZC+zx]=n?s/n:0;}
  for(var zi=0;zi<ZR*ZC;zi++){var v=zones[zi]/255;var cell=zoneViz.children[zi];
    if(cell){cell.textContent=Math.round(v*100);
      cell.style.borderColor='rgba(255,106,0,'+(v*0.5+0.1).toFixed(2)+')';}}

  var ec=0;for(var ey=1;ey<NAV_H-1;ey++)for(var ex=1;ex<NAV_W-1;ex++){var ei=ey*NAV_W+ex;
    if(Math.abs(gray[ei+1]-gray[ei-1])+Math.abs(gray[ei+NAV_W]-gray[ei-NAV_W])>30)ec++;}
  var edgeD=ec/(NAV_W*NAV_H)*100;
  var motD=0;
  if(prevGray){var tot=0;for(var mi=0;mi<gray.length;mi++)if(Math.abs(gray[mi]-prevGray[mi])>20)tot++;
    motD=tot/gray.length*100;}
  prevGray=gray;

  var mid=Math.floor(ZC/2),nL=0,nC=0,nR=0;
  for(var c=0;c<ZC;c++){
    var bot=zones[2*ZC+c];
    if(c<mid)nL+=bot;else if(c===mid)nC=bot;else nR+=bot;}
  nL/=mid;nR/=mid;
  var clearAhead=nC>80,blocked=nC<50;
  var stEl=document.getElementById('aState'),rsEl=document.getElementById('aReason');
  var t=0,s=0;
  if(motD<0.5&&aState==='FWD')stuckN++;else stuckN=0;
  if(aState==='SCAN'){t=0;s=25*turnDir;rsEl.textContent='SCANNING';
    if(clearAhead){aState='FWD';stuckN=0;}}
  else if(aState==='FWD'){
    if(blocked||edgeD>25){aState='AVOID';turnDir=nL>nR?-1:1;rsEl.textContent='OBSTACLE!';}
    else if(stuckN>6){aState='REV';rsEl.textContent='STUCK';}
    else{t=35;if(nL>nR+20)s=-10;else if(nR>nL+20)s=10;rsEl.textContent='CLEAR';}}
  else if(aState==='AVOID'){t=-15;s=40*turnDir;rsEl.textContent='AVOIDING';
    if(clearAhead){aState='FWD';stuckN=0;}}
  else if(aState==='REV'){t=-30;s=20*turnDir;rsEl.textContent='REVERSING';
    if(stuckN<=0||clearAhead){aState='SCAN';stuckN=0;}stuckN--;}
  else aState='SCAN';
  stEl.textContent=aState;jT=t;jS=s;
  addLog('NAV '+aState+' T'+t+' S'+s);
}

// === VISUAL MODES (throttled, only when active) ===
var vizTimer=null,VIZ_FPS=12;
function startViz(){if(vizTimer)return;vizTimer=setInterval(renderViz,1000/VIZ_FPS);}
function stopViz(){if(vizTimer){clearInterval(vizTimer);vizTimer=null;}}

function renderViz(){
  if(mode==='raw'||!feed.naturalWidth)return;
  var w=feed.naturalWidth,h=feed.naturalHeight;
  if(cv.width!==w)cv.width=w;
  if(cv.height!==h)cv.height=h;
  ctx.drawImage(feed,0,0);
  var img=ctx.getImageData(0,0,w,h),d=img.data,len=d.length;
  var gray=new Uint8Array(w*h);
  for(var i=0;i<len;i+=4)gray[i>>2]=(d[i]*77+d[i+1]*150+d[i+2]*29)>>8;

  if(mode==='night'){for(var ni=0;ni<len;ni+=4){var b=Math.min(255,gray[ni>>2]*2.5);
    d[ni]=0;d[ni+1]=b;d[ni+2]=b*0.15|0;}}
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
}
startViz();
</script>
</body>
</html>
)rawliteral";
