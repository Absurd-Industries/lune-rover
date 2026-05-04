#pragma once

static const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Explorer</title>
<script src="https://unpkg.com/lucide@latest/dist/umd/lucide.min.js"></script>
<script src="https://unpkg.com/three@0.160.0/build/three.min.js"></script>
<style>
*{margin:0;padding:0;box-sizing:border-box;user-select:none;-webkit-user-select:none}
:root{--bg:#0A0A0A;--fg:#E0E0E0;--dim:#666;--bdr:#222;--pnl:#141414;
  --accent:#FF6A00;--f:'SF Mono',SFMono-Regular,Consolas,monospace}
html,body{width:100%;height:100%;overflow:hidden;background:var(--bg);color:var(--fg);font-family:var(--f);font-size:11px}

#grid{display:grid;grid-template-columns:220px 1fr 2fr;gap:6px;height:100vh;padding:6px}
.col{display:flex;flex-direction:column;gap:6px;min-height:0}
.pnl{background:var(--pnl);border:1px solid var(--bdr);border-radius:4px;padding:10px;overflow:hidden}
.pnl-grow{flex:1;min-height:0;display:flex;flex-direction:column}
.ph{font-size:7px;letter-spacing:0.15em;color:var(--dim);margin-bottom:6px;text-transform:uppercase;display:flex;align-items:center;gap:5px}
.ph i{width:11px;height:11px;stroke:var(--dim)}

#identity{text-align:center}
#eyes{display:flex;justify-content:center;gap:14px;padding:18px 0}
.eye{width:42px;height:32px;background:#fff;border-radius:8px;
  transition:height 0.2s ease,width 0.15s ease,border-radius 0.2s ease,transform 0.15s ease}
#stateLabel{font-size:9px;letter-spacing:0.15em;color:var(--accent);font-weight:700;margin:4px 0}
#energyWrap{height:3px;background:#222;border-radius:2px;margin:0 20px 6px}
#energyBar{height:100%;background:var(--accent);border-radius:2px;width:100%;transition:width 0.5s}
#connStatus{font-size:8px;color:var(--dim);letter-spacing:0.1em}

#controls{display:flex;flex-direction:column;align-items:center;gap:8px}
#joy{width:120px;height:120px;border-radius:50%;border:2px solid var(--bdr);
  background:rgba(20,20,20,0.8);position:relative;touch-action:none}
#joyKnob{width:36px;height:36px;border-radius:50%;border:2px solid var(--accent);
  background:rgba(255,106,0,0.1);position:absolute;top:50%;left:50%;
  transform:translate(-50%,-50%);pointer-events:none}
.brow{display:flex;gap:4px;flex-wrap:wrap;justify-content:center}
.btn{font-size:7px;letter-spacing:0.08em;padding:4px 8px;border:1px solid var(--bdr);
  background:var(--pnl);color:var(--dim);cursor:pointer;font-family:var(--f);border-radius:3px;
  display:inline-flex;align-items:center;gap:3px}
.btn:hover{border-color:var(--accent);color:var(--fg)}
.btn.on{border-color:var(--accent);color:var(--accent);font-weight:700}
.btn i{width:10px;height:10px}

