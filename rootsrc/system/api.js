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
    "Paragraph": 3
};

const _nano_elementProps = {
    NONE: 0,
    SHOWING: 1,
    TEXT: 2
};

var nano = {};

nano.Element = class {
    constructor(type) {
        this._type = type;

        this._parent = null;
        this._children = [];
        this._id = null;
        this._propsToSet = {};
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

        var propIdsToSet = Object.keys(this._propsToSet);

        for (var i = 0; i < propIdsToSet.length; i++) {
            var propId = propIdsToSet[i];

            _nano_setElementProp(this._id, Number(propId), this._propsToSet[propId]);
        }

        this._propsToSet = {};

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

    setText(text) {
        this._setProp(_nano_elementProps.TEXT, text);
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