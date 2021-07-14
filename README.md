# JsToDef

[![Build Status](https://github.com/AGulev/jstodef/workflows/Build%20with%20bob/badge.svg)](https://github.com/AGulev/jstodef/actions)

This is [Native Extension](https://www.defold.com/manuals/extensions/) for the [Defold Game Engine](https://www.defold.com) that makes possible to send messages from JavaScript to Lua in [HTML5 build](https://www.defold.com/manuals/html5/).

## Setup

You can use the JsToDef extension in your own project by adding this project as a [Defold library dependency](https://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

https://github.com/AGulev/jstodef/archive/master.zip

Or point to the ZIP file of a [specific release](https://github.com/AGulev/jstodef/releases).

## API JavaScript side

`JsToDef.send(message_id, message)`

Where `message_id` is a string that helps you to identify this message on Lua side.

`message` is a custom value that might be one of the next types: object, number, boolean, string, undefined(if you don't need any extra data).

##### Example:

```javascript
JsToDef.send("MyCustomMessageName", "custom message");
JsToDef.send("ObjectEvent", {foo:"bar", num:16, isAv:true});
JsToDef.send("FloatEvent", 19.2);
JsToDef.send("IntEvent", 18);
JsToDef.send("StrintEvent", "custom string");
JsToDef.send("EmptyEvent");
JsToDef.send("BooleanEvent", true);
JsToDef.send("BooleanEvent", false);
```

## API Lua side

If you are working on cross-platform application the best practice to check the existence of jstodef module, this module exists only in html5 build:
```lua
if jstodef then
  -- any operations with jstodef
end
```
### Add Listener

`jstodef.add_listener(listener)`

Where `listener` is a function with the next parameters:

`self` is the current script self.

`message_id` is a string that helps you to identify this message.

`message` is a custom value that might be one of the next types: table, number, boolean, string, nil.

It is possible to add a few listeners.

##### Example:

```lua
local function js_listener(self, message_id, message)
  if message_id == "MyCustomMessageName" then
    -- do something
  end
end

function init(self)
  if jstodef then
    jstodef.add_listener(js_listener)
  end
end
```

### Remove Listener

`jstodef.remove_listener(listener)`

Where `listener` is the function that was previously added as a listener with `jstodef.add_listener()` method.

```lua
function final(self)
  if jstodef then
    jstodef.remove_listener(js_listener)
  end
end
```

## Issues and suggestions

If you have any issues, questions or suggestions please [create an issue](https://github.com/agulev/jstodef/issues) or contact me: me@agulev.com