#telemetry{font-size:8px;color:var(--dim)}
.tr{display:flex;justify-content:space-between;padding:2px 0;border-bottom:1px solid #1a1a1a}
.tr:last-child{border:none}
.tv{color:var(--accent);font-weight:700}

#tunerPanel .ph{cursor:pointer}
#tunerBody{display:none}
.trow{display:flex;align-items:center;gap:6px;margin:4px 0;font-size:9px}
.trow label{width:55px;color:var(--dim);flex-shrink:0}
.trow input[type=range]{flex:1;height:3px;accent-color:var(--accent)}
.trow span{width:35px;text-align:right;color:var(--fg);font-size:9px;flex-shrink:0}

#camPanel{flex:1;min-height:0;display:flex;flex-direction:column}
#feed{display:block;width:100%;flex:1;min-height:0;object-fit:contain;background:#000;border-radius:3px}
#roverPanel{flex:1;min-height:0;display:flex;flex-direction:column}
#roverWrap{flex:1;min-height:0;background:#000;border-radius:3px;overflow:hidden}
canvas#cv{display:block;width:100%;height:100%;object-fit:contain}

#dialogue{max-height:140px;display:flex;flex-direction:column;overflow:hidden}
#chatArea{flex:1;overflow-y:auto;min-height:40px;max-height:90px;padding:4px;font-size:10px;line-height:1.6}
.msg{margin-bottom:4px;padding:3px 6px;border-radius:3px}
.msg.rover{color:var(--accent);border-left:2px solid var(--accent);padding-left:8px}
.msg.human{color:var(--fg);text-align:right;border-right:2px solid var(--dim);padding-right:8px}
.msg.quest{color:#4CAF50;border-left:2px solid #4CAF50;padding-left:8px}
.msg.system{color:var(--dim);font-size:8px}
#chatInput{background:#111;border:1px solid var(--bdr);color:var(--fg);padding:6px 8px;
  font:10px var(--f);width:100%;border-radius:3px;margin-top:4px}
#chatInput:focus{outline:none;border-color:var(--accent)}
#chatInput::placeholder{color:#444}

#logPanel{max-height:90px;overflow-y:auto;font-size:8px;color:var(--dim)}
.log{padding:1px 0;white-space:nowrap}.log .ts{color:var(--accent);opacity:0.3;margin-right:4px}
.log.hi{color:var(--accent)}.log.qlog{color:#4CAF50}

#map3d{flex:1;min-height:200px;background:#080808;border-radius:3px;overflow:hidden}
#mapInfo{display:flex;justify-content:space-between;font-size:8px;color:var(--dim);margin-top:4px}
#mapInfo .v{color:var(--accent)}

#questPanel .ph{cursor:pointer}
#questBody{display:none}
.qcard{border:1px solid var(--bdr);border-radius:6px;padding:10px;margin-bottom:6px}
.qcard.active{border-color:var(--accent);background:rgba(255,106,0,0.05)}
.qn{font-size:12px;font-weight:700;letter-spacing:0.05em;color:var(--fg);margin-bottom:4px;display:flex;align-items:center;gap:6px}
.qn i{width:12px;height:12px;stroke:var(--accent)}
.qd{font-size:9px;color:var(--dim);margin-bottom:6px;line-height:1.4}
.qp{height:3px;background:#222;border-radius:2px;margin-bottom:4px}
.qb{height:100%;background:var(--accent);border-radius:2px;transition:width 0.5s}
.qs{font-size:9px;color:var(--dim);display:flex;align-items:center;gap:4px}
.qs i{width:9px;height:9px}
.qi{padding:6px 8px;font-size:10px;cursor:pointer;border-bottom:1px solid #1a1a1a;color:var(--dim);display:flex;align-items:center;gap:6px}
.qi:hover{color:var(--accent)}.qi.act{color:var(--accent);font-weight:700}
.qi.done{color:#4CAF50}.qi.locked{color:#333;cursor:default}
.qi i{width:10px;height:10px;flex-shrink:0}

@media(max-width:768px){
  #grid{grid-template-columns:1fr;grid-template-rows:auto;height:auto;min-height:100vh;overflow-y:auto}
  #joy{width:100px;height:100px}
  #map3d{min-height:250px}
}
</style>
</head>
<body>
<div id="grid">
<div class="col">
  <div class="pnl" id="identity">
    <div id="eyes"><div class="eye"></div><div class="eye"></div></div>
    <div id="stateLabel">BOOTING</div>
    <div id="energyWrap"><div id="energyBar"></div></div>
    <div id="connStatus">CONNECTING...</div>
  </div>
  <div class="pnl" id="controls">
    <div id="joy"><div id="joyKnob"></div></div>
    <div class="brow">
      <button class="btn" data-s="25"><i data-lucide="gauge"></i>SLO</button>
      <button class="btn on" data-s="55"><i data-lucide="gauge"></i>MED</button>
      <button class="btn" data-s="100"><i data-lucide="gauge"></i>MAX</button>
    </div>
    <div class="brow">
      <button class="btn on" id="autoBtn">AUTO</button>
      <button class="btn" id="muteBtn"><i data-lucide="volume-2"></i>MUTE</button>
      <button class="btn" id="snapBtn"><i data-lucide="camera"></i>SNAP</button>
    </div>
  </div>
  <div class="pnl" id="telemetry">
    <div class="ph"><i data-lucide="activity"></i>TELEMETRY</div>
    <div class="tr"><span>STATE</span><span class="tv" id="tSt">IDLE</span></div>
    <div class="tr"><span>THR</span><span class="tv" id="tT">0</span></div>
    <div class="tr"><span>TRN</span><span class="tv" id="tS">0</span></div>
    <div class="tr"><span>FLOOR</span><span class="tv" id="tFloor">--%</span></div>
    <div class="tr"><span>CLEAR</span><span class="tv" id="tClear">--</span></div>
    <div class="tr"><span>EDGE%</span><span class="tv" id="tEdge">0</span></div>
    <div class="tr"><span>MOT%</span><span class="tv" id="tMot">0</span></div>
    <div class="tr"><span>ML</span><span class="tv" id="tML">--</span></div>
  </div>
  <div class="pnl" id="tunerPanel">
    <div class="ph" onclick="toggleTuner()"><i data-lucide="sliders-horizontal"></i>TUNER <span id="tunerArrow">&#9656;</span></div>
    <div id="tunerBody">
      <div class="trow"><label>Floor Tol</label><input type="range" min="10" max="50" value="25" oninput="FLOOR_TOL=this.value/10;this.nextElementSibling.textContent=(this.value/10).toFixed(1)"><span>2.5</span></div>
      <div class="trow"><label>Cliff Sens</label><input type="range" min="10" max="60" value="25" oninput="CLIFF_FLOOR_MIN=this.value/100;this.nextElementSibling.textContent=this.value+'%'"><span>25%</span></div>
      <div class="trow"><label>Speed</label><input type="range" min="20" max="100" value="55" oninput="maxSpd=+this.value;this.nextElementSibling.textContent=this.value"><span>55</span></div>
      <div class="trow"><label>Auto Dly</label><input type="range" min="2000" max="15000" value="5000" step="500" oninput="autoResumeMs=+this.value;this.nextElementSibling.textContent=(this.value/1000)+'s'"><span>5s</span></div>
      <div class="trow"><label>Stuck Thr</label><input type="range" min="3" max="15" value="6" oninput="STUCK_THRESH=+this.value;this.nextElementSibling.textContent=this.value"><span>6</span></div>
    </div>
  </div>
</div>
<div class="col">
  <div class="pnl" id="camPanel">
    <div class="ph"><i data-lucide="video"></i>CAMERA</div>
    <img id="feed" crossorigin="anonymous">
  </div>
  <div class="pnl" id="roverPanel">
    <div class="ph"><i data-lucide="eye"></i>ROVER VIEW</div>
    <div id="roverWrap"><canvas id="cv"></canvas></div>
  </div>
  <div class="pnl" id="dialogue">
    <div class="ph"><i data-lucide="message-square"></i>COMMS</div>
    <div id="chatArea"></div>
    <input id="chatInput" type="text" placeholder="Say something..." autocomplete="off">
  </div>
  <div class="pnl" id="logPanel">
    <div class="ph"><i data-lucide="terminal"></i>LOG</div>
  </div>
</div>
<div class="col">
  <div class="pnl pnl-grow" id="mapPanel">
    <div class="ph"><i data-lucide="radar"></i>DISCOVERY MAP</div>
    <div id="map3d"></div>
    <div id="mapInfo">
      <span>EXPLORED <span class="v" id="mapPct">0</span>%</span>
      <span>POS <span class="v" id="mapPos">32,32</span></span>
      <span>HDG <span class="v" id="mapHdg">0</span>&deg;</span>
    </div>
  </div>
  <div class="pnl" id="questPanel">
    <div class="ph" onclick="toggleQuests()"><i data-lucide="scroll-text"></i>MISSIONS <span id="questArrow">&#9656;</span></div>
    <div id="questBody">
      <div id="activeQ"></div>
      <div id="questList"></div>
    </div>
  </div>
</div>
</div>

<script>
// ===== CORE STATE =====
var feed=document.getElementById('feed'),cv=document.getElementById('cv'),
    ctx=cv.getContext('2d',{willReadFrequently:true});
var streamUrl=location.protocol+'//'+location.hostname+':81/stream';
feed.src=streamUrl;
var jT=0,jS=0,maxSpd=55;
var autoOn=true,manualOn=false,manualTmr=null,autoResumeMs=5000;
var energy=1.0;

// ===== WORLD STATE =====
var world={
  floorPct:[0,0,0,0,0],floorTotal:0,
  cliff:false,cliffCols:[false,false,false,false,false],
  blocked:false,clearest:2,motD:0,motDirBias:0,avgEdge:0
};

// ===== FLOOR LEARNING =====
var floorRef=null,floorSamples={r:[],g:[],b:[]},floorFrames=0;
var FLOOR_TOL=2.5,CLIFF_FLOOR_MIN=0.25,STUCK_THRESH=6;

function computeFloorRef(){
  function stats(arr){
    var sum=0,n=arr.length;
    for(var i=0;i<n;i++)sum+=arr[i];
    var mean=sum/n,sqSum=0;
    for(var i=0;i<n;i++)sqSum+=(arr[i]-mean)*(arr[i]-mean);
    return{m:mean,s:Math.max(Math.sqrt(sqSum/n),12)};
  }
  floorRef={r:stats(floorSamples.r),g:stats(floorSamples.g),b:stats(floorSamples.b)};
}

function isFloor(r,g,b){
  if(!floorRef)return false;
  var dr=(r-floorRef.r.m)/floorRef.r.s,dg=(g-floorRef.g.m)/floorRef.g.s,db=(b-floorRef.b.m)/floorRef.b.s;
  return Math.sqrt(dr*dr+dg*dg+db*db)<FLOOR_TOL;
}

// ===== NAV STATE =====
var aState='IDLE',turnDir=1,stuckN=0,cliffTicks=0,cliffFlips=0,scanTicks=0;
var navCv=document.createElement('canvas'),navCtx=navCv.getContext('2d',{willReadFrequently:true});
var NW=80,NH=60;navCv.width=NW;navCv.height=NH;
var prevGray=null,prevFullGray=null;

// ===== MAP =====
var MS=64,map=new Int8Array(MS*MS),mapX=MS/2,mapY=MS/2,heading=0;
var scene3d,cam3d,ren3d,ptGeo,ptPos,ptCol,ptCount=0,MAX_PTS=40000;
var roverMark,camA=0,camP=0.9,camD=20,drag3d=false,dragS={x:0,y:0};
var spinPaused=false,wasDrag3d=false;

// ===== ML =====
var mlModel=null,mlDetections=[];
function loadScript(url,cb){var s=document.createElement('script');s.src=url;s.onload=cb;s.onerror=function(){addLog('CDN load failed');};document.head.appendChild(s);}
function initML(){
  addLog('Loading ML vision...');
  loadScript('https://cdn.jsdelivr.net/npm/@tensorflow/tfjs@4.17.0/dist/tf.min.js',function(){
    loadScript('https://cdn.jsdelivr.net/npm/@tensorflow-models/coco-ssd@2.2.3/dist/coco-ssd.min.js',function(){
      if(!window.cocoSsd){addLog('COCO-SSD not available');return;}
      cocoSsd.load({base:'lite_mobilenet_v2'}).then(function(model){
        mlModel=model;
        addLog('ML VISION ONLINE',1);
        say('Object detection online!','system');
      }).catch(function(e){addLog('ML: '+e.message);});
    });
  });
}
setInterval(function(){
  if(!mlModel||!feed.naturalWidth)return;
  mlModel.detect(feed,6,0.3).then(function(r){mlDetections=r;}).catch(function(){});
},800);

// ===== QUEST =====
var qState={explored:0,cliffs:0,social:0,motions:0,motionT:0,
  patrolStarted:false,patrolFar:false,startX:MS/2,startY:MS/2};
var questIcons={pioneer:'compass',edgewalker:'alert-triangle',watchdog:'shield',motion:'radar',social:'message-circle'};

// ===== WEBSOCKET =====
var ws=null,wsOk=false;
function wsConnect(){
  ws=new WebSocket('ws://'+location.hostname+'/ws');
  ws.onopen=function(){wsOk=true;document.getElementById('connStatus').textContent='WS LINKED';addLog('WS CONNECTED',1);};
  ws.onclose=function(){wsOk=false;document.getElementById('connStatus').textContent='WS ---';setTimeout(wsConnect,1000);};
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

// ===== SOUND =====
var SND={ctx:null,muted:false,last:0,
  init:function(){try{this.ctx=new(window.AudioContext||window.webkitAudioContext)();}catch(e){}},
  chirp:function(f0,f1,dur,type,vol){
    if(this.muted||!this.ctx)return;var n=Date.now();if(n-this.last<200)return;this.last=n;
    var o=this.ctx.createOscillator(),g=this.ctx.createGain();
    o.type=type||'sine';o.frequency.setValueAtTime(f0,this.ctx.currentTime);
    o.frequency.linearRampToValueAtTime(f1,this.ctx.currentTime+dur/1000);
    g.gain.setValueAtTime(vol||0.1,this.ctx.currentTime);
    g.gain.linearRampToValueAtTime(0,this.ctx.currentTime+dur/1000+0.02);
    o.connect(g);g.connect(this.ctx.destination);o.start();o.stop(this.ctx.currentTime+dur/1000+0.03);
  },
  seq:function(notes){var s=this,t=0;notes.forEach(function(n){
    setTimeout(function(){s.chirp(n[0],n[1],n[2],n[3],n[4]);},t);t+=n[2]+30;});},
  play:function(e){if(this.muted||!this.ctx)return;
    switch(e){
    case 'boot':this.seq([[440,880,100,'sine',.08],[660,1320,100,'sine',.08],[880,1760,120,'sine',.1]]);break;
    case 'cliff':this.seq([[900,600,80,'square',.12],[900,600,80,'square',.12]]);break;
    case 'clear':this.chirp(500,800,120,'triangle',.08);break;
    case 'stuck':this.chirp(200,120,300,'sawtooth',.07);break;
    case 'explore':this.chirp(600,700,80,'triangle',.04);break;
    case 'qstart':this.seq([[440,440,100,'triangle',.1],[554,554,100,'triangle',.1],[659,659,100,'triangle',.1],[880,880,200,'triangle',.12]]);break;
    case 'qdone':this.seq([[523,523,80,'sine',.12],[659,659,80,'sine',.12],[784,784,80,'sine',.12],[1047,1047,250,'sine',.15]]);break;
    case 'qstep':this.chirp(1200,1000,80,'sine',.08);break;
    case 'msg':this.chirp(600,900,60,'triangle',.06);break;
    }}};
document.addEventListener('click',function(){if(!SND.ctx){SND.init();SND.play('boot');}});
document.addEventListener('touchstart',function(){if(!SND.ctx)SND.init();});

// ===== EYES =====
var eyeEls=document.querySelectorAll('.eye'),eyeMood='normal';
function setEyes(expr){
  eyeMood=expr;var L=eyeEls[0],R=eyeEls[1];
  L.style.transform=R.style.transform='';
  switch(expr){
    case 'normal':L.style.height=R.style.height='32px';L.style.width=R.style.width='42px';L.style.borderRadius=R.style.borderRadius='8px';break;
    case 'curious':L.style.height='38px';R.style.height='26px';L.style.width=R.style.width='42px';L.style.borderRadius='10px';R.style.borderRadius='8px';break;
    case 'alert':L.style.height=R.style.height='44px';L.style.width=R.style.width='44px';L.style.borderRadius=R.style.borderRadius='12px';break;
    case 'happy':L.style.height=R.style.height='14px';L.style.width=R.style.width='44px';L.style.borderRadius=R.style.borderRadius='10px 10px 20px 20px';break;
    case 'cautious':L.style.height=R.style.height='20px';L.style.width=R.style.width='42px';L.style.borderRadius=R.style.borderRadius='6px';break;
  }
}
setInterval(function(){
  var dx=jS*0.12,dy=-jT*0.1;
  eyeEls.forEach(function(e){e.style.transform='translate('+dx+'px,'+dy+'px)';});
},80);
setInterval(function(){
  if(Math.random()<0.15){eyeEls.forEach(function(e){e.style.height='2px';});
    setTimeout(function(){setEyes(eyeMood);},140);}
},2800);

// ===== DIALOGUE =====
var sayPools={
  exploring:["Desk ahead, looks clear.","Open space.","Moving forward.","Exploring...","Smooth path."],
  obstacle:["Something in the way.","Object detected, going around.","Blocked. Finding a path.","Obstacle. Rerouting.","Need to go around this."],
  cliff:["Edge! Turning.","Desk edge detected.","Drop-off ahead, turning.","That's the edge. Nope.","Finding safer path."],
  clear:["Found a way!","Clear path ahead.","This way.","Going this direction.","Path found."],
  stuck:["I'm stuck.","Can't find a way. Help?","Tight spot...","Need some help here.","Boxed in."],
  curious:["What's that?","Something new.","Hmm, interesting.","Let me look.","Haven't seen this before."]
};
var typeTmr=null,typeI=0,typeTxt='',typeDiv=null;
function say(text,cls){
  var area=document.getElementById('chatArea');
  var div=document.createElement('div');div.className='msg '+(cls||'rover');
  area.appendChild(div);
  if(typeTmr)clearInterval(typeTmr);
  typeDiv=div;typeTxt=text;typeI=0;
  typeTmr=setInterval(function(){typeI++;typeDiv.textContent=typeTxt.substring(0,typeI);
    area.scrollTop=area.scrollHeight;
    if(typeI>=typeTxt.length){clearInterval(typeTmr);typeTmr=null;}},30);
  while(area.children.length>20)area.removeChild(area.firstChild);
}
function sayPool(pool){var p=sayPools[pool];if(p)say(p[Math.floor(Math.random()*p.length)]);}
var lastSayT=0;
function sayThrottled(pool,minGap){var n=Date.now();if(n-lastSayT<(minGap||4000))return;lastSayT=n;sayPool(pool);}

document.getElementById('chatInput').addEventListener('keydown',function(e){
  if(e.key!=='Enter')return;var val=this.value.trim();if(!val)return;this.value='';
  say(val,'human');SND.play('msg');qState.social++;
  var lv=val.toLowerCase();
  if(lv.match(/^(go|forward|move)/)){say("Moving!","rover");jT=30;jS=0;goManual(3000);startCmd();}
  else if(lv.match(/^(stop|wait|stay)/)){say("Stopping.","rover");goManual(10000);jT=0;jS=0;}
  else if(lv.match(/look.*left|turn.*left/)){say("Looking left.","rover");jT=0;jS=-30;goManual(2000);startCmd();}
  else if(lv.match(/look.*right|turn.*right/)){say("Looking right.","rover");jT=0;jS=30;goManual(2000);startCmd();}
  else if(lv.match(/quest|mission/)){say("Check the missions panel!","rover");}
  else if(lv.match(/hello|hi |hey/)){setEyes('happy');say("Hi! Ready to explore.","rover");setTimeout(function(){setEyes('curious');},2000);}
  else{var r=["Got it.","Interesting!","*chirp*","Noted.","Tell me more."];
    say(r[Math.floor(Math.random()*r.length)],'rover');}
});

// ===== QUESTS =====
var quests=[
  {id:'pioneer',name:'THE PIONEER',desc:'Explore 10 new map cells',
    steps:[{d:'Visit 5 cells',c:function(){return qState.explored>=5;}},
           {d:'Visit 10 cells',c:function(){return qState.explored>=10;}}],
    step:0,on:false,done:false,ok:true},
  {id:'edgewalker',name:'EDGEWALKER',desc:'Detect 3 cliffs and survive',
    steps:[{d:'Survive 1 cliff',c:function(){return qState.cliffs>=1;}},
           {d:'Survive 2 cliffs',c:function(){return qState.cliffs>=2;}},
           {d:'Survive 3 cliffs',c:function(){return qState.cliffs>=3;}}],
    step:0,on:false,done:false,ok:false},
  {id:'watchdog',name:'THE WATCHDOG',desc:'Venture out and return to base',
    steps:[{d:'Go 8+ cells from start',c:function(){return qState.patrolFar;}},
           {d:'Return within 3 cells',c:function(){
             var dx=mapX-qState.startX,dy=mapY-qState.startY;
             return qState.patrolFar&&Math.sqrt(dx*dx+dy*dy)<3;}}],
    step:0,on:false,done:false,ok:false},
  {id:'motion',name:'MOTION HUNTER',desc:'Detect 5 motion events',
    steps:[{d:'Detect 1 motion',c:function(){return qState.motions>=1;}},
           {d:'Detect 3 motions',c:function(){return qState.motions>=3;}},
           {d:'Detect 5 motions',c:function(){return qState.motions>=5;}}],
    step:0,on:false,done:false,ok:true},
  {id:'social',name:'SOCIAL BUTTERFLY',desc:'Receive 5 human messages',
    steps:[{d:'Say hello',c:function(){return qState.social>=1;}},
           {d:'Chat (3 msgs)',c:function(){return qState.social>=3;}},
           {d:'Become friends (5)',c:function(){return qState.social>=5;}}],
    step:0,on:false,done:false,ok:true}
];
function startQuest(id){
  quests.forEach(function(q){if(q.id===id&&q.ok&&!q.done){q.on=true;q.step=0;
    say('Mission: '+q.name+'! '+q.desc,'quest');SND.play('qstart');addLog('QUEST: '+q.name,0,'q');
    if(q.id==='watchdog'){qState.startX=mapX;qState.startY=mapY;qState.patrolStarted=true;qState.patrolFar=false;}
  }});renderQ();
}
function checkQuests(){
  var ex=0;for(var i=0;i<MS*MS;i++)if(map[i]>0)ex++;
  qState.explored=ex;document.getElementById('mapPct').textContent=Math.round(ex/(MS*MS)*100);
  if(qState.patrolStarted){var dx=mapX-qState.startX,dy=mapY-qState.startY;
    if(Math.sqrt(dx*dx+dy*dy)>=8)qState.patrolFar=true;}
  quests.forEach(function(q){if(!q.on||q.done)return;
    while(q.step<q.steps.length&&q.steps[q.step].c()){q.step++;SND.play('qstep');
      say('Step '+q.step+'/'+q.steps.length+' done!','quest');addLog('QUEST STEP: '+q.name,0,'q');}
    if(q.step>=q.steps.length){q.done=true;q.on=false;
      say('QUEST COMPLETE: '+q.name+'!','quest');SND.play('qdone');
      setEyes('happy');setTimeout(function(){setEyes('curious');},3000);
      addLog('QUEST DONE: '+q.name,1,'q');
      if(q.id==='pioneer')quests.forEach(function(q2){if(q2.id==='edgewalker'||q2.id==='watchdog')q2.ok=true;});}
  });renderQ();
}
function renderQ(){
  var aq=document.getElementById('activeQ'),ql=document.getElementById('questList');
  aq.innerHTML='';ql.innerHTML='';
  quests.forEach(function(q){
    if(q.on){var p=q.steps.length>0?q.step/q.steps.length*100:0;
      aq.innerHTML='<div class="qcard active"><div class="qn"><i data-lucide="'+
        (questIcons[q.id]||'circle')+'"></i>'+q.name+'</div><div class="qd">'+q.desc+
        '</div><div class="qp"><div class="qb" style="width:'+p+'%"></div></div><div class="qs"><i data-lucide="flag"></i>'+
        (q.step<q.steps.length?q.steps[q.step].d:'COMPLETE')+'</div></div>';}
    var it=document.createElement('div');
    it.className='qi'+(q.on?' act':'')+(q.done?' done':'')+(!q.ok&&!q.done?' locked':'');
    var icon=q.done?'check-circle':(!q.ok&&!q.done?'lock':(questIcons[q.id]||'circle'));
    it.innerHTML='<i data-lucide="'+icon+'"></i>'+q.name;
    if(q.ok&&!q.done&&!q.on)it.onclick=(function(id){return function(){startQuest(id);};})(q.id);
    ql.appendChild(it);
  });
  if(typeof lucide!=='undefined')lucide.createIcons();
}
renderQ();

// ===== TOGGLES =====
function toggleTuner(){
  var b=document.getElementById('tunerBody'),a=document.getElementById('tunerArrow');
  if(b.style.display==='none'){b.style.display='block';a.innerHTML='&#9662;';}
  else{b.style.display='none';a.innerHTML='&#9656;';}
}
function toggleQuests(){
  var b=document.getElementById('questBody'),a=document.getElementById('questArrow');
  if(b.style.display==='none'){b.style.display='block';a.innerHTML='&#9662;';}
  else{b.style.display='none';a.innerHTML='&#9656;';}
}

// ===== JOYSTICK =====
var joy=document.getElementById('joy'),knob=document.getElementById('joyKnob'),jRect=null,touching=false;
function goManual(dur){manualOn=true;clearTimeout(manualTmr);
  manualTmr=setTimeout(function(){manualOn=false;addLog('AUTO resumed');},dur||autoResumeMs);}
function jCalc(tx,ty){if(!jRect)jRect=joy.getBoundingClientRect();
  var cx=jRect.left+jRect.width/2,cy=jRect.top+jRect.height/2,dx=tx-cx,dy=ty-cy;
  var dist=Math.sqrt(dx*dx+dy*dy),mr=jRect.width/2-18;
  if(dist>mr){dx=dx/dist*mr;dy=dy/dist*mr;}
  knob.style.transform='translate(calc(-50% + '+dx+'px),calc(-50% + '+dy+'px))';
  jS=Math.round(dx/mr*maxSpd);jT=Math.round(-dy/mr*maxSpd);}
function jEnd(){touching=false;knob.style.transform='translate(-50%,-50%)';jT=0;jS=0;jRect=null;
  if(manualOn)goManual(autoResumeMs);if(!autoOn)stopCmd();}
joy.ontouchstart=function(e){e.preventDefault();touching=true;goManual();
  jRect=joy.getBoundingClientRect();jCalc(e.touches[0].clientX,e.touches[0].clientY);startCmd();};
joy.ontouchmove=function(e){if(!touching)return;e.preventDefault();jCalc(e.touches[0].clientX,e.touches[0].clientY);};
joy.ontouchend=function(e){e.preventDefault();jEnd();};
joy.ontouchcancel=function(){jEnd();};
joy.onmousedown=function(e){e.preventDefault();touching=true;goManual();
  jRect=joy.getBoundingClientRect();jCalc(e.clientX,e.clientY);startCmd();};
document.onmousemove=function(e){if(touching)jCalc(e.clientX,e.clientY);};
document.onmouseup=function(){if(touching)jEnd();};

// ===== KEYBOARD =====
var keys={};
document.onkeydown=function(e){if(e.target.id==='chatInput')return;
  var k=e.key.toLowerCase();
  if(k==='arrowup')k='w';if(k==='arrowdown')k='s';if(k==='arrowleft')k='a';if(k==='arrowright')k='d';
  if('wasd'.indexOf(k)<0)return;e.preventDefault();if(keys[k])return;keys[k]=true;goManual();kDrv();};
document.onkeyup=function(e){if(e.target.id==='chatInput')return;
  var k=e.key.toLowerCase();
  if(k==='arrowup')k='w';if(k==='arrowdown')k='s';if(k==='arrowleft')k='a';if(k==='arrowright')k='d';
  if('wasd'.indexOf(k)<0)return;e.preventDefault();keys[k]=false;kDrv();};
function kDrv(){var t=0,s=0;
  if(keys.w)t+=maxSpd;if(keys.s)t-=maxSpd;if(keys.a)s-=maxSpd;if(keys.d)s+=maxSpd;
  jT=t;jS=s;if(t||s)startCmd();else{goManual(autoResumeMs);if(!autoOn)stopCmd();}}

// ===== CONTROLS =====
document.querySelector('.brow').onclick=function(e){
  var btn=e.target.closest('[data-s]');if(!btn)return;
  maxSpd=parseInt(btn.dataset.s);
  this.querySelectorAll('.btn').forEach(function(b){b.classList.toggle('on',b===btn);});};
document.getElementById('autoBtn').onclick=function(){autoOn=!autoOn;this.classList.toggle('on',autoOn);
  if(!autoOn){stopCmd();aState='IDLE';addLog('MANUAL MODE');}
  else{resetMap();aState='LEARN';floorRef=null;floorFrames=0;floorSamples={r:[],g:[],b:[]};startCmd();addLog('AUTO ENGAGED',1);}};
document.getElementById('muteBtn').onclick=function(){SND.muted=!SND.muted;
  this.classList.toggle('on',SND.muted);this.innerHTML=SND.muted?'<i data-lucide="volume-x"></i>UNMUTE':'<i data-lucide="volume-2"></i>MUTE';
  if(typeof lucide!=='undefined')lucide.createIcons();};
document.getElementById('snapBtn').onclick=function(){
  if(!feed.naturalWidth){addLog('NO FRAME');return;}
  var sc=document.createElement('canvas');sc.width=feed.naturalWidth;sc.height=feed.naturalHeight;
  try{sc.getContext('2d').drawImage(feed,0,0);
    var a=document.createElement('a');a.download='explorer-'+Date.now()+'.png';a.href=sc.toDataURL();a.click();addLog('SNAP SAVED',1);}
  catch(e){addLog('SNAP FAILED');}};

// ===== LOG =====
var logP=document.getElementById('logPanel');
function addLog(m,hi,type){var d=document.createElement('div');
  d.className='log'+(hi?' hi':'')+(type==='q'?' qlog':'');
  d.innerHTML='<span class="ts">'+new Date().toTimeString().slice(0,8)+'</span>'+m;
  logP.appendChild(d);while(logP.children.length>15)logP.removeChild(logP.children[1]);
  logP.scrollTop=logP.scrollHeight;}

// ===== STATUS =====
function poll(){fetch('/status').then(function(r){return r.json();}).then(function(d){
  document.getElementById('connStatus').textContent=d.wifi?d.ip:'AP MODE';
}).catch(function(){document.getElementById('connStatus').textContent='NO LINK';});}
setInterval(poll,3000);

// ===== 3D MAP =====
function resetMap(){
  map.fill(0);mapX=MS/2;mapY=MS/2;heading=0;
  ptCount=0;if(ptGeo){ptGeo.setDrawRange(0,0);}
}

function init3D(){
  var c=document.getElementById('map3d');
  if(!window.THREE){c.textContent='Loading 3D...';setTimeout(init3D,500);return;}
  c.textContent='';
  scene3d=new THREE.Scene();
  scene3d.fog=new THREE.FogExp2(0x080808,0.025);
  cam3d=new THREE.PerspectiveCamera(50,c.clientWidth/Math.max(c.clientHeight,1),0.1,200);
  ren3d=new THREE.WebGLRenderer({antialias:true});
  ren3d.setSize(c.clientWidth,c.clientHeight);
  ren3d.setPixelRatio(Math.min(window.devicePixelRatio,2));
  ren3d.setClearColor(0x080808);
  c.appendChild(ren3d.domElement);

  ptPos=new Float32Array(MAX_PTS*3);ptCol=new Float32Array(MAX_PTS*3);
  ptGeo=new THREE.BufferGeometry();
  ptGeo.setAttribute('position',new THREE.BufferAttribute(ptPos,3));
  ptGeo.setAttribute('color',new THREE.BufferAttribute(ptCol,3));
  ptGeo.setDrawRange(0,0);
  scene3d.add(new THREE.Points(ptGeo,new THREE.PointsMaterial({size:0.15,vertexColors:true,transparent:true,opacity:0.85,sizeAttenuation:true})));
  scene3d.add(new THREE.GridHelper(32,64,0x111111,0x0a0a0a));

  var rGeo=new THREE.ConeGeometry(0.25,0.7,4);rGeo.rotateX(Math.PI/2);
  roverMark=new THREE.Mesh(rGeo,new THREE.MeshBasicMaterial({color:0xFF6A00}));
  scene3d.add(roverMark);
  var bGeo=new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(0,0,0),new THREE.Vector3(0,0.1,-3)]);
  roverMark.add(new THREE.Line(bGeo,new THREE.LineBasicMaterial({color:0x3388FF,transparent:true,opacity:0.4})));

  for(var i=0;i<15;i++){if(ptCount>=MAX_PTS)break;var idx=ptCount*3;
    ptPos[idx]=(Math.random()-0.5)*0.4;ptPos[idx+1]=Math.random()*0.3;ptPos[idx+2]=(Math.random()-0.5)*0.4;
    ptCol[idx]=0.3;ptCol[idx+1]=1.0;ptCol[idx+2]=0.3;ptCount++;}
  ptGeo.attributes.position.needsUpdate=true;ptGeo.attributes.color.needsUpdate=true;ptGeo.setDrawRange(0,ptCount);

  var el=ren3d.domElement;
  el.addEventListener('mousedown',function(e){drag3d=true;wasDrag3d=false;dragS={x:e.clientX,y:e.clientY};});
  window.addEventListener('mousemove',function(e){if(!drag3d)return;
    if(Math.abs(e.clientX-dragS.x)>3||Math.abs(e.clientY-dragS.y)>3)wasDrag3d=true;
    camA+=(e.clientX-dragS.x)*0.008;camP=Math.max(0.15,Math.min(1.4,camP-(e.clientY-dragS.y)*0.008));
    dragS={x:e.clientX,y:e.clientY};});
  window.addEventListener('mouseup',function(){drag3d=false;});
  el.addEventListener('click',function(){if(!wasDrag3d)spinPaused=!spinPaused;});
  el.addEventListener('wheel',function(e){camD=Math.max(5,Math.min(50,camD+e.deltaY*0.03));e.preventDefault();},{passive:false});
  el.addEventListener('touchstart',function(e){if(e.touches.length===1){drag3d=true;wasDrag3d=false;dragS={x:e.touches[0].clientX,y:e.touches[0].clientY};}},{passive:true});
  el.addEventListener('touchmove',function(e){if(!drag3d||e.touches.length!==1)return;wasDrag3d=true;
    camA+=(e.touches[0].clientX-dragS.x)*0.008;camP=Math.max(0.15,Math.min(1.4,camP-(e.touches[0].clientY-dragS.y)*0.008));
    dragS={x:e.touches[0].clientX,y:e.touches[0].clientY};},{passive:true});
  el.addEventListener('touchend',function(){drag3d=false;if(!wasDrag3d)spinPaused=!spinPaused;},{passive:true});

  if(window.ResizeObserver){new ResizeObserver(function(){if(!ren3d)return;
    cam3d.aspect=c.clientWidth/Math.max(c.clientHeight,1);cam3d.updateProjectionMatrix();
    ren3d.setSize(c.clientWidth,c.clientHeight);}).observe(c);}
  anim3D();
}

function anim3D(){
  requestAnimationFrame(anim3D);if(!scene3d)return;
  if(!drag3d&&!spinPaused)camA+=0.002;
  var tx=(mapX-MS/2)*0.5,tz=(mapY-MS/2)*0.5;
  cam3d.position.set(tx+Math.cos(camA)*camD*Math.cos(camP),camD*Math.sin(camP),tz+Math.sin(camA)*camD*Math.cos(camP));
  cam3d.lookAt(tx,0,tz);
  if(roverMark){roverMark.position.set(tx,0.4,tz);roverMark.rotation.y=-heading;}
  ren3d.render(scene3d,cam3d);
}

function addPts3D(gx,gy,type){
  var bx=(gx-MS/2)*0.5,bz=(gy-MS/2)*0.5;
  var n=type==='obstacle'?10:type==='cliff'?8:5;
  for(var i=0;i<n;i++){if(ptCount>=MAX_PTS)return;var idx=ptCount*3;
    ptPos[idx]=bx+(Math.random()-0.5)*0.45;
    ptPos[idx+1]=type==='obstacle'?Math.random()*0.5+0.05:Math.random()*0.06;
    ptPos[idx+2]=bz+(Math.random()-0.5)*0.45;
    if(type==='floor'){ptCol[idx]=0.05+Math.random()*0.1;ptCol[idx+1]=0.5+Math.random()*0.3;ptCol[idx+2]=0.7+Math.random()*0.3;}
    else if(type==='obstacle'){ptCol[idx]=0.7+Math.random()*0.3;ptCol[idx+1]=0.1+Math.random()*0.1;ptCol[idx+2]=0.05;}
    else{ptCol[idx]=1.0;ptCol[idx+1]=0.7+Math.random()*0.3;ptCol[idx+2]=0.0;}
    ptCount++;}
  ptGeo.attributes.position.needsUpdate=true;ptGeo.attributes.color.needsUpdate=true;ptGeo.setDrawRange(0,ptCount);
}

// ===== BUILD WORLD STATE =====
function buildWorld(d,gray){
  var colW=NW/5,startRow=Math.floor(NH*0.33);
  var colFloor=[0,0,0,0,0],colTotal=[0,0,0,0,0];
  for(var y=startRow;y<NH;y++){
    for(var x=0;x<NW;x++){
      var ci=Math.min(4,Math.floor(x/colW));
      var pi=(y*NW+x)*4;colTotal[ci]++;
      if(isFloor(d[pi],d[pi+1],d[pi+2]))colFloor[ci]++;
    }
  }
  var totalF=0,totalP=0;
  for(var c=0;c<5;c++){
    world.floorPct[c]=colTotal[c]>0?colFloor[c]/colTotal[c]:0;
    totalF+=colFloor[c];totalP+=colTotal[c];
  }
  world.floorTotal=totalP>0?totalF/totalP:0;

  // Edge density per column (Sobel on nav grayscale)
  var colEdge=[0,0,0,0,0],colET=[0,0,0,0,0];
  for(var ey=startRow+1;ey<NH-1;ey++){
    for(var ex=1;ex<NW-1;ex++){
      var eci=Math.min(4,Math.floor(ex/colW));
      var gx=-gray[(ey-1)*NW+ex-1]-2*gray[ey*NW+ex-1]-gray[(ey+1)*NW+ex-1]
             +gray[(ey-1)*NW+ex+1]+2*gray[ey*NW+ex+1]+gray[(ey+1)*NW+ex+1];
      var gy2=-gray[(ey-1)*NW+ex-1]-2*gray[(ey-1)*NW+ex]-gray[(ey-1)*NW+ex+1]
              +gray[(ey+1)*NW+ex-1]+2*gray[(ey+1)*NW+ex]+gray[(ey+1)*NW+ex+1];
      colET[eci]++;
      if(Math.abs(gx)+Math.abs(gy2)>60)colEdge[eci]++;
    }
  }
  var totalEdge=0;
  for(var ec=0;ec<5;ec++){
    var ePct=colET[ec]>0?colEdge[ec]/colET[ec]:0;
    totalEdge+=ePct;
    // High floor + high edges = floor-colored object
    if(ePct>0.15&&world.floorPct[ec]>0.4) world.floorPct[ec]*=0.5;
  }
  world.avgEdge=totalEdge/5;

  // ML detections reduce floor confidence
  if(mlDetections.length>0){
    var fw=feed.naturalWidth||320,fh=feed.naturalHeight||240;
    mlDetections.forEach(function(det){
      var objBot=(det.bbox[1]+det.bbox[3])/fh;
      var objCX=(det.bbox[0]+det.bbox[2]/2)/fw;
      var objW=det.bbox[2]/fw;
      if(objBot>0.4&&objW>0.08){
        var col=Math.min(4,Math.floor(objCX*5));
        world.floorPct[col]*=0.3;
      }
    });
  }

  // Recalculate total after adjustments
  totalF=0;totalP=0;
  for(var rc=0;rc<5;rc++){totalF+=world.floorPct[rc];totalP++;}
  world.floorTotal=totalF/totalP;

  // Cliff
  var bottomFloor=0,bottomTotal=0;
  for(var by=NH-10;by<NH;by++){
    for(var bx=Math.floor(NW*0.2);bx<Math.floor(NW*0.8);bx++){
      var bpi=(by*NW+bx)*4;bottomTotal++;
      if(isFloor(d[bpi],d[bpi+1],d[bpi+2]))bottomFloor++;
    }
  }
  world.cliff=bottomTotal>0?(bottomFloor/bottomTotal)<CLIFF_FLOOR_MIN:false;

  for(var cc=0;cc<5;cc++){
    var cf=0,ct=0;
    for(var cy=NH-10;cy<NH;cy++){
      for(var cx=Math.floor(cc*colW);cx<Math.floor((cc+1)*colW);cx++){
        var cpi=(cy*NW+cx)*4;ct++;
        if(isFloor(d[cpi],d[cpi+1],d[cpi+2]))cf++;
      }
    }
    world.cliffCols[cc]=ct>0?(cf/ct)<CLIFF_FLOOR_MIN:false;
  }

  world.blocked=world.floorPct[2]<0.3;
  var best=0,bestI=2;
  for(var bc=0;bc<5;bc++){if(world.floorPct[bc]>best){best=world.floorPct[bc];bestI=bc;}}
  world.clearest=bestI;

  world.motD=0;
  if(prevGray&&prevGray.length===gray.length){
    var tot=0;for(var mi=0;mi<gray.length;mi++)if(Math.abs(gray[mi]-prevGray[mi])>20)tot++;
    world.motD=tot/gray.length*100;
  }
  if(prevGray&&prevGray.length===gray.length){
    var motL=0,motR=0;
    for(var my=0;my<NH;my++)for(var mx=0;mx<NW;mx++){
      if(Math.abs(gray[my*NW+mx]-prevGray[my*NW+mx])>20){if(mx<NW/2)motL++;else motR++;}}
    world.motDirBias=(motR-motL)/(motL+motR+1);
  }
}

// ===== DECIDE =====
var lastDecisionLog=0;
function decide(){
  var t=0,s=0,dirs=['L','LC','C','RC','R'];

  document.getElementById('tSt').textContent=aState;
  document.getElementById('tT').textContent=jT;
  document.getElementById('tS').textContent=jS;
  document.getElementById('tFloor').textContent=Math.round(world.floorTotal*100)+'%';
  document.getElementById('tClear').textContent=dirs[world.clearest]+' '+Math.round(world.floorPct[world.clearest]*100)+'%';
  document.getElementById('tEdge').textContent=Math.round(world.avgEdge*100);
  document.getElementById('tMot').textContent=Math.round(world.motD);
  document.getElementById('tML').textContent=mlDetections.length>0?mlDetections.map(function(d){return d.class;}).join(','):'--';

  if(aState==='CLIFF')setEyes('alert');
  else if(aState==='AVOID')setEyes('cautious');
  else if(stuckN>STUCK_THRESH)setEyes('cautious');
  else if(aState==='FWD')setEyes('curious');
  else setEyes('normal');

  var stateNames={LEARN:'Learning floor...',SCAN:'Scanning...',FWD:'Exploring',
    CLIFF:'Edge detected!',AVOID:'Going around',REV:'Reversing',IDLE:'Idle'};
  document.getElementById('stateLabel').textContent=stateNames[aState]||aState;

  if(jT!==0||jS!==0)energy=Math.max(0,energy-0.002);
  else energy=Math.min(1,energy+0.005);
  document.getElementById('energyBar').style.width=(energy*100)+'%';

  if(world.motD<0.5&&aState==='FWD')stuckN++;else if(aState==='FWD')stuckN=0;

  if(aState==='CLIFF'){
    t=0;s=turnDir*25;cliffTicks--;
    if(cliffTicks<=0){
      if(world.cliff){
        if(cliffFlips>=2){t=-20;s=0;cliffTicks=3;sayThrottled('stuck',8000);SND.play('stuck');}
        else{turnDir=-turnDir;cliffTicks=5;cliffFlips++;}
      }else{aState='SCAN';cliffFlips=0;scanTicks=0;SND.play('clear');sayThrottled('clear',5000);}
    }
  }
  else if(world.cliff&&aState!=='LEARN'){
    aState='CLIFF';cliffTicks=5;cliffFlips=0;
    turnDir=world.clearest<2?-1:1;t=0;s=0;
    SND.play('cliff');sayThrottled('cliff',3000);qState.cliffs++;
    addPts3D(Math.round(mapX),Math.round(mapY),'cliff');
    addLog('CLIFF: turning '+dirs[world.clearest]);
  }
  else if(aState==='SCAN'){
    t=0;s=20*turnDir;scanTicks++;
    if(world.floorPct[2]>0.5&&!world.cliff){aState='FWD';stuckN=0;scanTicks=0;}
    if(scanTicks>16){turnDir=-turnDir;scanTicks=0;}
  }
  else if(aState==='FWD'){
    if(world.blocked||world.cliff){
      aState='AVOID';
      turnDir=world.clearest<2?-1:world.clearest>2?1:(Math.random()>0.5?1:-1);
      sayThrottled('obstacle',5000);
    }
    else if(stuckN>STUCK_THRESH){aState='REV';sayThrottled('stuck',8000);SND.play('stuck');}
    else{
      t=30;var steer=(world.clearest-2)*8;
      if(world.floorPct[0]<0.3&&world.floorPct[4]>0.5)steer+=6;
      else if(world.floorPct[4]<0.3&&world.floorPct[0]>0.5)steer-=6;
      s=steer;
    }
  }
  else if(aState==='AVOID'){
    t=-10;s=35*turnDir;
    if(world.floorPct[2]>0.5&&!world.cliff&&!world.blocked){aState='FWD';stuckN=0;}
  }
  else if(aState==='REV'){
    t=-25;s=20*turnDir;stuckN--;
    if(stuckN<=0||(!world.blocked&&world.floorPct[2]>0.4)){aState='SCAN';stuckN=0;scanTicks=0;}
  }
  else{aState='SCAN';scanTicks=0;}

  jT=t;jS=s;
  if(Date.now()-lastDecisionLog>3000){lastDecisionLog=Date.now();
    var pcts=world.floorPct.map(function(p){return Math.round(p*100);});
    addLog(aState+' floor:['+pcts.join(',')+'] edge:'+Math.round(world.avgEdge*100)+'% clear:'+dirs[world.clearest]);}
}

// ===== HEARTBEAT (200ms) =====
setInterval(function(){
  if(!feed.naturalWidth)return;
  try{navCtx.drawImage(feed,0,0,NW,NH);}catch(e){return;}
  var img=navCtx.getImageData(0,0,NW,NH),d=img.data;
  var gray=new Uint8Array(NW*NH);
  for(var i=0;i<d.length;i+=4)gray[i>>2]=(d[i]*77+d[i+1]*150+d[i+2]*29)>>8;

  if(aState==='LEARN'){
    for(var fy=Math.floor(NH*0.4);fy<NH;fy++)for(var fx=Math.floor(NW*0.1);fx<Math.floor(NW*0.9);fx++){
      var fi=(fy*NW+fx)*4;
      floorSamples.r.push(d[fi]);floorSamples.g.push(d[fi+1]);floorSamples.b.push(d[fi+2]);
    }
    floorFrames++;
    if(floorFrames>=10){
      computeFloorRef();aState='SCAN';scanTicks=0;
      say('Floor learned! RGB('+Math.round(floorRef.r.m)+','+Math.round(floorRef.g.m)+','+Math.round(floorRef.b.m)+') std:'+Math.round(floorRef.r.s),'system');
      addLog('FLOOR: R='+Math.round(floorRef.r.m)+'+-'+Math.round(floorRef.r.s)+
        ' G='+Math.round(floorRef.g.m)+'+-'+Math.round(floorRef.g.s)+
        ' B='+Math.round(floorRef.b.m)+'+-'+Math.round(floorRef.b.s));
    }
    jT=0;jS=0;prevGray=gray;return;
  }

  buildWorld(d,gray);
  if(world.motD>10&&Date.now()-qState.motionT>5000){qState.motions++;qState.motionT=Date.now();}

  heading+=jS*0.003;var dist=jT*0.006;
  mapX+=dist*Math.sin(heading);mapY-=dist*Math.cos(heading);
  mapX=Math.max(1,Math.min(MS-2,mapX));mapY=Math.max(1,Math.min(MS-2,mapY));
  var mi=Math.round(mapY)*MS+Math.round(mapX);
  if(mi>=0&&mi<MS*MS){if(map[mi]===0){map[mi]=1;addPts3D(Math.round(mapX),Math.round(mapY),'floor');}}
  if(world.blocked){var ox=mapX+Math.sin(heading)*2,oy=mapY-Math.cos(heading)*2;
    var oi=Math.round(oy)*MS+Math.round(ox);
    if(oi>=0&&oi<MS*MS){if(map[oi]!==-1)addPts3D(Math.round(ox),Math.round(oy),'obstacle');map[oi]=-1;}}

  document.getElementById('mapPos').textContent=Math.round(mapX)+','+Math.round(mapY);
  document.getElementById('mapHdg').textContent=Math.round(heading*180/Math.PI);

  if(autoOn&&!manualOn){decide();startCmd();}
  prevGray=gray;checkQuests();
},200);

// ===== ROVER VIEW (12fps, point cloud + edges + ML boxes) =====
var rvF=0;
setInterval(function(){
  if(!feed.naturalWidth||!floorRef)return;
  var w=feed.naturalWidth,h=feed.naturalHeight;
  if(cv.width!==w)cv.width=w;if(cv.height!==h)cv.height=h;
  try{ctx.drawImage(feed,0,0);}catch(e){return;}
  var src=ctx.getImageData(0,0,w,h),sd=src.data;

  var curGray=new Uint8Array(w*h);
  for(var gi=0;gi<sd.length;gi+=4)curGray[gi>>2]=(sd[gi]*77+sd[gi+1]*150+sd[gi+2]*29)>>8;

  // Point cloud on black
  var out=ctx.createImageData(w,h),od=out.data;
  rvF++;
  for(var y=0;y<h;y+=2){
    for(var x=0;x<w;x+=2){
      if((x*7+y*13+rvF)%5===0)continue;
      var si=(y*w+x)*4;
      var br=(sd[si]*77+sd[si+1]*150+sd[si+2]*29)>>8;
      var bf=0.4+br/425;
      var mot=prevFullGray&&Math.abs(curGray[y*w+x]-prevFullGray[y*w+x])>25;
      var r,g,b;
      if(mot){r=255;g=240;b=50;}
      else if(isFloor(sd[si],sd[si+1],sd[si+2])){r=(30*bf)|0;g=Math.min(255,(200*bf)|0);b=(100*bf)|0;}
      else{r=Math.min(255,(200*bf)|0);g=(40*bf)|0;b=(30*bf)|0;}
      for(var dy=0;dy<2&&y+dy<h;dy++)for(var dx=0;dx<2&&x+dx<w;dx++){
        var di=((y+dy)*w+(x+dx))*4;od[di]=r;od[di+1]=g;od[di+2]=b;od[di+3]=210;}
    }
  }

  // Sobel edge overlay (cyan edges on top of point cloud)
  for(var ey=1;ey<h-1;ey+=2){
    for(var ex=1;ex<w-1;ex+=2){
      var sgx=-curGray[(ey-1)*w+ex-1]-2*curGray[ey*w+ex-1]-curGray[(ey+1)*w+ex-1]
              +curGray[(ey-1)*w+ex+1]+2*curGray[ey*w+ex+1]+curGray[(ey+1)*w+ex+1];
      var sgy=-curGray[(ey-1)*w+ex-1]-2*curGray[(ey-1)*w+ex]-curGray[(ey-1)*w+ex+1]
              +curGray[(ey+1)*w+ex-1]+2*curGray[(ey+1)*w+ex]+curGray[(ey+1)*w+ex+1];
      if(Math.abs(sgx)+Math.abs(sgy)>80){
        var ei=(ey*w+ex)*4;od[ei]=100;od[ei+1]=255;od[ei+2]=255;od[ei+3]=200;
      }
    }
  }

  ctx.putImageData(out,0,0);
  prevFullGray=curGray;

  // ML bounding boxes
  if(mlDetections.length>0){
    ctx.strokeStyle='#00FFFF';ctx.lineWidth=2;ctx.font='bold 9px monospace';
    mlDetections.forEach(function(det){
      ctx.strokeRect(det.bbox[0],det.bbox[1],det.bbox[2],det.bbox[3]);
      ctx.fillStyle='rgba(0,0,0,0.6)';ctx.fillRect(det.bbox[0],det.bbox[1]-12,det.bbox[2],12);
      ctx.fillStyle='#00FFFF';
      ctx.fillText(det.class.toUpperCase()+' '+Math.round(det.score*100)+'%',det.bbox[0]+2,det.bbox[1]-3);
    });
  }

  // Confidence bars
  var barW=w/5,barH=25;
  for(var bi=0;bi<5;bi++){
    var pct=world.floorPct[bi],bh=pct*barH,bx=bi*barW;
    ctx.fillStyle=pct>0.5?'rgba(0,200,80,0.5)':'rgba(255,60,60,0.5)';
    ctx.fillRect(bx+1,h-bh,barW-2,bh);
    ctx.fillStyle='#fff';ctx.font='bold 9px monospace';
    ctx.fillText(Math.round(pct*100)+'%',bx+barW/2-12,h-bh-3);
    if(world.cliffCols[bi]){ctx.strokeStyle='#F00';ctx.lineWidth=2;ctx.strokeRect(bx,h-barH,barW,barH);}
  }
  var ax=w/2,ay=h-30,atx=w/2+(world.clearest-2)*(w/6),aty=h-55;
  ctx.strokeStyle='#FF6A00';ctx.lineWidth=2;ctx.beginPath();ctx.moveTo(ax,ay);ctx.lineTo(atx,aty);ctx.stroke();
  ctx.fillStyle='#FF6A00';ctx.beginPath();ctx.moveTo(atx,aty-5);ctx.lineTo(atx-5,aty+5);ctx.lineTo(atx+5,aty+5);ctx.closePath();ctx.fill();
  ctx.fillStyle='rgba(0,0,0,0.5)';ctx.fillRect(0,0,w,16);
  ctx.fillStyle='#FF6A00';ctx.font='bold 10px monospace';
  ctx.fillText(aState+' | Floor: '+Math.round(world.floorTotal*100)+'%'+(world.cliff?' | CLIFF!':'')+(mlDetections.length?' | ML:'+mlDetections.length:''),4,12);
},83);

// ===== BOOT =====
addLog('SYSTEMS ONLINE',1);
say("Ready to explore. Learning floor...","system");
setEyes('curious');
if(autoOn){aState='LEARN';startCmd();}
setTimeout(function(){poll();if(typeof lucide!=='undefined')lucide.createIcons();init3D();initML();},500);
</script>
</body>
</html>
)rawliteral";
