astronaut.unpack();

var launchDemoButton = Button() ("Launch demo");

launchDemoButton.on("click", function() {
    nano.launch("hello", true);
});

astronaut.render(Screen(true) (
    launchDemoButton
));