nano.hello();

print("Hello from an app stored in the filesystem!");

var screen = new nano.Element("Screen");
var container = new nano.Element("Container");
var counterText = new nano.Element("Paragraph");
var subtext = new nano.Element("Paragraph");

screen.add(container, counterText, subtext);

counterText.setText("1234");
subtext.setText("LiveG OS Nano");

nano.render(screen);

screen.print();

var containerChild = new nano.Element("Paragraph");
var longContainerChild = new nano.Element("Paragraph");
var anotherLongContainerChild = new nano.Element("Paragraph");

containerChild.setText("Hello, world!");
longContainerChild.setText("This is a test of the Astronaut Nano framework within LiveG OS Nano.");
anotherLongContainerChild.setText("As you can see, it works very similarly to Adapt UI's Astronaut, but it's running on a small device!");

container.add(containerChild, longContainerChild, anotherLongContainerChild);

screen.print();

// screen.screenJump();