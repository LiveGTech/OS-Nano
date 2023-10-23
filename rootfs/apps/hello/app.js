nano.hello();

print("Hello from an app stored in the filesystem!");

var container = new nano.Element("Container");
var counterText = new nano.Element("Paragraph");
var subtext = new nano.Element("Paragraph");

container.add(counterText, subtext);

container.print();