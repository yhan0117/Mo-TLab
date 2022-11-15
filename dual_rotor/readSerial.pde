import processing.serial.*; // add the serial library
Serial myPort; // define a serial port object to monitor

// Define initial "time" coordinates of cursor location
int x = 1251;
long time, time2;
float a_prev = 0, w_prev = 0;

// Data table and filename specifications
Table table;
String title = month()+"."+day()+"-"+hour()+"h"+minute()+"m-"+second()+"s"+".csv";

void setup() {
  size(1300, 675); // set the window size
  myPort = new Serial(this, Serial.list()[1], 9600); // define input port
  myPort.clear(); // clear the port of any initial junk

  table = new Table(); // Create a table to save data as a csv file

  table.addColumn("Time");
  table.addColumn("Angle");
  table.addColumn("Speed");

  time = millis();
  time2 = millis();
}

void draw () {

  while (myPort.available () > 0) { // make sure port is open
    String inString = myPort.readStringUntil('\n'); // read input string

    if (inString != null) { // ignore null strings
      
      inString = trim(inString); // trim off any whitespace
      String[] states = splitTokens(inString, ","); // extract x & y into an array
      // proceed only if correct # of values extracted from the string:

      if (states.length == 2) {
        
        float a = float(states[0]);
        float w = float(states[1]);
        int t = (int)(millis()-time);
        TableRow newRow = table.addRow();
        newRow.setInt("Time", t);
        newRow.setFloat("Angle", a);
        newRow.setFloat("Speed", w);

        //a = a*10.0 + 300;
        a = a*600.0/1024+25;
        w = w/2.0 + 300;
        stroke(200,0,0);
        strokeWeight(3);
        line(x-1, a_prev, x, a);

        x = x + 1;

        a_prev = a;
        w_prev = w;
        // Set the bounds inside with the cursor can translate
        // This prevents the cursor from moving off the screen
        if (x > 1250) {
          background(255, 255, 255); // pick the fill color (r,g,b). Feel free to change this.
          grid();
          ylegend();
          int dt = (int)(millis() - time2);
          time2 = millis();
          xlegend(t, dt);
          x = 50;
        }
      }
    }
  }
  // Save file to .csv file containing month, day, hour, minute, and second
  //saveTable(table, title);
}

void grid() {
  //y grid
  for (int i = 0; i <= 6; i++) {
    stroke(200);
    strokeWeight(1);
    line(50, 25+100*i, 1250, 25+100*i);
  }
  
  //x grid
  for (int i = 0; i <= 12; i++) {
    stroke(200);
    strokeWeight(1);
    line(50+100*i, 25, 50+100*i, 625);
  }
}

void ylegend(){
  for (int i = 0; i <= 6; i++) {
    textSize(15);
    textAlign(RIGHT);
    text(-30+10*i, 40, 630-100*i); 
    fill(100,100,100);
  }
}

void xlegend(int t, int dt){
  for (int i = 0; i <= 12; i++) {
    textSize(15);
    textAlign(CENTER);
    text((t + i*dt/12)/1000.0, 50+100*i, 660); 
    fill(100,100,100);
  }
}
