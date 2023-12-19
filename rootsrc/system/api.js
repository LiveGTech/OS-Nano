/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

const _NANO_ELEMENT_TYPE_IDS = {
    "": 0,
    "Screen": 1,
    "Container": 2,
    "Text": 3,
    "Paragraph": 4,
    "Button": 5
};

const _NANO_EVENT_TYPE_IDS = {
    "": 0,
    "click": 1
};

const _NANO_TEXTUAL_ELEMENTS = {
    "Text": true,
    "Paragraph": true
};

const _nano_elementProps = {
    NONE: 0,
    SHOWING: 1,
    TEXT: 2
};

var _currentTimestamp = 0;
var _timers = [];
var _elementsById = {};

class Date {
    static now() {
        return Math.floor(_nano_timing_getCurrentTime());
    }
};

class Event {
    constructor(type) {
        this.type = type;
    }
};

function _nano_nextTick(timestamp) {
    _currentTimestamp = timestamp;

    _timers.forEach(function(timer, id) {
        if (timer == null) {
            return;
        }

        if (timestamp > timer.nextDue) {
            timer.callback();

            if (timer.indefinite) {
                timer.nextDue = timestamp + timer.duration;
            } else {
                _timers[id] = null;
            }
        }
    });
}

function _nano_processEvent(eventType, targetId) {
    var eventTypeName = null;

    Object.keys(_NANO_EVENT_TYPE_IDS).forEach(function(type) {
        if (_NANO_EVENT_TYPE_IDS[type] == eventType) {
            eventTypeName = type;
        }
    });

    if (eventTypeName == null) {
        return;
    }

    var event = new Event(eventTypeName);

    event.target = _elementsById[targetId];

    event.target._emit(event);
}

function setTimeout(callback, duration) {
    duration = duration || 0;

    _timers.push({callback: callback, duration: duration, nextDue: _currentTimestamp + duration, indefinite: false});

    return _timers.length - 1;
}

function setInterval(callback, duration) {
    duration = duration || 0;

    _timers.push({callback: callback, duration: duration, nextDue: _currentTimestamp + duration, indefinite: true});

    return _timers.length - 1;
}

function clearTimeout(id) {
    _timers[id] = null;
}

function clearInterval(id) {
    _timers[id] = null;
}

var nano = {};

nano.Element = class {
    constructor(type) {
        this._type = type;

        this._parent = null;
        this._children = [];
        this._id = null;
        this._propsToSet = {};
        this._shouldListenForEvents = false;
        this._eventListeners = [];
    }

    add() {
        for (var i = 0; i < arguments.length; i++) {
            var child = arguments[i];

            this._children.push(arguments[i]);

            arguments[i]._parent = this;

            child._register();
        }

        return this;
    }

    remove() {
        this.clear();

        for (var i = 0; i < this._parent._children.length; i++) {
            if (this._parent._children[i] == this) {
                this._parent._children.splice(i, 1);

                break;
            }
        }

        _nano_removeElement(this._id);

        this._id = null;
    }

    clear() {
        while (this._children.length > 0) {
            this._children[0].remove();
        }
    }

    _print(indent, lastChild) {
        var indents = "    ".repeat(indent);
        var delimeter = lastChild ? "" : ",";

        if (this._children.length == 0) {
            print(`${indents}${this._type}(${this._id}) ()${delimeter}`);
            // print(`${indents}${this._type}() ()${delimeter}`);

            return;
        }

        print(`${indents}${this._type}(${this._id}) (`);
        // print(`${indents}${this._type} (`);

        for (var i = 0; i < this._children.length; i++) {
            this._children[i]._print(indent + 1, i == this._children.length - 1);
        }

        print(`${indents})${delimeter}`);
    }

    _register() {
        if (this._id != null) {
            return;
        }

        if (this._type != "Screen" && this._parent._id == null) {
            return;
        }

        this._id = _nano_addElement(this._parent ? this._parent._id : null, _NANO_ELEMENT_TYPE_IDS[this._type]);

        _elementsById[this._id] = this;

        var propIdsToSet = Object.keys(this._propsToSet);

        for (var i = 0; i < propIdsToSet.length; i++) {
            var propId = propIdsToSet[i];

            _nano_setElementProp(this._id, Number(propId), this._propsToSet[propId]);
        }

        this._propsToSet = {};

        if (this._shouldListenForEvents) {
            _nano_listenForEvents(this._id);
        }

        for (var i = 0; i < this._children.length; i++) {
            this._children[i]._register();
        }
    }

    _setProp(propId, propValue) {
        if (this._id == null) {
            this._propsToSet[propId] = propValue;

            return;
        }

        _nano_setElementProp(this._id, propId, propValue);
    }

    on(eventType, callback) {
        this._shouldListenForEvents = true;

        if (this._id != null) {
            _nano_listenForEvents(this._id);
        }

        this._eventListeners.push({type: eventType, callback: callback});
    }

    _emit(event) {
        this._eventListeners.forEach(function(listener) {
            if (listener.type == event.type) {
                listener.callback(event);
            }
        });
    }

    setText(text) {
        if (_NANO_TEXTUAL_ELEMENTS[this._type]) {
            this._setProp(_nano_elementProps.TEXT, text);
        } else {
            this.clear();

            this.add(Text(text));
        }
    }

    screenJump() {
        this._setProp(_nano_elementProps.SHOWING, true);
    }

    print() {
        this._print(0, true);

        return this;
    }
};

