


void setup() {
  size(800, 800);
  selectInput("Select a file to process:", "fileSelected");
  background(255);
  stroke(0);
  colorMode(HSB);
};

void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
  }
}