nano.hello();

print("Hello from an app stored in the filesystem!");

astronaut.unpack();

var counter = Paragraph() ();
var timestamp = Paragraph() ();
var itemsContainer = Container() ();
var addItemButton = Button() ("Add item");
var launchAnotherAppButton = Button() ("Launch another app");
var exitButton = Button() ("Exit");
var i = 0;

var Item = astronaut.component("Item", function(props, children) {
    var message = Paragraph() (props.message);
    var button = Button() ("Remove");

    var container = Container (
        message,
        button
    );

    button.on("click", function() {
        container.remove();
    });

    return container;
});

var screen = Screen(true) (
    Container (
        Paragraph() ("Hello, world!"),
        Paragraph() ("This is a test of Astronaut in LiveG OS Nano."),
        counter,
        timestamp
    ),
    Paragraph() ("As you can see, it works very similarly to Adapt UI's Astronaut, but it's running on a small device!"),
    Paragraph() ("How cool is that?!"),
    launchAnotherAppButton,
    exitButton,
    itemsContainer,
    addItemButton
);

launchAnotherAppButton.on("click", function() {
    nano.launch("hello2");
});

exitButton.on("click", function() {
    nano.back();
});

addItemButton.on("click", function() {
    itemsContainer.add(Item({message: "You added me at time " + Date.now() + "!"}) ());
});

astronaut.render(screen);

screen.print();

setInterval(function() {
    counter.setText("Count: " + i);

    i++;
}, 1000);

setInterval(function() {
    timestamp.setText("Date.now(): " + Date.now());
});