class ClassTest {
    constructor() {
        this.message = "Testing classes!";

        this.output();
    }

    output() {
        print(...[this.message]);
    }

    eachArg() {
        for (var i = 0; i < arguments.length; i++) {
            print("Argument " + i + ": " + arguments[i]);
        }
    }
}

nano.hello = function() {
    print("Hello, world! This has been called from the LiveG OS Nano API.");

    var classTest = new ClassTest();

    classTest.eachArg(...["a", "b", "c"]);
};

nano.render = function() {
    for (var i = 0; i < arguments.length; i++) {
        arguments[i]._register();
    }
};

var astronaut = {};

astronaut.components = {};
astronaut.unpacked = false;

astronaut.unpack = function() {
    Object.keys(astronaut.components).forEach(function(name) {
        globalThis[name] = astronaut.components[name];
    });

    astronaut.unpacked = true;
};

astronaut.component = function(options, init) {
    var _options = options;

    if (typeof(options) == "string") {
        _options = {name: options};
    }

    var newComponent = function() {
        var args = [...arguments];
        var props = {};
        var inter = {};
        var children = [];
        var isSplittingChildren = true;

        function create() {
            var createdComponent = init(props, children, inter);

            createdComponent.inter = inter;

            return createdComponent;
        }

        if (args.length > 0) {
            if (Array.isArray(args[0])) {
                isSplittingChildren = false;
            } else if (args[0] instanceof nano.Element) {
                children = [];

                args.forEach(function(arg) {
                    if (Array.isArray(arg)) {
                        children.push(...arg);

                        return;
                    }

                    children.push(arg);
                });

                isSplittingChildren = false;
            } else if (typeof(args[0]) == "object") {
                props = args[0];
            } else {
                props.args = args;

                if (_options.positionals) {
                    args.forEach(function(arg, i) {
                        props[_options.positionals[i]] = arg;
                    });
                }
            }
        } else {
            props = _options.default || [];
        }

        if (isSplittingChildren) {
            return function() {
                children = [];

                [...arguments].forEach(function(arg) {
                    if (Array.isArray(arg)) {
                        children.push(...arg);

                        return;
                    }

                    children.push(arg);
                });

                for (var i = 0; i < children.length; i++) {
                    if (typeof(children[i]) != "object") {
                        children[i] = String(children[i]);
                    }
                }

                return create();
            };
        }

        return create();
    };

    if (!_options.isPrivate) {
        astronaut.components[_options.name] = newComponent;

        if (astronaut.unpacked) {
            globalThis[_options.name] = newComponent;
        }
    }

    return newComponent;
};

astronaut.render = function(component) {
    nano.render(component);
};

function elementBase(name, options) {
    var element = new nano.Element(name);

    return element;
}

function containerElement(name, options) {
    return function() {
        var element = elementBase(name, options);

        [...arguments].forEach(function processArg(arg) {
            if (Array.isArray(arg)) {
                arg.forEach(function(subArg) {
                    processArg(subArg);
                });

                return;
            }

            if (typeof(arg) == "string") {
                element.add(astronaut.components.Text(arg));

                return;
            }

            element.add(arg);
        });

        return element;
    };
}

function textualElement(name, options) {
    return function() {
        var element = elementBase(name, options);

        element.setText([...arguments].map(function(arg) {
            return String(arg);
        }).join(""));

        return element;
    };
}

astronaut.components.Text = function(text) {
    var element = new nano.Element("Text");

    element.setText(text);

    return element;
};

astronaut.component({name: "Screen", positionals: ["showing"]}, function(props, children) {
    var screen = containerElement("Screen", props) (...children);

    if (props.showing) {
        screen.screenJump();
    }

    return screen;
});

astronaut.component("Container", function(props, children) {
    return containerElement("Container", props) (...children);
});

astronaut.component("Paragraph", function(props, children) {
    return textualElement("Paragraph", props) (...children);
});

astronaut.component("Button", function(props, children) {
    return containerElement("Button", props) (...children);
});