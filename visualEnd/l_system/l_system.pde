import java.util.*;

boolean isCapital(char c) {
  return((c>='A')&&(c<='Z'));
}

boolean isAlpha(char c) {
  return(((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z')));
}

class transf {
  float vx1, vx2, vy1, vy2;
  transf(float xmin, float xmax, float ymin, float ymax) {
    if (xmax==xmin) {
      xmax+=0.01;
    }    
    if (ymax==ymin) {
      ymax+=0.01;
    }
    if ((xmax-xmin)<(ymax-ymin)) {
      vy1=ymin;
      vy2=ymax;
      vx1=xmin;
      vx2=xmin+(ymax-ymin);
    } else {
      vy1=ymin;
      vy2=ymin+(xmax-xmin);
      vx1=xmin;
      vx2=xmax;
    }
  }
  PVector transform(PVector in) {
    return(new PVector(map(in.x, vx1, vx2, 10, width-10), map(in.y, vy1, vy2, 10, width-10)));
  };
}

class TState {
  int angle;
  PVector pos;
  TState(int a, PVector p) {
    angle=a;
    pos=p;
  }
  TState clone() {
    return(new TState(angle, pos));
  }
}

class Segment {
  PVector start;
  PVector end;
  color col=color(255);
  int weight=1;
  Segment(PVector start, PVector end) {
    this.start=start;
    this.end=end;
  }
  Segment(PVector start, PVector end, color col) {
    this(start, end);
    this.col=col;
  }  
  void draw(PGraphics canvas, transf tran) {
    canvas.stroke(col);
    canvas.strokeWeight(weight);

    PVector p1=tran.transform(start);
    PVector p2=tran.transform(end);

    canvas.line(p1.x, p1.y, p2.x, p2.y);
    //println((start.x+off)*fac+100, (start.y+off)*fac+100, (end.x+off)*fac+100, (end.y+off)*fac+100,off,fac);
  }
}

float depth=1;

class Turtle {
  String myString;

  float xmax, ymax, xmin, ymin;
  ArrayList<Segment> segs; //I wanna have segs
  PGraphics canvas;


  double angStep=PI/3;
  double len=2;

  TState myState=new TState(0, new PVector(0, 0));

  ArrayDeque<TState> stack;
  Turtle(String input, PGraphics canvas) {
    stack=new ArrayDeque<TState>();
    myString=input;
    this.canvas=canvas;
  }
  PVector mapV(PVector in) {
    return(new PVector(map(in.x, xmin, xmax, 0, width), map(in.y, ymin, ymax, 0, height)));
  }
  void genSegs() {
    segs=new ArrayList<Segment>();
    char c=' ';
    xmax=-Float.MAX_VALUE;
    ymax=-Float.MAX_VALUE;
    xmin=Float.MAX_VALUE;
    ymin=Float.MAX_VALUE;

    myState.angle=0;

    for (int i=0; i<myString.length(); i++) {
      c=myString.charAt(i);

      switch(c) {
      case '+':
        myState.angle++;
        break;
      case '-':
        myState.angle--;
        break;
      case '[':
        stack.addLast(myState);
        myState=myState.clone();
        break;
      case ']':
        myState=stack.removeLast();
        break;
      }
      PVector newPos=new PVector(0, 0);
      if (isAlpha(c)) {
        //println(newPos, pos);
        newPos=myState.pos.copy();
        if (isCapital(c)) {
          newPos=myState.pos.copy().add(PVector.fromAngle((float)(myState.angle*angStep)).setMag((float)len));
          //Einfärben
          segs.add(new Segment(myState.pos, newPos, lerpColor(color(255, 255, 0), color(0, 255, 255), (float)stack.size()/depth)));
        }

        myState.pos=newPos;
      }
      if (myState.pos.x>xmax) {
        xmax=myState.pos.x;
      }      
      if (myState.pos.y>ymax) {
        ymax=myState.pos.y;
      }      
      if (myState.pos.x<xmin) {
        xmin=myState.pos.x;
      }
      if (myState.pos.y<ymin) {
        ymin=myState.pos.y;
      }
    }
  }

  void draw() {
    transf t=new transf(xmin, xmax, ymin, ymax);
    //println(t.vmin, t.vmax, xmin, xmax, ymin, ymax);
    for (int i=0; i<segs.size(); i++) {
      segs.get(i).draw(canvas, t);
    }
  }
  
  void save(){
    
    PrintWriter output;
    String filePath="path_lsys.csv";
    output = createWriter(filePath);
    
    transf tran=new transf(xmin, xmax, ymin, ymax);
    //println(t.vmin, t.vmax, xmin, xmax, ymin, ymax);
    for (int i=0; i<segs.size(); i++) {
      Segment s=segs.get(i);
      PVector p1=tran.transform(s.start);
      PVector p2=tran.transform(s.end);
      if(i==0){
            output.println(p1.x+","+p1.y);
      }
      output.println(p2.x+","+p2.y);
      
    }
    output.flush(); // Writes the remaining data to the file
    output.close(); // Finishes the file
  
  }
}
float lastTime=0;
class Rule {
  String l="";
  String r="";
  Rule(String left, String right) {
    l=left;
    r=right;
  }
}

class LSys {
  char[] variable;
  char[] constant;
  String start;
  ArrayList<Rule> rulez;

  int level=0;
  String iter="";

  LSys(String start) {
    this.start=start;
    iter=start;
    rulez=new ArrayList<Rule>();
  }
  void addRule(Rule R) {
    rulez.add(R);
  }
  void step() {
    float startT=millis();
    println("Started!");
    String newS="";
    String cpy=new String(iter);
    //int[] firstInd=new int[rulez.size()];
    //for(int i=0;i<rulez.size();i++){
    //  firstInd=cpy.indexOf(rulez[i]);
    //}
    while (cpy.length()>0) {
      //println(cpy, newS);
      boolean rulemet=false;
      for (int i=0; i<rulez.size(); i++) { 
        Rule r=rulez.get(i);
        //println(r.l,r.r,r.l.length());
        if (!rulemet&cpy.length()>=r.l.length()) {
          if (cpy.substring(0, r.l.length()).equalsIgnoreCase(r.l)) {
            newS+=r.r;
            cpy=cpy.substring(r.l.length(), cpy.length());
            rulemet=true;
          }
        }
      }
      if (!rulemet) {
        newS+=cpy.charAt(0);
        cpy=cpy.substring(1, cpy.length());
      }
    }
    float time=millis()-startT;
    iter=newS;
    println("Iterating took: "+time+" millis, "+(time/lastTime)+" times as long");
    lastTime=time;
    depth++;
  }
}

/*void keyPressed() {
 if (key==32) {
 testL.step();
 
 }
 }*/
void mousePressed() {
  testL.step();
  println(testL.iter.length());
}

void keyPressed(){
  testTurt.save();


}
Turtle testTurt;
LSys testL;

void setup() {
  size(1000, 1000);
  //fullScreen();
  background(0);
  String test="";

  testTurt=new Turtle(test, this.g);

  testTurt.angStep=(PI/2);

  testL =new LSys("F");
  testL.addRule(new Rule("F","F+F-"));

  //testL.addRule(new Rule("b","-bF+aFa+Fb-"));
  //testL.addRule(new Rule("b","+aF-bFb-Fa+"));
  
  //testL.addRule(new Rule("F","G+F+G"));
  //testL.addRule(new Rule("G","F-G-F"));
  //testL.addRule(new Rule("F","F+G"));
  //testL.addRule(new Rule("G","F-G"));
  
  
  //testL.addRule(new Rule("F","FF"));
  //testL.addRule(new Rule("F", "F[+XF][-FX]"));
  //testL.addRule(new Rule("X", "X"));
}

void draw() {

  background(0);
  delay(10);

  //testTurt.myString+="F+F-F-F+"+(random(1)>0.5?"F+":"-");
  //println("test iter: "+testL.iter);
  testTurt.myString=testL.iter;
  testTurt.genSegs();
  testTurt.draw();
};
