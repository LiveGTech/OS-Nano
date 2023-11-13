nano.hello();

print("Hello from an app stored in the filesystem!");

var screen = new nano.Element("Screen");
var container = new nano.Element("Container");
var counterText = new nano.Element("Paragraph");
var subtext = new nano.Element("Paragraph");

screen.add(container, counterText, subtext);

nano.render(screen);

screen.print();

var containerChild = new nano.Element("Paragraph");

container.add(containerChild);

screen.print();