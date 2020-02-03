

PImage img;
float scale=4;

int numPoints=30000;
float rf=0.1;
boolean invert=false;
float[][] outp;
void setup() {
  size(800, 800);
  selectInput("Select a file to process:", "fileSelected");
  background(255);
  stroke(0);
  colorMode(HSB);
};

void draw() {
  background(0);
  stroke (255);
  strokeWeight(1);
  if (points!=null) {
    drawPoints();
  }
  if (loadedPath!=null) {
    strokeWeight(1);
    loadedPath.drawPath();
  }
};

float zf=1;
float[][] diff1={{0, 1}, {1, 0}};
float[][] diff2={{0, 0, 7}, {3, 5, 1}};

PVector[] points;


boolean doImprove=false;
void keyPressed() {
  background(0);
  if (key=='f') {
    ditherErrDiff(img, outp, diff1);
    points=pointsFromImage(outp);
  }

  if (key=='s') {
    if (points!=null) {
      println("saving");
      saveTSP(points);
    }
  }
  if (key=='g') {

    points=monteCarlo(img, numPoints);
  }
  if (key=='l') {
    selectInput("Load a Path:", "loadPath");
  }
  if (key=='o') {
    selectInput("Load order:", "loadOrder");
  }
  if (keyCode==UP) {
    zf*=1.1;
  }
  if (keyCode==DOWN) {
    zf/=1.1;
  }
  stroke(color(255));
  strokeWeight(1);
  //drawPoints();
};

PVector[] monteCarlo(PImage src, int num) {
  PVector[] aim=new PVector[num];
  src.loadPixels();
  int i=0;
  while (i<num) {
    float x=random(src.width);
    float y=random(src.height-1);
    float b=255-brightness(src.pixels[int(y)*src.width+int(x)]);

    boolean accept=int(random(255*255))<b*b;
    //println(x, y, b, accept);
    if (accept) {
      aim[i]=new PVector(x*scale, y*scale);
      i++;
    }
  }

  return aim;
}


void ditherErrDiff(PImage src, float[][] aim, float[][]diffmask) {
  if (aim!=null) {
    for (int i =0; i<aim.length; i++) {
      for (int j=0; j<aim[i].length; j++) {
        aim[i][j]=0;
        //println(aim[i][j]);
      }
    }
    float maskSum=0;
    float[][] maskNorm=new float[diffmask.length][diffmask[0].length];
    for (int i =0; i<diffmask.length; i++) {
      for (int j=0; j<diffmask[i].length; j++) {
        maskSum+=diffmask[i][j];
      }
    }
    for (int i =0; i<diffmask.length; i++) {
      for (int j=0; j<diffmask[i].length; j++) {
        maskNorm[i][j]=diffmask[i][j]/maskSum;
      }
    }
    src.loadPixels();
    loadPixels();
    for (int i =0; i<aim.length; i++) {
      for (int j=0; j<aim[i].length; j++) {
        //float val=(brightness(src.pixels[j*src.width+i])<128?255:0)+aim[i][j];
        float b=brightness(src.pixels[j*src.width+i]);
        //float val=(b>100&&b<150?255:0)+aim[i][j];
        float val=b+aim[i][j];
        //println(val,aim[i][j]);
        aim[i][j]=val>128?255:0;
        float err=val-aim[i][j];

        //println(i,j,err);
        /*try {
         pixels[j*width+i]=color(aim[i][j]);
         }
         catch(ArrayIndexOutOfBoundsException e) {
         }*/
        for (int k =0; k<maskNorm.length&&i+k<aim.length; k++) {
          for (int l=0; l<maskNorm[k].length&&j+l<aim[i+k].length; l++) {

            aim[i+k][j+l]+=maskNorm[k][l]*err;
            //println(maskNorm[k][l],aim[i+k][j+l]);
          }
        }
      }
    }
    updatePixels();
  } else {
    println("ayyy");
  }
}

void drawPoints() {
  for (PVector p : points) {
    point(p.x*zf, p.y*zf);
  }
};

PVector[] pointsFromImage(float[][] src) {
  ArrayList<PVector> pzw=new ArrayList<PVector>();
  for (int i =0; i<src.length; i++) {
    for (int j=0; j<src[i].length; j++) {
      if (src[i][j]!=0) {
        pzw.add(new PVector(i*scale, j*scale).add(PVector.random2D().mult(random(rf*scale))));
        //pzw.add(new PVector(i*scale, j*scale));//).add(PVector.random2D().mult(random(scale/2))));
      }
    }
  }
  PVector[] points=new PVector[pzw.size()];
  int i=0;
  for (PVector p : pzw) {
    points[i]=p;
    i++;
  }
  return points;
}
void saveTSP(PVector[] points) {
  PrintWriter pw;
  pw=createWriter(fileName+".tsp");
  pw.println("NAME : "+fileName);
  pw.println("COMMENT : processing rocks, flix was here");
  pw.println("TYPE : TSP");
  pw.println("DIMENSION : "+points.length);
  pw.println("EDGE_WEIGHT_TYPE : EUC_2D");
  pw.println("NODE_COORD_SECTION");
  for (int i=0; i<points.length; i++) {
    pw.println((i+1)+" "+points[i].x + " "+points[i].y);
  };
  pw.println("EOF");
  pw.flush();
  pw.close();
}
String fileName="ayyy";
void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    img=loadImage(selection.getAbsolutePath());
    fileName=selection.getName().replaceFirst("[.][^.]+$", "");


    println(img.height, displayHeight);
    while (img.height>displayHeight) {
      println("DIVIDE!");
      img.resize(img.width/2, img.height/2);
    }
    println(img.width/2, displayWidth);
    while (img.width*2>displayWidth) {
      println("DIVIDE!");
      img.resize(img.width/2, img.height/2);
    }
    img.resize(int(img.width/scale), int(img.height/scale));
  }
  zf=min((float(height)/img.height)/scale, (float(width)/img.width)/scale);
  outp=new float[img.width][img.height];
}
Path loadedPath=null;
String orderPath=null;

void loadPath(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    if (orderPath!=null) {
      delay(2000);
      loadedPath=new Path(selection.getAbsolutePath(), orderPath);
    }
  }
}

void loadOrder(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    orderPath= selection.getAbsolutePath();
  }
}

class Path {

  ArrayList<PVector> points=new ArrayList<PVector>();
  int[] path;

  Path(String path, String orderPath) {
    String[] lines = loadStrings(path);
    //points=new PVector[lines.length-7];
    for (int i=0; i<lines.length-6; i++) {
      println(lines[i]);
      float[] nums=float(lines[i+6].split(" "));
      points.add(new PVector(nums[1], nums[2]));
    }
    String[] olines=loadStrings(orderPath);
    this.path=new int[olines.length];
    for (int i=0; i<this.path.length; i++) {
      this.path[i]=int(olines[i]);
    }
  }



  void drawPath() {
    PVector last=points.get(path[0]);
    PVector n;
    float totalLength=0;
    for (int i=1; i<path.length; i++) {
      n=points.get(path[i]);
      line(last.x*zf, last.y*zf, n.x*zf, n.y*zf);
      totalLength+=last.dist(n);
      stroke(map(float(i)/path.length, 0, 1, 0, 255), 255, 255);
      last=n;
    }
  };
}