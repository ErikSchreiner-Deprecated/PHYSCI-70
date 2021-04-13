let serial;
let latestData = "waiting for data";
let xPos = 0;
let yPos = 0;
let button = 1;

function setup() {
 createCanvas(4095/10, 4095/10);

 serial = new p5.SerialPort();

 serial.list();
 serial.open('COM8');

 serial.on('connected', serverConnected);

 serial.on('list', gotList);

 serial.on('data', gotData);

 serial.on('error', gotError);

 serial.on('open', gotOpen);

 serial.on('close', gotClose);
}

function serverConnected() {
 print("Connected to Server");
}

function gotList(thelist) {
 print("List of Serial Ports:");

 for (let i = 0; i < thelist.length; i++) {
  print(i + " " + thelist[i]);
 }
}

function gotOpen() {
 print("Serial Port is Open");
}

function gotClose(){
 print("Serial Port is Closed");
 latestData = "Serial Port is Closed";
}

function gotError(theerror) {
 print(theerror);
}

function gotData() {
 let currentString = serial.readLine();
  trim(currentString);
 if (!currentString) return;
 console.log(currentString);
 latestData = currentString;
  var sensors = split(currentString, ","); //slpit the string called current string on the commas
  if (sensors.length > 2){
    xPos = int(sensors[0]);
    yPos = int(sensors[1]);
    button = int(sensors[2]);
  }
}

function draw() {
 background('#c6e2ff');
  let c = color('magenta');
 fill(c);
 text(latestData, 10, 10);
  
for (let i = 0; i < num; i++) {
  circle(xPos/10, yPos/10, 5);
    }
  
  if (button == 0) {
    clear();
    background('#c6e2ff');
  }
}