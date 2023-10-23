/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

var nano = {};

nano.Element = class {
    constructor(type) {
        this._type = type;

        this._parent = null;
        this._children = [];
    }

    add() {
        for (var i = 0; i < arguments.length; i++) {
            this._children.push(arguments[i]);

            arguments[i].parent = this;
        }

        return this;
    }

    _print(indent, lastChild) {
        var indents = "    ".repeat(indent);
        var delimeter = lastChild ? "" : ",";

        if (this._children.length == 0) {
            print(`${indents}${this._type}() ()${delimeter}`);

            return;
        }

        print(`${indents}${this._type} (`);

        for (var i = 0; i < this._children.length; i++) {
            this._children[i]._print(indent + 1, i == this._children.length - 1);
        }

        print(`${indents})${delimeter}`);
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