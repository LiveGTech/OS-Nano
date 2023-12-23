astronaut.unpack();

var exitButton = Button() ("Exit");

exitButton.on("click", function() {
    nano.close();
});

astronaut.render(Screen(true) (
    Paragraph() ("Hello, world!"),
    exitButton
));